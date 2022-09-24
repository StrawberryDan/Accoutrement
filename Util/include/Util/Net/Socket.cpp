#include "Socket.hpp"



std::vector<uint8_t> Socket::ReadVector(size_t len)
{
    std::vector<uint8_t> data;
    data.resize(len);
    auto bytesRead = Read(reinterpret_cast<void*>(data.data()), len);
    data.resize(bytesRead);
    return data;
}



void Socket::WriteVector(const std::vector<uint8_t>& data)
{
    Write(reinterpret_cast<const void*>(data.data()), data.size());
}