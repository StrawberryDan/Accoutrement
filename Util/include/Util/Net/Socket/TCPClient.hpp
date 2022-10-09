#pragma once



#include <string>
#include "Socket.hpp"



#if _WIN32
using SOCKET_HANDLE = uintptr_t;
#else
#error "NO SOCKET IMPLEMENTATION FOR PLATFORM"
#endif // _WIN32



class TCPClient : public Socket
{
public:
    TCPClient(const std::string& hostname, uint16_t port);
    TCPClient(const TCPClient&) = delete;
    TCPClient& operator=(const TCPClient&) = delete;
    TCPClient(TCPClient&& other) noexcept ;
    TCPClient& operator=(TCPClient&& other) noexcept ;
    ~TCPClient();

    Result<size_t, Error> Read(uint8_t* data, size_t len) const override;

    Result<size_t, Error> Write(const uint8_t* data, size_t len) const override;



private:
    SOCKET_HANDLE mSocket;
};
