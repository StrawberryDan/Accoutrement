#pragma once



#include "Socket.hpp"
#include <string>

#if _WIN32
using SOCKET_HANDLE = uintptr_t;
#else
#error "NO SOCKET IMPLEMENTATION FOR PLATFORM"
#endif // _WIN32



class UDPClient : public Socket
{
public:
    UDPClient(const std::string& hostname, uint16_t port);
    UDPClient(const UDPClient&) = delete;
    UDPClient& operator=(const UDPClient&) = delete;
    UDPClient(UDPClient&& other);
    UDPClient& operator=(UDPClient&& other);
    ~UDPClient();

    size_t Read(uint8_t* data, size_t len) const override;
    void Write(const uint8_t* data, size_t len) const override;


private:
    SOCKET_HANDLE mSocket;
};
