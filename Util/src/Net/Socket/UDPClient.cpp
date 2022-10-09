#include "Util/Net/Socket/UDPClient.hpp"



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
    WSAData wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
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



UDPClient::UDPClient(UDPClient&& other) noexcept
    : mSocket(Take(other.mSocket))
{

}



UDPClient& UDPClient::operator=(UDPClient&& other) noexcept
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



Result<size_t, Socket::Error> UDPClient::Read(uint8_t* data, size_t len) const
{
#if _WIN32
    auto bytesRead = recv(mSocket, reinterpret_cast<char*>(data), len, 0);
    if (bytesRead == SOCKET_ERROR)
    {
        return Result<size_t, Socket::Error>::Err(Socket::ErrorFromWSACode(WSAGetLastError()));
    }
    else
    {
        return Result<size_t, Socket::Error>::Ok(bytesRead);
    }
#endif // _WIN32
}



Result<size_t, Socket::Error> UDPClient::Write(const uint8_t* data, size_t len) const
{
#if _WIN32
    auto bytesSent = send(mSocket, reinterpret_cast<const char*>(data), len, 0);
    if (bytesSent == SOCKET_ERROR)
    {
        return Result<size_t, Socket::Error>::Err(Socket::ErrorFromWSACode(WSAGetLastError()));
    }
    else
    {
        return Result<size_t, Socket::Error>::Ok(bytesSent);
    }
#endif // _WIN32
}
