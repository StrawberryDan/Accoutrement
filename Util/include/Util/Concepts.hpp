#pragma once



#include <concepts>



template <typename R, typename F, typename ...Args>
concept Callable = requires
{
    requires std::invocable<F, Args...>;
    requires std::is_same_v<R, std::invoke_result_t<F, Args...>>;
};
