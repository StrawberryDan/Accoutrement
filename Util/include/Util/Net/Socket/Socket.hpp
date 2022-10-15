#pragma once



#include <cstdio>
#include <vector>
#include <cstdint>
#include <string>
#include <utility>



#include "Util/Result.hpp"



class Socket
{
public:
    enum class Error;

#if WIN32
    static Error ErrorFromWSACode(int error);
#endif // WIN32

public:
    [[nodiscard]] virtual Result<size_t, Error> Read(uint8_t * data, size_t len) const = 0;

    [[nodiscard]] virtual Result<size_t, Error> Write(const uint8_t* data, size_t len) const = 0;

    template<typename T>
    [[nodiscard]] Result<T, Error> ReadType() const requires ( std::is_fundamental_v<T> );

    template<typename T>
    [[nodiscard]] Result<size_t, Error> ReadArray(T* data, size_t count) const requires ( std::is_fundamental_v<T> );

    template<typename T>
    [[nodiscard]] Result<std::vector<T>, Error> ReadVector(size_t count) const requires ( std::is_fundamental_v<T> );

    template<typename T>
    [[nodiscard]] Result<size_t, Error> WriteType(const T& data) const requires ( std::is_fundamental_v<T> );

    template<typename T>
    [[nodiscard]] Result<size_t, Error> WriteArray(const T* data, size_t count) const requires ( std::is_fundamental_v<T> );

    template<typename T>
    [[nodiscard]] Result<size_t, Error> WriteVector(const std::vector<T>& data) const requires ( std::is_fundamental_v<T> );
};



template<typename T>
concept SocketImpl = requires(T t, std::string hostname, uint16_t port)
{
    std::is_base_of_v<Socket, T>;
};



enum class Socket::Error
{
    Closed,
};



#include "Socket.inl"