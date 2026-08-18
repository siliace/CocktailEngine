#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_LOCKABLE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_LOCKABLE_HPP

#include <concepts>

namespace Ck
{
    template <typename T>
    concept Lockable = requires(T& v)
    {
        { v.Lock() } -> std::same_as<void>;
        { v.TryLock() } -> std::same_as<bool>;
        { v.Unlock() } -> std::same_as<void>;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_LOCKABLE_HPP
