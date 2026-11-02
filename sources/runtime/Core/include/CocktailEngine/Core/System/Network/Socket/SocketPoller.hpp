#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETPOLLER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETPOLLER_HPP

#include <CocktailEngine/Core/Flags.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

namespace Ck
{
    class SocketOwner;

    /**
     * \brief Waits for read and write readiness on registered socket owners.
     *
     * A SocketPoller borrows the native handle owned by each registered
     * SocketOwner. The owner must retain the same valid socket until it is
     * removed or this poller is cleared. Remove an owner, or clear this poller,
     * before closing, destroying, moving, or move-assigning that owner.
     *
     * Call Wait before querying readiness. Its flags describe the most recent
     * wait and can become stale as soon as another thread or the operating
     * system changes a registered socket. A SocketPoller must not be copied.
     * Concurrent calls that mutate or query this poller, or that mutate a
     * registered owner, require external synchronization.
     */
    class COCKTAILENGINE_CORE_API SocketPoller
    {
    public:

        /**
         * \brief Identifies the readiness state to observe for a socket owner.
         */
        enum class EventFlagBits
        {
            /** \brief Observe whether data can be read without waiting. */
            Read = Bit(0),
            /** \brief Observe whether data can be written without waiting. */
            Write = Bit(1)
        };

        /**
         * \brief Bit mask of EventFlagBits values.
         */
        using EventFlags = Flags<EventFlagBits>;

        /**
         * \brief Constructs a poller with no registered owners.
         */
        SocketPoller();

        /**
         * \brief Prevents copying the platform-specific polling state.
         */
        SocketPoller(const SocketPoller& other) = delete;

        /**
         * \brief Transfers polling state and registrations from another poller.
         *
         * Registered owners remain registered in this poller after the transfer.
         * The source poller has no implementation afterwards and may only be
         * destroyed or assigned a new polling state before further use.
         *
         * \param other Poller to move from.
         */
        SocketPoller(SocketPoller&& other) noexcept;

        /**
         * \brief Destroys the platform-specific polling state.
         *
         * Registered owners are not closed or otherwise modified.
         */
        ~SocketPoller();

        /**
         * \brief Prevents copying the platform-specific polling state.
         */
        SocketPoller& operator=(const SocketPoller& other) = delete;

        /**
         * \brief Transfers polling state and registrations from another poller.
         *
         * Registered owners in \p other remain registered in this poller after
         * the transfer. The source poller has no implementation afterwards and
         * may only be destroyed or assigned a new polling state before further
         * use.
         *
         * \param other Poller to move from.
         *
         * \return This poller.
         */
        SocketPoller& operator=(SocketPoller&& other) noexcept;

        /**
         * \brief Registers an owner and the readiness events to observe.
         *
         * The poller does not take ownership of \p socketOwner. Remove the owner
         * or clear this poller before closing, destroying, moving, or
         * move-assigning it.
         *
         * \pre \p socketOwner owns a valid socket.
         * \pre \p socketOwner is not already registered in this poller.
         *
         * \param socketOwner Owner of the socket to observe.
         * \param eventFlags Readiness events to observe.
         */
        void Add(const SocketOwner& socketOwner, EventFlags eventFlags) const;

        /**
         * \brief Stops observing an owner in this poller.
         *
         * Removing an unregistered owner has no effect. The owner must not have
         * been closed or moved since it was registered, because removal matches
         * its current native handle.
         *
         * \param socketOwner Owner to stop observing.
         */
        void Remove(const SocketOwner& socketOwner) const;

        /**
         * \brief Stops observing every owner in this poller.
         *
         * Owners remain valid and may subsequently be closed, moved, or
         * destroyed independently of this poller.
         */
        void Clear() const;

        /**
         * \brief Waits until a registered owner reports an observed event.
         *
         * On return, query IsReadyToRead, IsReadyToWrite, or IsDisconnected for
         * registered owners to inspect the latest event flags.
         *
         * \param timeout Maximum time to wait. The default waits indefinitely.
         *
         * \return \c true when at least one registered owner reported an event;
         *         \c false when no owners are registered or the wait timed out.
         *
         * \throws SystemException If the platform polling operation fails.
         */
        bool Wait(Duration timeout = Duration::Infinite()) const;

        /**
         * \brief Checks whether a registered owner reported a disconnection.
         *
         * \pre \p socketOwner is registered in this poller and has not been
         *      closed or moved since registration.
         *
         * \param socketOwner Registered owner to query.
         *
         * \return \c true when the most recent Wait observed a hang-up event.
         */
        bool IsDisconnected(const SocketOwner& socketOwner) const;

        /**
         * \brief Checks whether a registered owner is ready to read.
         *
         * \pre \p socketOwner is registered in this poller and has not been
         *      closed or moved since registration.
         *
         * \param socketOwner Registered owner to query.
         *
         * \return \c true when the most recent Wait observed read readiness.
         */
        bool IsReadyToRead(const SocketOwner& socketOwner) const;

        /**
         * \brief Checks whether a registered owner is ready to write.
         *
         * \pre \p socketOwner is registered in this poller and has not been
         *      closed or moved since registration.
         *
         * \param socketOwner Registered owner to query.
         *
         * \return \c true when the most recent Wait observed write readiness.
         */
        bool IsReadyToWrite(const SocketOwner& socketOwner) const;

    private:

        /**
         * \brief Platform-specific polling implementation.
         */
        class Impl;
        Impl* mImpl;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_SOCKET_SOCKETPOLLER_HPP
