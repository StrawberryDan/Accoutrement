#include "Util/Net/Socket/TLSClient.hpp"



#include <cassert>
#include "Util/Utilities.hpp"



TLSClient::TLSClient(const std::string& host, uint16_t port)
    : mTLS(nullptr)
    , mConfig(nullptr)
{
    auto result = tls_init();
    assert(result >= 0);
    mTLS = tls_client();
    assert(mTLS != nullptr);
    mConfig = tls_config_new();
    assert(mConfig != nullptr);
    result = tls_config_set_protocols(mConfig, TLS_PROTOCOL_TLSv1_2);
    assert(result >= 0);
    result = tls_config_set_ca_file(mConfig, "./CA.pem");
    assert(result >= 0);
    result = tls_configure(mTLS, mConfig);
    assert(result >= 0);
    auto portAsString = std::to_string(port);
    result = tls_connect(mTLS, host.c_str(), portAsString.c_str());
    assert(result >= 0);
    result = tls_handshake(mTLS);
    assert(result >= 0);
}



TLSClient::TLSClient(TLSClient&& other) noexcept
    : mTLS(Take(other.mTLS))
    , mConfig(Take(other.mConfig))
{

}



TLSClient& TLSClient::operator=(TLSClient&& other) noexcept
{
    mTLS = Take(other.mTLS);
    mConfig = Take(other.mConfig);
    return (*this);
}



TLSClient::~TLSClient()
{
    tls_config_free(mConfig);
    tls_free(mTLS);
}



size_t TLSClient::Read(uint8_t* data, size_t len) const
{
    auto bytesRead =  tls_read(mTLS, data, len);
    assert(bytesRead == len);
    return bytesRead;
}



void TLSClient::Write(const uint8_t* data, size_t len) const
{
    auto bytesWritten = tls_write(mTLS, reinterpret_cast<const void*>(data), len);
    assert(bytesWritten == len);
}
