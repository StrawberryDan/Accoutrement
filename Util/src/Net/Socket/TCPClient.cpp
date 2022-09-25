#include "Util/Net/Socket/TCPClient.hpp"



#include "Util/Utilities.hpp"
#include <cassert>



#if _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#error "NO SOCKET IMPLEMENTATION FOR PLATFORM"
#endif // _WIN32



TCPClient::TCPClient(const std::string& hostname, uint16_t port)
    : mSocket{}
{
#if _WIN32
    mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(mSocket != INVALID_SOCKET);

    std::string portAsString = std::to_string(port);
    addrinfo* addressInfo;
    auto result = getaddrinfo(hostname.c_str(), portAsString.c_str(), nullptr, &addressInfo);
    assert(result == 0);

    result = connect(mSocket, addressInfo->ai_addr, static_cast<int>(addressInfo->ai_addrlen));
    assert(result == 0);
#endif // _WIN32
}



TCPClient::TCPClient(TCPClient&& other)
    : mSocket(Take(other.mSocket))
{

}



TCPClient& TCPClient::operator=(TCPClient&& other)
{
    mSocket = Take(other.mSocket);
    return (*this);
}



TCPClient::~TCPClient()
{
#if _WIN32
    closesocket(mSocket);
#endif // _WIN32
}



size_t TCPClient::Read(uint8_t* data, size_t len)
{
#if _WIN32
    return recv(mSocket, reinterpret_cast<char*>(data), static_cast<int>(len), 0);
#endif // _WIN32
}



void TCPClient::Write(const uint8_t* data, size_t len)
{
#if _WIN32
    auto bytesSent = send(mSocket, reinterpret_cast<const char*>(data), static_cast<int>(len), 0);
    assert(bytesSent == len);
#endif // _WIN32
}
