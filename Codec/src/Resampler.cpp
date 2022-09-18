#include "Codec/Resampler.hpp"


#include <cassert>



static constexpr unsigned int TARGET_SAMPLE_RATE = 48000;



Resampler::Resampler()
    :mSwrContext(nullptr)
{

}



Resampler::Resampler(const AVCodecParameters* codecParameters)
    : mSwrContext(nullptr)
{
    AVChannelLayout outputLayout = AV_CHANNEL_LAYOUT_STEREO;
    auto result = swr_alloc_set_opts2(&mSwrContext, &outputLayout, AV_SAMPLE_FMT_S32, TARGET_SAMPLE_RATE, &codecParameters->ch_layout, static_cast<AVSampleFormat>(codecParameters->format), codecParameters->sample_rate, 0, nullptr);
    assert(result == 0);
}



Resampler::Resampler(Resampler&& other)
 noexcept {
    mSwrContext = other.mSwrContext;
    other.mSwrContext = nullptr;
}



Resampler& Resampler::operator=(Resampler&& other)
 noexcept {
    mSwrContext = other.mSwrContext;
    other.mSwrContext = nullptr;
    return (*this);
}



Resampler::~Resampler()
{
    swr_free(&mSwrContext);
}



Frame Resampler::Resample(const Frame& input)
{
    assert(mSwrContext != nullptr);

    Frame output;
    output->ch_layout = AV_CHANNEL_LAYOUT_MONO;
    output->format = AV_SAMPLE_FMT_FLT;
    output->sample_rate = TARGET_SAMPLE_RATE;
    auto result = swr_convert_frame(mSwrContext, *output, *input);
    assert(result == 0);

    return output;
}



std::vector<Frame> Resampler::Resample(const std::vector<Frame>& input)
{
    assert(mSwrContext != nullptr);
    std::vector<Frame> output;
    output.reserve(input.size());
    for (const auto& frame : input)
    {
        output.push_back(Resample(frame));
    }
    return output;
}
