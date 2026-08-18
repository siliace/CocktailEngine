#ifndef COCKTAILENGINE_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP
#define COCKTAILENGINE_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Signal/Connection.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>

namespace Ck::Detail
{
    /**
     * \brief Owns the connections created by an observable object
     *
     * Each call to \ref Connect creates and stores a \ref Connection. Destroying these
     * stored connections disconnects their callbacks. The base destructor lets its member
     * collection perform that cleanup after derived members have been destroyed; a derived
     * destructor must call \ref DisconnectAll when callbacks must stop earlier.
     *
     * \tparam TLockable Lock implementation protecting the connection collection
     */
    template <Lockable TLockable>
    class COCKTAILENGINE_CORE_API ObservableBase
    {
    public:

        /**
         * \brief Creates an observable without owned connections
         */
        ObservableBase() = default;

        /**
         * \brief Observables cannot be copied because they uniquely own their connections
         */
        ObservableBase(const ObservableBase&) = delete;

        /**
         * \brief Transfers the owned connections from another observable
         *
         * The source no longer owns its transferred connections after this operation.
         *
         * \param other Observable that transfers ownership of its connections
         */
        ObservableBase(ObservableBase&& other) noexcept
        {
            LockGuard<TLockable, TLockable> lhs(mMutex, other.mMutex);

            mConnections = Move(other.mConnections);
        }

        /**
         * \brief Releases the owned connections after derived members are destroyed
         *
         * Derived classes that need callbacks to stop before their own members are destroyed
         * must call \ref DisconnectAll from their destructor.
         */
        virtual ~ObservableBase() = default;

        /**
         * \brief Observables cannot be copied because they uniquely own their connections
         */
        ObservableBase& operator=(const ObservableBase&) = delete;

        /**
         * \brief Replaces the owned connections with those from another observable
         *
         * Existing connections owned by this observable are released before the incoming
         * connections become owned by it. The source no longer owns its transferred
         * connections after this operation.
         *
         * \param other Observable that transfers ownership of its connections
         *
         * \return This observable
         */
        ObservableBase& operator=(ObservableBase&& other) noexcept
        {
            if (this != &other)
            {
                LockGuard<TLockable, TLockable> lhs(mMutex, other.mMutex);

                mConnections = Move(other.mConnections);
            }

            return *this;
        }

        /**
         * \brief Connects a callback and transfers its lifetime to this observable
         *
         * The callback is forwarded to \p signal and its connection is retained until
         * \ref DisconnectAll, this observable's destruction, or a move operation releases
         * it.
         *
         * \tparam Args Types delivered by the signal
         * \tparam Callback Type of the callback to connect
         *
         * \param signal Signal receiving the callback
         * \param callback Callback to connect
         */
        template <typename... Args, typename Callback>
        void Connect(SignalBase<TLockable, Args...>& signal, Callback&& callback)
        {
            LockGuard<TLockable> lg(mMutex);

            Connection connection = signal.Connect(Forward<Callback>(callback));
            mConnections.Add(Move(connection));
        }

    protected:

        /**
         * \brief Disconnects and releases every connection owned by this observable
         *
         * Call this from a derived destructor when callbacks must stop before derived members
         * are destroyed.
         */
        void DisconnectAll()
        {
            LockGuard<TLockable> lg(mMutex);
            mConnections.Clear();
        }

    private:

        TLockable mMutex;
        Array<Connection> mConnections;
    };
}

#endif // COCKTAILENGINE_CORE_SIGNAL_DETAIL_OBSERVABLEBASE_HPP
