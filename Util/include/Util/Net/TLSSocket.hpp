#pragma once



#include "tls.h"
#include <string>
#include "Socket.hpp"



class TLSSocket : public Socket
{
public:
    TLSSocket(const std::string& host, uint16_t port);
    TLSSocket(const TLSSocket&) = delete;
    TLSSocket& operator=(const TLSSocket&) = delete;
    TLSSocket(TLSSocket&& other) noexcept ;
    TLSSocket& operator=(TLSSocket&& other) noexcept ;
    ~TLSSocket();

    size_t Read(void* data, size_t len) override;

    void Write(const void* data, size_t len) override;


private:
    tls* mTLS;
    tls_config* mConfig;
};