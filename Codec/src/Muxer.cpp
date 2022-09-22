#include "Codec/Muxer.hpp"



#include "Util/Utilities.hpp"
#include <cassert>



Muxer::Muxer(const std::string& file)
    : mAVFormatContext(nullptr)
    , mStreams()
    , mStage(Unopened)
{
    auto result = avformat_alloc_output_context2(&mAVFormatContext, nullptr, nullptr, file.c_str());
    assert(result >= 0);

    result = avio_open2(&mAVFormatContext->pb, file.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr);
    assert(result >= 0);
    mStage = Opened;
}



Muxer::Muxer(Muxer&& other) noexcept
    : mAVFormatContext(Take(other.mAVFormatContext))
    , mStreams(Take(other.mStreams))
    , mStage(Replace(other.mStage, Unopened))
{


}



Muxer& Muxer::operator=(Muxer&& other) noexcept {
    mAVFormatContext = Take(other.mAVFormatContext);
    mStreams = Take(other.mStreams);
    return (*this);
}



Muxer::~Muxer()
{
    assert(mStage == TrailerWritten || mStage == Finished);
    avio_close(mAVFormatContext->pb);
    avformat_free_context(mAVFormatContext);
}



void Muxer::OpenStream(AVCodecParameters* codecParameters)
{
    assert(mStage == Opened);
    auto encoder = avcodec_find_encoder(codecParameters->codec_id);
    assert(codecParameters != nullptr);
    auto stream = avformat_new_stream(mAVFormatContext, encoder);
    assert(stream != nullptr);
    auto result = avcodec_parameters_copy(stream->codecpar, codecParameters);
    assert(result >= 0);
    mStreams.push_back(stream);
}



void Muxer::WriteHeader()
{
    assert(mStage == Opened);
    auto result = avformat_write_header(mAVFormatContext, nullptr);
    assert(result >= 0);
    mStage = HeaderWritten;
}



void Muxer::WritePacket(Packet& packet)
{
    assert(mStage == HeaderWritten || mStage == WritingPackets);
    assert(mStreams.size() > packet->stream_index);
    auto result = av_write_frame(mAVFormatContext, *packet);
    assert(result >= 0);
    mStage = WritingPackets;
}



void Muxer::WriteTrailer()
{
    assert(mStage == WritingPackets);
    auto result = av_write_trailer(mAVFormatContext);
    assert(result >= 0);
    mStage = TrailerWritten;
}



void Muxer::Flush()
{
    assert(mStage == TrailerWritten);
    avio_flush(mAVFormatContext->pb);
    avformat_flush(mAVFormatContext);
    mStage = Finished;
}
