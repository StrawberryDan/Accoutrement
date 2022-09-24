#pragma once



#include "Util/Net/TLSClient.hpp"
#include "Util/Net/UDPClient.hpp"



class Bot
{
public:
    Bot();

private:
    TLSClient mSocket;
    UDPClient mUDPSocket;
};