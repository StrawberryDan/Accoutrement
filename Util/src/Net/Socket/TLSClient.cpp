#include "Util/Net/Socket/TLSClient.hpp"



#include "Util/Assert.hpp"
#include <iostream>
#include "Util/Utilities.hpp"



TLSClient::TLSClient(const std::string& host, uint16_t port)
    : mTLS(nullptr)
    , mConfig(nullptr)
{
    auto result = tls_init();
    Assert(result >= 0);
    mTLS = tls_client();
    Assert(mTLS != nullptr);
    mConfig = tls_config_new();
    Assert(mConfig != nullptr);
    result = tls_config_set_protocols(mConfig, TLS_PROTOCOL_TLSv1_2);
    Assert(result >= 0);
    result = tls_config_set_ca_file(mConfig, "./CA.pem");
    Assert(result >= 0);
    result = tls_configure(mTLS, mConfig);
    Assert(result >= 0);
    auto portAsString = std::to_string(port);
    result = tls_connect(mTLS, host.c_str(), portAsString.c_str());
    // result = tls_connect_cbs(mTLS, RecvData, SendData, mTCP.get(), host.c_str());
    Assert(result >= 0);
    result = tls_handshake(mTLS);
    Assert(result >= 0);
}



TLSClient::TLSClient(TLSClient&& other) noexcept
    : mTLS(Take(other.mTLS))
    , mConfig(Take(other.mConfig))
{

}



TLSClient& TLSClient::operator=(TLSClient&& rhs) noexcept
{
    if (this != &rhs)
    {
        mTLS = Take(rhs.mTLS);
        mConfig = Take(rhs.mConfig);
    }

    return (*this);
}



TLSClient::~TLSClient()
{
    if (mConfig) tls_config_free(mConfig);
    if (mTLS)
    {
        tls_close(mTLS);
        tls_free(mTLS);
    }
}



Result<size_t, Socket::Error> TLSClient::Read(uint8_t* data, size_t len) const
{
    size_t bytesRead = 0;
    do
    {
        auto error = tls_read(mTLS, reinterpret_cast<void*>(data + bytesRead), len - bytesRead);
        if (error < 0)
        {
#if !NDEBUG
            std::cout << tls_error(mTLS) << std::endl;
            return Result<size_t, Socket::Error>::Err(Socket::Error::Unknown);
#endif // !NDEBUG
        }
        else
        {
            bytesRead += error;
        }
    }
    while (bytesRead < len);
    return Result<size_t, Socket::Error>::Ok(static_cast<size_t>(bytesRead));
}



Result<size_t, Socket::Error> TLSClient::Write(const uint8_t* data, size_t len) const
{
    size_t bytesWritten = 0;
    do
    {
        auto error = tls_write(mTLS, reinterpret_cast<const void*>(data + bytesWritten), len - bytesWritten);
        if (error < 0)
        {
#if !NDEBUG
            std::cout << tls_error(mTLS) << std::endl;
            return Result<size_t, Socket::Error>::Err(Socket::Error::Unknown);
#endif // !NDEBUG
        }
        else
        {
            bytesWritten += error;
        }
    }
    while (bytesWritten < len);

    return Result<size_t, Socket::Error>::Ok(static_cast<size_t>(bytesWritten));
}
