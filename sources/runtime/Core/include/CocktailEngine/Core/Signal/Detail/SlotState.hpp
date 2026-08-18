#ifndef COCKTAILENGINE_CORE_SIGNAL_DETAIL_SLOTSTATE_HPP
#define COCKTAILENGINE_CORE_SIGNAL_DETAIL_SLOTSTATE_HPP

#include <atomic>

#include <CocktailEngine/Core/Export.hpp>
#include <CocktailEngine/Core/Memory/WeakPtr.hpp>
#include <CocktailEngine/Core/System/Concurrency/Lockable.hpp>

namespace Ck::Detail
{
    class SlotContainer;

    template <Lockable, typename...>
    class SignalState;

    /**
     * \brief Connection state shared by a slot and its \ref Connection handles.
     *
     * The connected and disabled flags are atomic, allowing handles to query or change the
     * slot state independently of callback execution. The slot observes its stable registry
     * through a weak reference, so a connection never calls a destroyed signal facade.
     */
    class COCKTAILENGINE_CORE_API SlotState
    {
    public:

        /**
         * \brief Destroys the slot state.
         */
        virtual ~SlotState() = default;

        /**
         * \brief Atomically disconnects the slot and requests its removal from the registry.
         *
         * The registry is notified only when this call changes the state from connected to
         * disconnected. If the registry has already been destroyed, the slot is still marked
         * inactive and no removal is attempted.
         *
         * \return True if the slot was connected, otherwise false.
         */
        bool Disconnect();

        /**
         * \brief Enables callback invocation without reconnecting the slot.
         */
        void Enable();

        /**
         * \brief Disables callback invocation without disconnecting the slot.
         */
        void Disable();

        /**
         * \brief Tells whether the slot remains connected to its signal.
         *
         * \return True if the slot is connected, otherwise false.
         */
        bool IsConnected() const;

        /**
         * \brief Tells whether callback invocation is disabled for this slot.
         *
         * \return True if the slot is disabled, otherwise false.
         */
        bool IsDisabled() const;

        /**
         * \brief Gets the immutable callback invocation group.
         *
         * \return Group identifier used to order the slot.
         */
        unsigned int GetGroupId() const;

    protected:

        /**
         * \brief Creates state for a slot owned by a stable signal registry.
         *
         * \param container Registry that owns the slot.
         * \param groupId Group used to order callback invocation.
         */
        SlotState(WeakPtr<SlotContainer> container, unsigned int groupId);

    private:

        template <Lockable, typename...>
        friend class SignalState;

        /**
         * \brief Marks the slot as disconnected without notifying its registry.
         *
         * SignalState uses this while it already owns the registry lock and controls the
         * physical removal of the slot from its maps.
         *
         * \return True when the slot changed from connected to disconnected.
         */
        bool DisconnectSilently();

        WeakPtr<SlotContainer> mContainer;
        unsigned int mGroupId;
        std::atomic<bool> mConnected;
        std::atomic<bool> mDisabled;
    };
}

#endif // COCKTAILENGINE_CORE_SIGNAL_DETAIL_SLOTSTATE_HPP
