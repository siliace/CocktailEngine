#ifndef COCKTAILENGINE_CORE_SIGNAL_SIGNAL_HPP
#define COCKTAILENGINE_CORE_SIGNAL_SIGNAL_HPP

#include <mutex>

#include <CocktailEngine/Core/Signal/Detail/SignalBase.hpp>
#include <CocktailEngine/Core/System/Concurrency/NullMutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/SpinMutex.hpp>

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

#endif // COCKTAILENGINE_CORE_SIGNAL_SIGNAL_HPP
