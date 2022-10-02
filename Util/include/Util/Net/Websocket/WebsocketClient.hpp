#pragma once



#include "Util/Net/Socket/Socket.hpp"
#include "Util/Net/Socket/TCPClient.hpp"
#include "Util/Net/Socket/TLSClient.hpp"
#include "libressl/openssl/sha.h"
#include <cstdint>
#include <string>



template<SocketImpl S, uint16_t PORT>
class WebsocketClientImpl
{
public:
    WebsocketClientImpl(const std::string& hostname);

    inline S TakeSocket() { return Take(mSocket); }

private:
    S mSocket;
};



using WSClient  = WebsocketClientImpl<TCPClient,  80>;
using WSSClient = WebsocketClientImpl<TLSClient, 443>;
