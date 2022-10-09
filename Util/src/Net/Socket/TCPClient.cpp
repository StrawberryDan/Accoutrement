#include "Util/Net/Socket/TCPClient.hpp"



#include "Util/Utilities.hpp"
#include <cassert>
#include <iostream>



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
    WSAData wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
    mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(mSocket != INVALID_SOCKET);

    std::string portAsString = std::to_string(port);
    addrinfo* addressInfo;
    auto result = getaddrinfo(hostname.c_str(), portAsString.c_str(), nullptr, &addressInfo);
    assert(result == 0);

    result = connect(mSocket, addressInfo->ai_addr, static_cast<int>(addressInfo->ai_addrlen));
    assert(result == 0);

    unsigned long iMode = 0;
    result = ioctlsocket(mSocket, FIONBIO, &iMode);
    assert(result == 0);

    int timeout = 1000;
    result = setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&timeout), sizeof(timeout));
    assert(result >= 0);
#endif // _WIN32
}



TCPClient::TCPClient(TCPClient&& other) noexcept
    : mSocket(Take(other.mSocket))
{

}



TCPClient& TCPClient::operator=(TCPClient&& other) noexcept
{
    mSocket = Take(other.mSocket);
    return (*this);
}



TCPClient::~TCPClient()
{
#if _WIN32
    if (mSocket)
    {
        shutdown(mSocket, SD_BOTH);
        closesocket(mSocket);
    }
#endif // _WIN32
}



Result<size_t, Socket::Error> TCPClient::Read(uint8_t* data, size_t len) const
{
#if _WIN32
    auto bytesRead = recv(mSocket, reinterpret_cast<char*>(data), static_cast<int>(len), 0);
    if (bytesRead == SOCKET_ERROR)
    {

        int wsaError = WSAGetLastError();
        return Result<size_t, Socket::Error>::Err(Socket::ErrorFromWSACode(wsaError));
    }
    else
    {
        return Result<size_t, Socket::Error>::Ok(bytesRead);
    }
#endif // _WIN32
}



Result<size_t, Socket::Error> TCPClient::Write(const uint8_t* data, size_t len) const
{
#if _WIN32
    auto bytesSent = send(mSocket, reinterpret_cast<const char*>(data), static_cast<int>(len), 0);
    if (bytesSent == SOCKET_ERROR)
    {
        int wsaError = WSAGetLastError();
        return Result<size_t, Socket::Error>::Err(Socket::ErrorFromWSACode(wsaError));
    }
    else
    {
        return Result<size_t, Socket::Error>::Ok(bytesSent);
    }
#endif // _WIN32
}
