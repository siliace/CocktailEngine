#ifndef COCKTAILENGINE_CORE_SIGNAL_SIGNAL_HPP
#define COCKTAILENGINE_CORE_SIGNAL_SIGNAL_HPP

#include <CocktailEngine/Core/Signal/Detail/SignalBase.hpp>
#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/NullMutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/SpinMutex.hpp>

namespace Ck
{
    /**
     * \brief Thread-safe signal with a mutex-backed subscription registry.
     *
     * Use this as the default signal type. It synchronizes registration, disconnection,
     * and traversal of the subscription registry. Callbacks execute without the registry
     * lock; callback bodies and listener-object lifetimes remain the caller's responsibility.
     *
     * \tparam Args Types delivered to connected callbacks.
     */
    template <typename... Args>
    using Signal = Detail::SignalBase<Mutex, Args...>;

    /**
     * \brief Thread-safe signal optimized for short registry critical sections.
     *
     * Callbacks execute outside the spin lock and may manipulate the same signal. Reserve
     * this type for cases where registration and disconnection are both brief and highly
     * contended; use \ref Signal as the default choice.
     *
     * \tparam Args Types delivered to connected callbacks.
     */
    template <typename... Args>
    using SpinSignal = Detail::SignalBase<SpinMutex, Args...>;

    /**
     * \brief Signal without internal synchronization.
     *
     * The caller must externally serialize every access, including connection management,
     * emission, movement, and destruction.
     *
     * \tparam Args Types delivered to connected callbacks.
     */
    template <typename... Args>
    using UnsafeSignal = Detail::SignalBase<NullMutex, Args...>;
}

#endif // COCKTAILENGINE_CORE_SIGNAL_SIGNAL_HPP
