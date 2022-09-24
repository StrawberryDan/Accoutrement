#pragma once



#include "tls.h"
#include <string>
#include "Socket.hpp"



class TLSClient : public Socket
{
public:
    TLSClient(const std::string& host, uint16_t port);
    TLSClient(const TLSClient&) = delete;
    TLSClient& operator=(const TLSClient&) = delete;
    TLSClient(TLSClient&& other) noexcept ;
    TLSClient& operator=(TLSClient&& other) noexcept ;
    ~TLSClient();

    size_t Read(void* data, size_t len) override;

    void Write(const void* data, size_t len) override;


private:
    tls* mTLS;
    tls_config* mConfig;
};