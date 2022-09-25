#pragma once



#include "Util/Net/Socket.hpp"
#include "HTTPResponse.hpp"
#include "HTTPRequest.hpp"
#include "Util/Net/TCPClient.hpp"
#include "Util/Net/TLSClient.hpp"



template<SocketImpl S, uint16_t PORT>
class HTTPClientImpl
{
public:
    HTTPClientImpl(const std::string& hostname);


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