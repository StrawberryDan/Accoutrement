#pragma once



#include <cstdint>
#include <type_traits>
#include <vector>



class HTTPSimplePayload
{
public:
    void Read(uint8_t* data, size_t len);

    template<typename T>
    T Read() requires(std::is_fundamental_v<T>);

    void Write(const uint8_t* data, size_t len);

    template<typename T>
    void Write(const T& data) requires(std::is_fundamental_v<T>);

    std::vector<uint8_t>&       operator*()       { return mData; }
    const std::vector<uint8_t>& operator*() const { return mData; }

    const uint8_t* Data() const { return mData.data(); }
    size_t Size() const { return mData.size(); }


private:
    std::vector<uint8_t> mData;
};



template<typename T>
T HTTPSimplePayload::Read() requires(std::is_fundamental_v<T>)
{
    T data;
    Read(reinterpret_cast<uint8_t*>(&data), sizeof(T));
    return data;
}



template<typename T>
void HTTPSimplePayload::Write(const T& data) requires (std::is_fundamental_v<T>)
{
    Write(reinterpret_cast<const uint8_t*>(&data), sizeof(T));
}
