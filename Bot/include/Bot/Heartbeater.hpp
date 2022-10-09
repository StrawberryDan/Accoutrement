#pragma once



#include <thread>



#include "Util/Clock.hpp"
#include "Util/Mutex.hpp"
#include "Util/Net/Websocket/WebsocketClient.hpp"



class Heartbeater
{
public:
    Heartbeater(SharedMutex<WSSClient> wss, double interval);
    ~Heartbeater();

private:
    void Run();

    std::thread            mThread;
    const double           mInterval;
    Clock                  mClock;
    Mutex<bool>            mShouldStop;
    SharedMutex<WSSClient> mWSS;
    Option<size_t>  mLastSequenceNumber;
};
