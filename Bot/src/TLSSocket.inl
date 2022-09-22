#include "TLSSocket.hpp"



template<typename T>
T TLSSocket::Read() requires ( std::is_trivial_v<T> )
{
    T data;
    Read(&data, sizeof(T));
    return data;
}



template<typename T>
void TLSSocket::Read(T* data, size_t count) requires ( std::is_trivial_v<T> )
{
    Read(reinterpret_cast<void*>(data), count * sizeof(T));
}



template<typename T>
std::vector<T> TLSSocket::Read(size_t count) requires ( std::is_trivial_v<T> )
{
    std::vector<T> data;
    data.resize(count);
    Read(data.data(), count);
    return data;
}



template<typename T>
void TLSSocket::Write(const T& data) requires ( std::is_trivial_v<T> )
{
    Write(reinterpret_cast<void*>(&data), sizeof(T));
}



template<typename T>
void TLSSocket::Write(const T* data, size_t count) requires ( std::is_trivial_v<T> )
{
    Write(reinterpret_cast<const void*>(data), count * sizeof(T));
}



template<typename T>
void TLSSocket::Write(const std::vector<T>& data) requires ( std::is_trivial_v<T> )
{
    Write(reinterpret_cast<void*>(data), data.size() * sizeof(T));
}