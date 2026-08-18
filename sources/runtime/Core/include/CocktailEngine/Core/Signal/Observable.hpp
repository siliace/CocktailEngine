#ifndef COCKTAILENGINE_CORE_SIGNAL_OBSERVABLE_HPP
#define COCKTAILENGINE_CORE_SIGNAL_OBSERVABLE_HPP

#include <CocktailEngine/Core/Signal/Detail/ObservableBase.hpp>
#include <CocktailEngine/Core/System/Concurrency/NullMutex.hpp>

namespace Ck
{
    /**
     * \brief Specialization of ObservableBase to be used in multi threaded contexts
     */
    using Observable = Detail::ObservableBase<std::recursive_mutex>;

    /**
     * \brief Specialization of ObservableBase to be used in single threaded contexts
     */
    using TlsObservable = Detail::ObservableBase<NullMutex>;
}

#endif // COCKTAILENGINE_CORE_SIGNAL_OBSERVABLE_HPP
