#pragma once



#include <bit>
#include <type_traits>



template<typename T>
T ToBigEndian(T v) requires( std::is_integral_v<T> )
{
    if constexpr (std::endian::native == std::endian::big)
    {
        return v;
    }
    else if (std::endian::native == std::endian::little)
    {
        return std::byteswap(v);
    }
}



template<typename T>
T ToLittleEndian(T v) requires( std::is_integral_v<T> )
{
    if constexpr (std::endian::native == std::endian::little)
    {
        return v;
    }
    else if (std::endian::native == std::endian::big)
    {
        return std::byteswap(v);
    }
}



template<typename T>
T FromBigEndian(T v) requires( std::is_integral_v<T> )
{
    if constexpr (std::endian::native == std::endian::big)
    {
        return v;
    }
    else if (std::endian::native == std::endian::little)
    {
        return std::byteswap(v);
    }
}



template<typename T>
T FromLittleEndian(T v) requires( std::is_integral_v<T> )
{
    if constexpr (std::endian::native == std::endian::little)
    {
        return v;
    }
    else if (std::endian::native == std::endian::big)
    {
        return std::byteswap(v);
    }
}