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

    size_t Read(void* data, size_t len) override;
    void Write(const void* data, size_t len) override;


private:
    SOCKET_HANDLE mSocket;
};
