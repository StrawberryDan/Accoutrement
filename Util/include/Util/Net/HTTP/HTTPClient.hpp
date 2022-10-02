#pragma once



#include "Util/Utilities.hpp"
#include "Util/Net/Socket/Socket.hpp"
#include "HTTPResponse.hpp"
#include "HTTPRequest.hpp"
#include "Util/Net/Socket/TCPClient.hpp"
#include "Util/Net/Socket/TLSClient.hpp"



template<SocketImpl S, uint16_t PORT>
class HTTPClientImpl
{
public:
    HTTPClientImpl(const std::string& hostname);

    inline S TakeSocket() { return Take(mSocket); }

    void Request(const HTTPRequest& request);
    HTTPResponse Receive();

private:
    std::string ReadLine();
    HTTPChunkedPayload ReadChunkedPayload();

private:
    S mSocket;
};



using HTTPClient  = HTTPClientImpl<TCPClient, 80>;
using HTTPSClient = HTTPClientImpl<TLSClient, 443>;



#include "HTTPClient.inl"