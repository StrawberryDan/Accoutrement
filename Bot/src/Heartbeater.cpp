#include "Bot/Heartbeater.hpp"



#include <random>



Heartbeater::Heartbeater(SharedMutex<WSSClient> wss, double interval)
    : mWSS(wss)
    , mInterval(interval)
    , mShouldStop(false)
{
    mThread = std::thread(&Heartbeater::Run, this);
}



Heartbeater::~Heartbeater()
{
    (*mShouldStop.Lock()) = true;
    mThread.join();
}



void Heartbeater::Run()
{
    std::cout << "Beginning Heartbeat" << std::endl;

    std::random_device rd;
    std::uniform_real_distribution<double> jitterDist(0.0, 0.9 * mInterval);
    std::mt19937_64 rng(rd());
    double jitter = jitterDist(rng);

    std::cout << "Jitter: " << jitter << std::endl;

    mClock.Restart();
    while (mClock.Read() < jitter && !*mShouldStop.Lock())
    {
        std::this_thread::yield();
    }

    int count = 0;
    while (!*mShouldStop.Lock())
    {
        if (mClock.Read() > 0.9 * mInterval || count == 0)
        {
            nlohmann::json message;
            message["op"] = 1;
            message["d"] = mLastSequenceNumber ? nlohmann::json(*mLastSequenceNumber->Lock()) : nlohmann::json();
            WebsocketMessage wssMessage(to_string(message));

	        mWSS.Lock()->SendMessage(wssMessage);
            count += 1;
            mClock.Restart();
        }

        std::this_thread::yield();
    }
}



void Heartbeater::UpdateSequenceNumber(size_t value)
{
	if (mLastSequenceNumber)
	{
		(*mLastSequenceNumber->Lock()) = value;
	}
	else
	{
		mLastSequenceNumber = value;
	}
}
