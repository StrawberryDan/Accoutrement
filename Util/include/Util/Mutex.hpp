#pragma once



#include <mutex>
#include <cassert>



template<typename T>
class Mutex;



template <typename T, bool IsConst>
class MutexGuard
{
public:
    using LockType    = std::unique_lock<std::mutex>;
    using PayloadType = std::conditional_t<IsConst, const T, T>;

    MutexGuard(const MutexGuard& other) = delete;
    MutexGuard(MutexGuard&& other) = default;
    MutexGuard& operator=(const MutexGuard&) = delete;
    MutexGuard& operator=(MutexGuard&& other)  noexcept = default;

    inline PayloadType& operator*()  const { return *mPayload; }
    inline PayloadType* operator->() const { return  mPayload; }
private:
    MutexGuard(LockType lock, PayloadType* ptr) : mLock(std::move(lock)), mPayload(ptr) {}

    LockType     mLock;
    PayloadType* mPayload;

    friend class Mutex<T>;
};



template<typename T>
class Mutex
{
public:
    Mutex() requires ( std::is_default_constructible_v<T> ) = default;
    ~Mutex() { assert(mMutex.try_lock()); }

    explicit Mutex(T&& payload) : mPayload(std::forward<T>(payload)) {}

    template<typename ... Ts>
    Mutex(Ts ... ts) : mPayload(std::forward<Ts>(ts)...) {}

    Mutex Copy(const Mutex& other) const requires ( std::is_copy_constructible_v<T> )
    {
        auto lock = other.Lock();
        return Mutex(*lock);
    }

    Mutex& operator=(const Mutex& other) requires ( std::is_copy_assignable_v<T> )
    {
        auto lock = other.Lock();
        mPayload = *lock;
        return (*this);
    }

    Mutex& operator=(Mutex&& other) requires ( std::is_move_assignable_v<T> )
    {
        auto lock = other.Lock();
        mPayload = std::move(*lock);
        return (*this);
    }

    MutexGuard<T, false> Lock()       { return {typename MutexGuard<T, false>::LockType(mMutex), &mPayload}; }
    MutexGuard<T,  true> Lock() const { return {typename MutexGuard<T,  true>::LockType(mMutex), &mPayload}; }

private:
    std::mutex mMutex;
    T          mPayload;
};
