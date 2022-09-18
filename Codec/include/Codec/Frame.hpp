#pragma once



#include "Sample.hpp"
#include <vector>



extern "C"
{
#include "libavcodec/avcodec.h"
}


class Frame
{
public:
    Frame();
    Frame(const Frame& other);
    Frame& operator=(const Frame& other);
    Frame(Frame&& other) noexcept ;
    Frame& operator=(Frame&& other) noexcept ;
    ~Frame();


    inline       AVFrame* operator*()        { return mFrame; }
    inline const AVFrame* operator*()  const { return mFrame; }
    inline       AVFrame* operator->()       { return mFrame; }
    inline const AVFrame* operator->() const { return mFrame; }

    std::vector<Sample> Samples() const;

private:
    AVFrame* mFrame;
};
