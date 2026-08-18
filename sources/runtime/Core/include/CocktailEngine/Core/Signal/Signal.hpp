#ifndef COCKTAILENGINE_CORE_SIGNAL_SIGNAL_HPP
#define COCKTAILENGINE_CORE_SIGNAL_SIGNAL_HPP

#include <CocktailEngine/Core/Signal/Detail/SignalBase.hpp>
#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/NullMutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/SpinMutex.hpp>

namespace Ck
{
    template <typename... Args>
    using Signal = Detail::SignalBase<Mutex, Args...>;

    template <typename... Args>
    using SpinSignal = Detail::SignalBase<SpinMutex, Args...>;

    template <typename... Args>
    using UnsafeSignal = Detail::SignalBase<NullMutex, Args...>;
}

#endif // COCKTAILENGINE_CORE_SIGNAL_SIGNAL_HPP
