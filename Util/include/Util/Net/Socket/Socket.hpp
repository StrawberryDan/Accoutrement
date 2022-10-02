#pragma once



#include <cstdio>
#include <vector>
#include <cstdint>
#include <string>



class Socket
{
public:
    virtual size_t Read(uint8_t * data, size_t len) const = 0;

    virtual void Write(const uint8_t* data, size_t len) const = 0;

    template<typename T>
    T ReadType() const requires ( std::is_fundamental_v<T> );

    template<typename T>
    void ReadArray(T* data, size_t count) const requires ( std::is_fundamental_v<T> );

    template<typename T>
    std::vector<T> ReadVector(size_t count) const requires ( std::is_fundamental_v<T> );

    template<typename T>
    void WriteType(const T& data) const requires ( std::is_fundamental_v<T> );

    template<typename T>
    void WriteArray(const T* data, size_t count) const requires ( std::is_fundamental_v<T> );

    template<typename T>
    void WriteVector(const std::vector<T>& data) const requires ( std::is_fundamental_v<T> );
};



template<typename T>
concept SocketImpl = requires(T t, std::string hostname, uint16_t port)
{
    std::is_base_of_v<Socket, T>;
};



#include "Socket.inl"