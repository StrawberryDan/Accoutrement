#pragma once



#include "../../src/TLSSocket.hpp"



class Bot
{
public:
    Bot();

private:
    TLSSocket mSocket;
};