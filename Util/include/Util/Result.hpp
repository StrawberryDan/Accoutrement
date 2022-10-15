#pragma once



#include <variant>
#include "Util/Assert.hpp"



template<typename D, typename E>
class Result
{
public:
    template <typename ...Args>
    static Result   Ok(Args ...args)  { return Result(true,  D{std::forward<Args>(args)...}); }

    template <typename ...Args>
    static Result  Err(Args ...args)  { return Result(false, E{std::forward<Args>(args)...}); }

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

private:
    using Payload = std::variant<D, E>;

    Result(bool isOk, Payload&& payload) : mIsOk(isOk), mPayload(payload) {}

    bool mIsOk;
    Payload mPayload;
};
