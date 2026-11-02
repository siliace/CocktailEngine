#ifndef COCKTAILENGINE_CORE_SYSTEM_NETWORK_UNIX_SOCKETPOLLERIMPL_HPP
#define COCKTAILENGINE_CORE_SYSTEM_NETWORK_UNIX_SOCKETPOLLERIMPL_HPP

#include <poll.h>

#include <CocktailEngine/Core/System/Network/Socket/Socket.hpp>
#include <CocktailEngine/Core/System/Network/Socket/SocketPoller.hpp>

namespace Ck
{
    /**
     * \brief Unix implementation of SocketPoller's native descriptor registry.
     */
    class SocketPoller::Impl
    {
    public:

        /**
         * \brief Adds a native socket descriptor to the polling set.
         *
         * \param socketHandle Native descriptor to observe.
         * \param flags Readiness events to observe.
         */
        void Add(Socket::Handle socketHandle, EventFlags flags);

        /**
         * \brief Removes every registration for a native socket descriptor.
         *
         * \param socketHandle Native descriptor to stop observing.
         */
        void Remove(Socket::Handle socketHandle);

        /**
         * \brief Removes every native descriptor from the polling set.
         */
        void Clear();

        /**
         * \brief Checks whether a native descriptor is registered.
         *
         * \param socketHandle Native descriptor to query.
         *
         * \return \c true when the descriptor is registered, \c false otherwise.
         */
        bool Has(Socket::Handle socketHandle) const;

        /**
         * \brief Waits for events on the registered native descriptors.
         *
         * \param duration Maximum duration to wait.
         *
         * \return \c true when at least one descriptor reported an event, or
         *         \c false when no descriptor is registered or the wait timed out.
         *
         * \throws SystemException If poll fails.
         */
        bool Wait(const Duration& duration);

        /**
         * \brief Checks whether a descriptor reported a hang-up.
         *
         * \pre \p socketHandle is registered.
         *
         * \param socketHandle Registered native descriptor to query.
         *
         * \return \c true when the latest poll reported a hang-up event.
         */
        bool IsDisconnected(Socket::Handle socketHandle) const;

        /**
         * \brief Checks whether a descriptor reported read readiness.
         *
         * \pre \p socketHandle is registered.
         *
         * \param socketHandle Registered native descriptor to query.
         *
         * \return \c true when the latest poll reported read readiness.
         */
        bool IsReadyToRead(Socket::Handle socketHandle) const;

        /**
         * \brief Checks whether a descriptor reported write readiness.
         *
         * \pre \p socketHandle is registered.
         *
         * \param socketHandle Registered native descriptor to query.
         *
         * \return \c true when the latest poll reported write readiness.
         */
        bool IsReadyToWrite(Socket::Handle socketHandle) const;

    private:

        /**
         * \brief Finds the descriptor record for a native socket handle.
         *
         * \param socketHandle Native descriptor to look up.
         *
         * \return Pointer to the matching record, or \c nullptr when absent.
         */
        const pollfd* GetSocketDescriptor(Socket::Handle socketHandle) const;

        /**
         * \brief Native descriptors and their latest readiness flags.
         */
        Array<pollfd> mDescriptors;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_NETWORK_UNIX_SOCKETPOLLERIMPL_HPP
