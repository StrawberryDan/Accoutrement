#include "Codec/Frame.hpp"



#include <cassert>



Frame::Frame()
    : mFrame(nullptr)
{
    mFrame = av_frame_alloc();
    assert(mFrame != nullptr);
}



Frame::Frame(const Frame& other)
    : mFrame(nullptr)
{
    assert(other.mFrame != nullptr);
    mFrame = av_frame_clone(other.mFrame);
    assert(mFrame != nullptr);
}



Frame& Frame::operator=(const Frame& other)
{
    if (this == &other) return (*this);
    assert(other.mFrame != nullptr);
    if (mFrame) av_frame_free(&mFrame);
    mFrame = av_frame_clone(other.mFrame);
    assert(mFrame != nullptr);
    return (*this);
}



Frame::Frame(Frame&& other) noexcept
    : Frame()
{
    av_frame_unref(mFrame);
    av_frame_move_ref(mFrame, other.mFrame);
    assert(mFrame != nullptr);
    other.mFrame = nullptr;
}



Frame& Frame::operator=(Frame&& other) noexcept
{
    av_frame_unref(mFrame);
    av_frame_move_ref(mFrame, other.mFrame);
    assert(mFrame != nullptr);
    other.mFrame = nullptr;
    return (*this);
}



Frame::~Frame()
{
    av_frame_free(&mFrame);
}



std::vector<Sample> Frame::Samples() const
{
    std::vector<Sample> samples;
    samples.resize(mFrame->nb_samples);
    memcpy(samples.data(), mFrame->data[0], mFrame->nb_samples * sizeof(Sample));
    return samples;
}
