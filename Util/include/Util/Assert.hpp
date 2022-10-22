#pragma once



#include <cstddef>
#include <iostream>
#include <utility>



inline constexpr void Assert(bool value)
{
#if !NDEBUG
    if (!value)
    {
        std::abort();
    }
    else
    {
        return;
    }
#endif // !NDEBUG
}