#pragma once



#include <cstdint>
#include "Util/Assert.hpp"
#include <utility>
#include <concepts>



template<typename T>
class Option
{
public:
    Option()
        : mHasValue(false)
        , mData{}
    {}

    Option(const Option& rhs) requires ( std::is_copy_constructible_v<T> )
    : mHasValue(rhs.mHasValue)
    , mData{}
    {
        new (mData) T(*rhs);
    }

    Option(Option&& rhs)  noexcept requires ( std::is_move_constructible_v<T> )
    : mHasValue(rhs.mHasValue)
    , mData{}
    {
        new (mData) T(std::move(*rhs));
        rhs.mHasValue = false;
    }

    template<typename ...Args>
    Option(Args ...args) requires ( std::is_constructible_v<T, Args...> )
        : mHasValue(true)
        , mData{}
    {
        new (mData) T(std::forward<Args>(args)...);
    }

    Option& operator=(const Option& rhs) requires ( std::is_copy_assignable_v<T> )
    {
        if (this != &rhs)
        {
            if (mHasValue)
            {
                (*this)->~T();
            }

            mHasValue = rhs.mHasValue;
            if (mHasValue)
            {
                new(mData) T(*rhs);
            }
        }

        return *this;
    }

    Option& operator=(Option&& rhs) noexcept requires ( std::is_move_assignable_v<T> )
    {
        if (this != &rhs)
        {
            if (mHasValue)
            {
                (*this)->~T();
            }

            mHasValue = rhs.mHasValue;
            if (mHasValue)
            {
                new (mData) T(std::move(*rhs));
                rhs.mHasValue;
            }
        }

        return *this;
    }

    ~Option()
    {
        if (mHasValue)
        {
            (*this)->~T();
        }
    }


    template<typename ...Args>
    void Emplace(Args ...args)
    {
        if (mHasValue)
        {
            (*this)->~T();
        }

        new (mData) T(std::forward<Args>(args)...);
        mHasValue = true;
    }

    void Reset()
    {
        if (mHasValue)
        {
            (*this)->~T();
            mHasValue = false;
        }
    }


    inline bool HasValue() const { return mHasValue; }
    explicit inline operator bool() const { return mHasValue; }


          T& operator *()       { Assert(mHasValue); return *reinterpret_cast<      T*>(mData); }
    const T& operator *() const { Assert(mHasValue); return *reinterpret_cast<const T*>(mData); }

          T* operator->()       { Assert(mHasValue); return  reinterpret_cast<      T*>(mData); }
    const T* operator->() const { Assert(mHasValue); return  reinterpret_cast<const T*>(mData); }


    bool operator==(const Option<T>& rhs) const requires ( std::equality_comparable<T> )
    {
        if (!HasValue() && !rhs.HasValue())
        {
            return true;
        } else if (HasValue() && rhs.HasValue())
        {
            return (**this) == (*rhs);
        }

        return false;
    }

    inline bool operator!=(const Option<T>& rhs) const requires ( std::equality_comparable<T> )
    {
        return !((*this) == rhs);
    }

private:
    bool    mHasValue;
    uint8_t mData[sizeof(T)];
};
