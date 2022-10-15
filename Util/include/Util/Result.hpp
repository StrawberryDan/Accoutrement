#pragma once



#include <variant>
#include "Concepts.hpp"
#include "Util/Assert.hpp"



template<typename D, typename E>
class Result
{
public:
    static Result Ok(const D& value) requires ( std::is_copy_constructible_v<D> )
        { return Result(true, value); }

    static Result Ok(D&& value) requires ( std::is_move_constructible_v<D> )
        { return Result(true, value); }

    template <typename ...Args>
    static Result   Ok(Args ...args) requires ( std::is_constructible_v<D, Args...> )
        { return Result(true,  D(std::forward<Args>(args)...)); }



    static Result Err(const E& value) requires ( std::is_copy_constructible_v<D> )
        { return Result(false, value); }

    static Result Err(E&& value) requires ( std::is_move_constructible_v<D> )
        { return Result(false, value); }

    template <typename ...Args>
    static Result  Err(Args ...args) requires ( std::is_constructible_v<D, Args...> )
        { return Result(false, E(std::forward<Args>(args)...)); }



    inline bool  IsOk() const { return  mIsOk; }
    inline bool IsErr() const { return !mIsOk; }



          D& operator *()        { Assert(IsOk()); return  std::get<D>(mPayload); }
    const D& operator *()  const { Assert(IsOk()); return  std::get<D>(mPayload); }
          D* operator->()       { Assert(IsOk()); return &std::get<D>(mPayload); }
    const D* operator->() const { Assert(IsOk()); return &std::get<D>(mPayload); }



    D        Unwrap() { Assert(IsOk());  return std::move(std::get<D>(mPayload)); }
    D        UnwrapOr(D value) { if (IsOk()) { return Unwrap(); } else { return value; }}
    const E&    Err() { Assert(IsErr()); return std::move(std::get<E>(mPayload)); }



    explicit inline operator bool() { return IsOk(); }



    template<typename U, typename F> requires Callable<U, F, D&&>
    Result<U, E> Map(F f)
    {
        if (*this)
        {
            return Ok(f(std::move(Unwrap())));
        }
        else
        {
            return Err(std::move(Err()));
        }
    }

private:
    using Payload = std::variant<D, E>;

    Result(bool isOk, Payload&& payload) : mIsOk(isOk), mPayload(payload) {}

    bool mIsOk;
    Payload mPayload;
};
