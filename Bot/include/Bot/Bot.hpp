#pragma once



#include "Util/Net/TLSSocket.hpp"



class Bot
{
public:
    Bot();

private:
    TLSSocket mSocket;
};