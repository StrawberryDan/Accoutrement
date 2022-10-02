#pragma once



#include <variant>



template<typename D, typename E>
class Result
{
public:
    static Result   Ok(const D& data) { return Result(true,  data); }
    static Result   Ok(D&& data)      { return Result(true,  data); }
    static Result Err(const E& error) { return Result(false, error); }
    static Result Err(E&& error)      { return Result(false, error); }

    inline bool  IsOk() const { return  mIsOk; }
    inline bool IsErr() const { return !mIsOk; }

          D& operator*()        { assert(IsOk()); return  std::get<D>(mPayload); }
    const D& operator*()  const { assert(IsOk()); return  std::get<D>(mPayload); }
          D* operator->()       { assert(IsOk()); return &std::get<D>(mPayload); }
    const D* operator->() const { assert(IsOk()); return &std::get<D>(mPayload); }

    D        Unwrap() { assert(IsOk());  return std::move(std::get<D>(mPayload)); }
    const E&    Err() { assert(IsErr()); return std::get<E>(mPayload); }

    inline operator bool() { return IsOk(); }

private:
    using Payload = std::variant<D, E>;

    Result(bool isOk, Payload&& payload) : mIsOk(isOk), mPayload(payload) {}

    bool mIsOk;
    Payload mPayload;
};
