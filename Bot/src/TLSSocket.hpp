#pragma once



#include "tls.h"
#include <string>
#include <cstdint>
#include <vector>
#include <cassert>



class TLSSocket
{
public:
    TLSSocket(const std::string& host, uint16_t port);
    TLSSocket(const TLSSocket&) = delete;
    TLSSocket& operator=(const TLSSocket&) = delete;
    TLSSocket(TLSSocket&& other) noexcept ;
    TLSSocket& operator=(TLSSocket&& other) noexcept ;
    ~TLSSocket();

    size_t Read(void* data, size_t len);

    std::vector<uint8_t> Read(size_t len);

    template<typename T>
    T Read() requires ( std::is_trivial_v<T> );

    template<typename T>
    void Read(T* data, size_t count) requires ( std::is_trivial_v<T> );

    template<typename T>
    std::vector<T> Read(size_t count) requires ( std::is_trivial_v<T> );



    void Write(const void* data, size_t len);

    void Write(const std::vector<uint8_t>& data);

    template<typename T>
    void Write(const T& data) requires ( std::is_trivial_v<T> );

    template<typename T>
    void Write(const T* data, size_t count) requires ( std::is_trivial_v<T> );

    template<typename T>
    void Write(const std::vector<T>& data) requires ( std::is_trivial_v<T> );



private:
    tls* mTLS;
    tls_config* mConfig;
};



#include "TLSSocket.inl"