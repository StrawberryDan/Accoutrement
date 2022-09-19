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
    mContext->sample_rate = 48000;
    mContext->time_base = AVRational{1, mContext->sample_rate};
    mContext->sample_fmt = AV_SAMPLE_FMT_FLTP;
    mContext->ch_layout = AV_CHANNEL_LAYOUT_STEREO;

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



std::vector<Packet> OpusEncoder::Encode(const Samples& samples)
{
    mSampleBuffer.Append(samples);
    if (mSampleBuffer.Size() < mContext->frame_size)
    {
        return {};
    }

    std::vector<Frame> frames;
    auto [split, leftover] = mSampleBuffer.Split(mContext->frame_size);
    mSampleBuffer = leftover;

    for (const auto& sampleFrame : split)
    {
        assert(sampleFrame.Size() == mContext->frame_size);

        Frame frame;
        frame->sample_rate = mContext->sample_rate;
        frame->ch_layout   = mContext->ch_layout;
        frame->format      = mContext->sample_fmt;
        frame->pts         = mPTS;
        frame->time_base   = mContext->time_base;
        frame->nb_samples  = sampleFrame.Size();
        frame->duration    = sampleFrame.Size();

        auto result = av_frame_get_buffer(*frame, 0);
        assert(result == 0);
        result = av_frame_make_writable(*frame);
        assert(result == 0);
        assert(av_frame_is_writable(*frame));
        memcpy(frame->data[0], sampleFrame.Left().data(),  sampleFrame.Left().size()  * sizeof(Sample));
        memcpy(frame->data[1], sampleFrame.Right().data(), sampleFrame.Right().size() * sizeof(Sample));
        frames.push_back(std::move(frame));

        mPTS += sampleFrame.Size();
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
    assert(mSampleBuffer.Size() < mContext->frame_size);

    Frame frame;
    frame->sample_rate = mContext->sample_rate;
    frame->ch_layout   = mContext->ch_layout;
    frame->format      = mContext->sample_fmt;
    frame->time_base   = mContext->time_base;
    frame->pts         = mPTS;
    frame->nb_samples  = mSampleBuffer.Size();
    frame->duration    = mSampleBuffer.Size();

    auto result = av_frame_get_buffer(*frame, 0);
    assert(result == 0);
    result = av_frame_make_writable(*frame);
    assert(result == 0);
    assert(av_frame_is_writable(*frame));
    memcpy(frame->data[0], mSampleBuffer.Left().data(),  mSampleBuffer.Left().size()  * sizeof(Sample));
    memcpy(frame->data[1], mSampleBuffer.Right().data(), mSampleBuffer.Right().size() * sizeof(Sample));

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
