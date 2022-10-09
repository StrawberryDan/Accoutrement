#include "Util/Net/Socket/TLSClient.hpp"



#include <cassert>
#include <iostream>
#include "Util/Utilities.hpp"



TLSClient::TLSClient(const std::string& host, uint16_t port)
    : mTLS(nullptr)
    , mConfig(nullptr)
    , mTCP(std::make_unique<TCPClient>(host, port))
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
    // result = tls_connect_cbs(mTLS, RecvData, SendData, mTCP.get(), host.c_str());
    assert(result >= 0);
    result = tls_handshake(mTLS);
    assert(result >= 0);
}



TLSClient::TLSClient(TLSClient&& other) noexcept
    : mTLS(Take(other.mTLS))
    , mConfig(Take(other.mConfig))
    , mTCP(Take(other.mTCP))
{

}



TLSClient& TLSClient::operator=(TLSClient&& rhs) noexcept
{
    if (this != &rhs)
    {
        mTLS = Take(rhs.mTLS);
        mConfig = Take(rhs.mConfig);
        mTCP = Take(rhs.mTCP);
    }

    return (*this);
}



TLSClient::~TLSClient()
{
    if (mConfig) tls_config_free(mConfig);
    if (mTLS)
    {
        tls_close(mTLS);
    }
    mTCP.reset();
    if (mTLS)
    {
        tls_free(mTLS);
    }
}



Result<size_t, Socket::Error> TLSClient::Read(uint8_t* data, size_t len) const
{
    auto bytesRead = tls_read(mTLS, data, len);
    if (bytesRead < 0)
    {
        std::cout << tls_error(mTLS) << std::endl;
    }
    assert(bytesRead >= 0);
    while(bytesRead < len)
    {
        bytesRead += tls_read(mTLS, data + bytesRead, len - bytesRead);
    }
    return Result<size_t, Socket::Error>::Ok(bytesRead);
}



Result<size_t, Socket::Error> TLSClient::Write(const uint8_t* data, size_t len) const
{
    auto bytesWritten = tls_write(mTLS, reinterpret_cast<const void*>(data), len);
    if (bytesWritten < 0)
    {
        std::cout << tls_error(mTLS) << std::endl;
    }
    assert(bytesWritten >= 0);

    while (bytesWritten < len)
    {
        bytesWritten += tls_write(mTLS, reinterpret_cast<const uint8_t*>(data) + bytesWritten, len - bytesWritten);
    }
    return Result<size_t, Socket::Error>::Ok(bytesWritten);
}
