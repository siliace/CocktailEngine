#ifndef COCKTAIL_CORE_SIGNAL_SIGNAL_HPP
#define COCKTAIL_CORE_SIGNAL_SIGNAL_HPP

#include <mutex>

#include <Cocktail/Core/Signal/Detail/SignalBase.hpp>
#include <Cocktail/Core/System/Concurrency/NullMutex.hpp>
#include <Cocktail/Core/System/Concurrency/SpinMutex.hpp>

namespace Ck
{
    template <typename Lockable, typename... Args>
    struct SignalOf
    {
        using Type = Detail::SignalBase<Lockable, Args...>;
    };

    template <typename Lockable, typename... Args>
    using SignalOfType = typename SignalOf<Lockable, Args...>::Type;
    
    template <typename... Args>
    using Signal = SignalOfType<std::recursive_mutex, Args...>;

    template <typename... Args>
    using SpinSignal = SignalOfType<SpinMutex, Args...>;

    template <typename... Args>
    using UnsafeSignal = SignalOfType<NullMutex, Args...>;
}

#endif // COCKTAIL_CORE_SIGNAL_SIGNAL_HPP
