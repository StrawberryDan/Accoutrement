#pragma once



#include <thread>



#include "Util/Clock.hpp"
#include "Util/Mutex.hpp"
#include "Util/Net/Websocket/WebsocketClient.hpp"



class Heartbeat
{
public:
    Heartbeat(SharedMutex<WSSClient> wss, double interval);
    ~Heartbeat();

	void UpdateSequenceNumber(size_t value);

private:
    void Run();

    std::thread            mThread;
    const double           mInterval;
    Clock                  mClock;
    Mutex<bool>            mShouldStop;
    SharedMutex<WSSClient> mWSS;
    Option<Mutex<size_t>>  mLastSequenceNumber;
};
