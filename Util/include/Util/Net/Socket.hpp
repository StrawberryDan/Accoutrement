#pragma once



#include <cstdio>
#include <vector>
#include <cstdint>



class Socket
{
public:
    virtual size_t Read(void* data, size_t len) = 0;

    virtual void Write(const void* data, size_t len) = 0;

    virtual std::vector<uint8_t> ReadVector(size_t len) final;

    template<typename T>
    T ReadType() requires ( std::is_trivial_v<T> );

    template<typename T>
    void ReadArray(T* data, size_t count) requires ( std::is_trivial_v<T> );

    template<typename T>
    std::vector<T> ReadVector(size_t count) requires ( std::is_trivial_v<T> );

    virtual void WriteVector(const std::vector<uint8_t>& data) final;

    template<typename T>
    void WriteType(const T& data) requires ( std::is_trivial_v<T> );

    template<typename T>
    void WriteArray(const T* data, size_t count) requires ( std::is_trivial_v<T> );

    template<typename T>
    void WriteVector(const std::vector<T>& data) requires ( std::is_trivial_v<T> );
};



#include "Socket.inl"