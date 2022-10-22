#include "Util/Net/Socket/UDPClient.hpp"



#include "Util/Utilities.hpp"
#include "Util/Assert.hpp"
#include "Util/Macros.hpp"


#if _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#elif __APPLE__ || __linux__
#include <sys/socket.h>
#include <netdb.h>
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
    Assert(mSocket != INVALID_SOCKET);

    std::string portAsString = std::to_string(port);
    addrinfo* addressInfo;
    auto result = getaddrinfo(hostname.c_str(), portAsString.c_str(), nullptr, &addressInfo);
    Assert(result == 0);

    result = connect(mSocket, addressInfo->ai_addr, static_cast<int>(addressInfo->ai_addrlen));
    Assert(result == 0);
#elif __APPLE__ || __linux__
	mSocket = socket(AF_INET, SOCK_DGRAM, 0);
	Assert(mSocket != -1);
	auto host = gethostbyname(hostname.c_str());
	Assert(host != nullptr);
	sockaddr_in address{};
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	memcpy(&address.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
	auto result = connect(mSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address));
	Assert(result == 0);
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
#elif __APPLE__ || __linux__
	if (mSocket)
	{
		auto err = shutdown(mSocket, SHUT_RDWR);
		Assert(err == 0);
	}
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
        return Result<size_t, Socket::Error>::Ok(static_cast<size_t>(bytesRead));
    }
#elif __APPLE__ || __linux__
	auto bytesRead = recv(mSocket, data, len, MSG_WAITALL);
	if (bytesRead >= 0)
	{
		return bytesRead;
	}
	else
	{
		switch (bytesRead)
		{
			default:
				UNREACHABLE;
		}
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
        return Result<size_t, Socket::Error>::Ok(static_cast<size_t>(bytesSent));
    }
#elif __APPLE__ || __linux__
	auto bytesSent = send(mSocket, data, len, 0);
	if (bytesSent >= 0)
	{
		return bytesSent;
	}
	else
	{
		switch (bytesSent)
		{
			default:
				UNREACHABLE;
		}
	}
#endif // _WIN32
}
