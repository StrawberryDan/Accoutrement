#include "Codec/OpusEncoder.hpp"



#include "Codec/Frame.hpp"
#include "Codec/Packet.hpp"
#include <cassert>
#include <iostream>
#include <numeric>



OpusEncoder::OpusEncoder()
    : mContext(nullptr)
    , mParameters(nullptr)
    , mPTS(0)
    , mSampleBuffer()
{
    auto opusCodec = avcodec_find_encoder(AV_CODEC_ID_OPUS);
    assert(opusCodec != nullptr);
    assert(av_codec_is_encoder(opusCodec));
    mContext = avcodec_alloc_context3(opusCodec);
    assert(mContext != nullptr);
    mContext->strict_std_compliance = -2;
    mContext->time_base = AVRational{1, 48000};
    mContext->sample_rate = 48000;
    mContext->sample_fmt = AV_SAMPLE_FMT_FLT;
    mContext->ch_layout = AV_CHANNEL_LAYOUT_MONO;
    mContext->bit_rate = sizeof(float) * 48000;

    auto result = avcodec_open2(mContext, opusCodec, nullptr);
    assert(result == 0);
    assert(avcodec_is_open(mContext));

    mParameters = avcodec_parameters_alloc();
    assert(mParameters != nullptr);
    result = avcodec_parameters_from_context(mParameters, mContext);
    assert(result >= 0);
}



OpusEncoder::~OpusEncoder()
{
    avcodec_parameters_free(&mParameters);
    avcodec_close(mContext);
    avcodec_free_context(&mContext);
}



std::vector<Packet> OpusEncoder::Encode(const std::vector<Sample>& samples)
{
    mSampleBuffer.insert(mSampleBuffer.end(), samples.begin(), samples.end());
    if (mSampleBuffer.size() < mContext->frame_size)
    {
        return {};
    }

    std::vector<Frame> frames;
    unsigned int availableSamples = mSampleBuffer.size() / mContext->frame_size;
    std::vector<std::vector<Sample>> splitSamples;
    for (int i = 0; i < availableSamples; ++i)
    {
        splitSamples.emplace_back(mSampleBuffer.begin() + i * mContext->frame_size, mSampleBuffer.begin() + (i + 1) * mContext->frame_size);
    }
    mSampleBuffer.erase(mSampleBuffer.begin(), mSampleBuffer.begin() + availableSamples * mContext->frame_size);

    for (const auto& someSamples : splitSamples)
    {
        assert(someSamples.size() == mContext->frame_size);

        Frame frame;
        frame->sample_rate = 48000;
        frame->ch_layout = AV_CHANNEL_LAYOUT_MONO;
        frame->format = AV_SAMPLE_FMT_FLT;
        frame->nb_samples = someSamples.size();
        frame->pts = mPTS;
        frame->time_base = AVRational{1, 48000};
        frame->duration = someSamples.size();
        auto result = av_frame_get_buffer(*frame, 0);
        assert(result == 0);
        result = av_frame_make_writable(*frame);
        assert(result == 0);
        assert(av_frame_is_writable(*frame));
        memcpy(frame->data[0], someSamples.data(), someSamples.size() * sizeof(Sample));
        frames.push_back(std::move(frame));

        mPTS += someSamples.size();
    }


    std::vector<Packet> packets;
    for (const auto& frame : frames)
    {
        auto send = avcodec_send_frame(mContext, *frame);
        assert(send == 0);

        int receive;
        do
        {
            Packet packet;
            receive = avcodec_receive_packet(mContext, *packet);
            assert(receive == 0 || receive == AVERROR(EAGAIN));

            if (receive == 0)
            {
                packets.push_back(packet);
            }
        } while (receive == 0);
    }

    return packets;
}



std::vector<Packet> OpusEncoder::Finish()
{
    Frame frame;
    frame->sample_rate = 48000;
    frame->ch_layout = AV_CHANNEL_LAYOUT_MONO;
    frame->format = AV_SAMPLE_FMT_FLT;
    frame->nb_samples = mSampleBuffer.size();
    frame->pts = mPTS;
    frame->time_base = AVRational{1, 48000};
    auto result = av_frame_get_buffer(*frame, 0);
    assert(result == 0);
    result = av_frame_make_writable(*frame);
    assert(result == 0);
    assert(av_frame_is_writable(*frame));
    memcpy(frame->data[0], mSampleBuffer.data(), mSampleBuffer.size() * sizeof(Sample));

    std::vector<Packet> packets;
    auto send = avcodec_send_frame(mContext, *frame);
    assert(send == 0);

    int receive;
    do
    {
        Packet packet;
        receive = avcodec_receive_packet(mContext, *packet);
        assert(receive == 0 || receive == AVERROR(EAGAIN));

        if (receive == 0)
        {
            packets.push_back(packet);
        }
    } while (receive == 0);


    return packets;
}



AVCodecParameters* OpusEncoder::Parameters() const
{
    return mParameters;
}
