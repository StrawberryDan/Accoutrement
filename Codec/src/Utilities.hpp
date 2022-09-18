#pragma once



#include <type_traits>



template<typename T>
    requires ( !std::is_pointer_v<T> && std::is_move_assignable_v<T> )
inline constexpr T Take(T& origin)
{
    T result = std::move(origin);
    return result;
}



template<typename T>
inline constexpr T* Take(T*& origin)
{
    T* result = origin;
    origin = nullptr;
    return result;
}



template<typename T, typename R>
inline constexpr T Replace(T& origin, R replacement)
    requires ( !std::is_pointer_v<T> && std::is_move_assignable_v<T> && std::is_convertible_v<R, T> )
{
    T result = std::move(origin);
    origin = replacement;
    return result;
}