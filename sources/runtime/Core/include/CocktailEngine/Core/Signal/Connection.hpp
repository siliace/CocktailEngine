#ifndef COCKTAILENGINE_CORE_SIGNAL_CONNECTION_HPP
#define COCKTAILENGINE_CORE_SIGNAL_CONNECTION_HPP

#include <CocktailEngine/Core/Export.hpp>
#include <CocktailEngine/Core/Memory/WeakPtr.hpp>
#include <CocktailEngine/Core/Signal/Detail/SlotState.hpp>

namespace Ck::Detail
{
    template <Lockable, typename...>
    class SignalBase;
}

namespace Ck
{
    /**
     * \brief Move-only RAII handle for a signal subscription.
     *
     * A connection disconnects its slot when it is destroyed or overwritten by a moved-in
     * connection. Keep the object alive for as long as its callback must remain registered.
     * Operations on an empty, expired, or already disconnected connection are no-ops.
     */
    class COCKTAILENGINE_CORE_API Connection
    {
    public:

        /**
         * \brief Creates an empty connection.
         */
        Connection() = default;

        /**
         * \brief Connections have unique ownership of their subscriptions.
         */
        Connection(const Connection&) = delete;

        /**
         * \brief Transfers subscription ownership from another connection.
         *
         * \param other Connection to transfer from.
         */
        Connection(Connection&& other) noexcept = default;

        /**
         * \brief Disconnects the managed slot, if any.
         */
        ~Connection();

        /**
         * \brief Connections have unique ownership of their subscriptions.
         */
        Connection& operator=(const Connection&) = delete;

        /**
         * \brief Disconnects the current slot and transfers another subscription into this handle.
         *
         * \param other Connection to transfer from.
         * \return This connection.
         */
        Connection& operator=(Connection&& other) noexcept;

        /**
         * \brief Disconnects the underlying slot from its signal.
         */
        void Disconnect();

        /**
         * \brief Enables the managed slot, allowing it to receive later emissions.
         */
        void Enable() const;

        /**
         * \brief Disables the managed slot without disconnecting it.
         */
        void Disable() const;

        /**
         * \brief Tells whether this handle still manages an active slot.
         *
         * \return True if the slot is connected, otherwise false.
         */
        bool IsConnected() const;

        /**
         * \brief Tells whether the managed slot is disabled.
         *
         * \return True if the slot is disabled, otherwise false.
         */
        bool IsDisabled() const;

    private:

        template <Lockable, typename...>
        friend class Detail::SignalBase;

        /**
         * \brief Creates a connection that observes a slot.
         *
         * \param state Slot state to observe.
         */
        explicit Connection(WeakPtr<Detail::SlotState> state);

        WeakPtr<Detail::SlotState> mState;
    };
}

#endif // COCKTAILENGINE_CORE_SIGNAL_CONNECTION_HPP
