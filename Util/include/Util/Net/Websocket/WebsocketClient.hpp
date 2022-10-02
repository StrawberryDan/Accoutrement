#pragma once



#include <cstdint>
#include <string>
#include <optional>



#include "Util/Net/Socket/Socket.hpp"
#include "Util/Net/Socket/TCPClient.hpp"
#include "Util/Net/Socket/TLSClient.hpp"
#include "libressl/openssl/sha.h"



template<SocketImpl S, uint16_t PORT>
class WebsocketClientImpl
{
public:
    WebsocketClientImpl(const std::string& hostname, const std::string& subresource);

    [[nodiscard]] inline bool IsValid() const { return mSocket.has_value(); }

    inline S TakeSocket() { return Take(mSocket); }

private:
    [[nodiscard]] std::string GenerateNonce() const;


private:
    std::optional<S> mSocket;
};



using WSClient  = WebsocketClientImpl<TCPClient,  80>;
using WSSClient = WebsocketClientImpl<TLSClient, 443>;



#include "WebsocketClient.inl"