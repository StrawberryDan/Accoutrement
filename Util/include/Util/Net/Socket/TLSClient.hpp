#pragma once



#include "tls.h"
#include <string>
#include <memory>
#include "Socket.hpp"
#include "TCPClient.hpp"



class TLSClient : public Socket
{
public:
    TLSClient(const std::string& host, uint16_t port);
    TLSClient(const TLSClient&) = delete;
    TLSClient& operator=(const TLSClient&) = delete;
    TLSClient(TLSClient&& other) noexcept ;
    TLSClient& operator=(TLSClient&& other) noexcept ;
    ~TLSClient();

    Result<size_t, Error> Read(uint8_t* data, size_t len) const override;

    Result<size_t, Error> Write(const uint8_t* data, size_t len) const override;


private:
    tls*                       mTLS;
    tls_config*                mConfig;
};