#ifndef COCKTAILENGINE_CORE_SIGNAL_DETAIL_SLOTCONTAINER_HPP
#define COCKTAILENGINE_CORE_SIGNAL_DETAIL_SLOTCONTAINER_HPP

namespace Ck::Detail
{
    class SlotState;

    /**
     * \brief Interface through which a signal owns and removes its slots.
     *
     * A \ref SlotState notifies this interface after it atomically disconnects itself.
     * Implementations decide whether physical removal occurs immediately or is deferred until
     * an active emission completes.
     */
    class SlotContainer
    {
    public:

        /**
         * \brief Destroys the slot container.
         */
        virtual ~SlotContainer() = default;

        /**
         * \brief Disconnects every slot owned by the signal.
         */
        virtual void Disconnect() = 0;

        /**
         * \brief Disconnects every slot in an invocation group.
         *
         * \param groupId Group identifier to disconnect.
         */
        virtual void Disconnect(unsigned int groupId) = 0;

    protected:

        friend class SlotState;

        /**
         * \brief Removes a slot that has already transitioned to the disconnected state.
         *
         * This function is called by \ref SlotState after an explicit connection
         * disconnection. Implementations may defer physical removal while emitting.
         *
         * \param state State identifying the slot to remove.
         */
        virtual void Disconnect(SlotState* state) = 0;
    };
}

#endif // COCKTAILENGINE_CORE_SIGNAL_DETAIL_SLOTCONTAINER_HPP
