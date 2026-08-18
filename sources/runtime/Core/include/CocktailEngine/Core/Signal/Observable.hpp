#ifndef COCKTAILENGINE_CORE_SIGNAL_OBSERVABLE_HPP
#define COCKTAILENGINE_CORE_SIGNAL_OBSERVABLE_HPP

#include <CocktailEngine/Core/Signal/Detail/ObservableBase.hpp>
#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/NullMutex.hpp>

namespace Ck
{
    /**
     * \brief Thread-safe owner for a set of signal connections.
     *
     * Derive from this type to bind callbacks whose lifetimes are owned together. Call
     * \ref Detail::ObservableBase::DisconnectAll from the derived destructor when its
     * subscriptions must stop before other derived resources are destroyed.
     */
    using Observable = Detail::ObservableBase<Mutex>;

    /**
     * \brief Unsynchronized owner for a set of signal connections.
     *
     * This variant does not synchronize access to its connection collection. The caller
     * must serialize connection management, movement, and destruction externally.
     */
    using TlsObservable = Detail::ObservableBase<NullMutex>;
}

#endif // COCKTAILENGINE_CORE_SIGNAL_OBSERVABLE_HPP
