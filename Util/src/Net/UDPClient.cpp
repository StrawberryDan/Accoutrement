#include "Util/Net/UDPClient.hpp"



#include "Util/Utilities.hpp"
#include <cassert>



#if _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#error "NO SOCKET IMPLEMENTATION FOR PLATFORM"
#endif // _WIN32



UDPClient::UDPClient(const std::string& hostname, uint16_t port)
    : mSocket{}
{
#if _WIN32
    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    assert(mSocket != INVALID_SOCKET);

    std::string portAsString = std::to_string(port);
    addrinfo* addressInfo;
    auto result = getaddrinfo(hostname.c_str(), portAsString.c_str(), nullptr, &addressInfo);
    assert(result == 0);

    result = connect(mSocket, addressInfo->ai_addr, static_cast<int>(addressInfo->ai_addrlen));
    assert(result == 0);
#endif // _WIN32
}



UDPClient::UDPClient(UDPClient&& other)
    : mSocket(Take(other.mSocket))
{

}



UDPClient& UDPClient::operator=(UDPClient&& other)
{
    mSocket = Take(other.mSocket);
    return (*this);
}



UDPClient::~UDPClient()
{
#if _WIN32
    closesocket(mSocket);
#endif // _WIN32
}



size_t UDPClient::Read(uint8_t* data, size_t len)
{
#if _WIN32
    return recv(mSocket, reinterpret_cast<char*>(data), len, 0);
#endif // _WIN32
}



void UDPClient::Write(const uint8_t* data, size_t len)
{
#if _WIN32
    auto bytesSent = send(mSocket, reinterpret_cast<const char*>(data), len, 0);
    assert(bytesSent == len);
#endif // _WIN32
}
