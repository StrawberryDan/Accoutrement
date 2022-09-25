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
    TCPClient(TCPClient&& other);
    TCPClient& operator=(TCPClient&& other);
    ~TCPClient();

    size_t Read(uint8_t* data, size_t len) override;

    void Write(const uint8_t* data, size_t len) override;



private:
    SOCKET_HANDLE mSocket;
};
