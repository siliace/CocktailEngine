#ifndef COCKTAILENGINE_CORE_SIGNAL_DETAIL_SIGNALSTATE_HPP
#define COCKTAILENGINE_CORE_SIGNAL_DETAIL_SIGNALSTATE_HPP

#include <cstddef>
#include <limits>
#include <tuple>
#include <utility>

#include <CocktailEngine/Core/Memory/SharedPtr.hpp>
#include <CocktailEngine/Core/Signal/Detail/Slot.hpp>
#include <CocktailEngine/Core/Signal/Detail/SlotContainer.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/TreeMap.hpp>
#include <CocktailEngine/Core/Utility/CompositeKey.hpp>

namespace Ck::Detail
{
    /**
     * \brief Stable slot registry owned by a signal facade.
     *
     * Slots retain a weak reference to this registry rather than to \ref SignalBase. Moving
     * the facade therefore preserves the registry identity observed by existing connections.
     * Callbacks execute without the registry lock and may safely manipulate this state.
     *
     * \tparam TLockable Lock implementation protecting the registry.
     * \tparam Args Callback argument types.
     */
    template <Lockable TLockable, typename... Args>
    class SignalState final : public SlotContainer
    {
    private:

        using SlotKey = CompositeKey<Uint32, Uint64>;
        using SlotMap = TreeMap<SlotKey, SharedPtr<Slot<Args...>>>;

    public:

        /**
         * \brief Creates an empty slot registry.
         */
        SignalState() = default;

        /**
         * \brief Disconnects every remaining slot before the registry is destroyed.
         */
        ~SignalState() override
        {
            Disconnect();
        }

        SignalState(const SignalState&) = delete;
        SignalState& operator=(const SignalState&) = delete;
        SignalState(SignalState&&) = delete;
        SignalState& operator=(SignalState&&) = delete;

        /**
         * \brief Registers a newly created slot.
         *
         * Connections created while an emission is active are deferred until the outermost
         * emission completes.
         *
         * \param slot Slot to register.
         * \param groupId Group used to order callback invocation.
         */
        void Register(const SharedPtr<Slot<Args...>>& slot, unsigned int groupId)
        {
            LockGuard<TLockable> lock(mSlotLock);

            const SlotKey slotKey{ groupId, ++mNextSlotId };
            if (mEmissionDepth != 0)
                mConnectingSlots.Put(slotKey, slot);
            else
                mSlots.Put(slotKey, slot);
        }

        /**
         * \brief Invokes the slots active when the emission begins without allocating.
         *
         * The next slot is retained while the registry lock is held, then invoked after the
         * lock is released. New connections are deferred until the outermost emission
         * completes. A callback that disconnects a slot, a group, or the entire signal
         * prevents slots that have not yet run from being invoked. Callback exceptions are
         * propagated after deferred mutations are applied.
         *
         * \param args Arguments supplied to every callback.
         */
        void Emit(Args... args)
        {
            SlotKey lastSlotKey{};
            {
                LockGuard<TLockable> lock(mSlotLock);
                ++mEmissionDepth;
            }

            try
            {
                while (const SharedPtr<Slot<Args...>> slot = GetNextSlot(lastSlotKey))
                    slot->Invoke(std::forward_as_tuple(args...));
            }
            catch (...)
            {
                CompleteEmission();
                throw;
            }

            CompleteEmission();
        }

        /**
         * \brief Tells whether the registry contains an active connection.
         *
         * \return True when an active slot is connected, otherwise false.
         */
        bool IsBound() const
        {
            LockGuard<TLockable> lock(mSlotLock);
            return HasConnectedSlot(mSlots) || HasConnectedSlot(mConnectingSlots);
        }

        /**
         * \brief Disconnects every registered slot.
         *
         * Slots that are part of an active emission become inactive immediately and are not
         * invoked later in that emission.
         */
        void Disconnect() override
        {
            LockGuard<TLockable> lock(mSlotLock);
            DisconnectAllSlots(mSlots);
            DisconnectAllSlots(mConnectingSlots);
            DisconnectAllSlots(mDisconnectingSlots);
        }

        /**
         * \brief Disconnects every slot in an invocation group.
         *
         * Slots that are part of an active emission become inactive immediately and are not
         * invoked later in that emission.
         *
         * \param groupId Group identifier to disconnect.
         */
        void Disconnect(unsigned int groupId) override
        {
            LockGuard<TLockable> lock(mSlotLock);
            DisconnectGroupSlots(mSlots, groupId, mEmissionDepth != 0);
            DisconnectGroupSlots(mConnectingSlots, groupId, false);
        }

    protected:

        /**
         * \brief Removes a slot after its connection state became inactive.
         *
         * \param state State identifying the slot to remove.
         */
        void Disconnect(SlotState* state) override
        {
            LockGuard<TLockable> lock(mSlotLock);
            if (mEmissionDepth != 0)
            {
                const auto [begin, end] = GetGroupRange(mSlots, state->GetGroupId());
                for (auto it = begin; it != end; ++it)
                {
                    if (it->Value.Get() == state)
                    {
                        mDisconnectingSlots.Put(it->Key, it->Value);
                        return;
                    }
                }

                return;
            }

            EraseSlot(state);
        }

    private:

        /**
         * \brief Retains the next active slot in traversal order.
         *
         * \param lastSlotKey Key of the slot most recently examined.
         *
         * \return The next slot, or an empty pointer when traversal is complete.
         */
        SharedPtr<Slot<Args...>> GetNextSlot(SlotKey& lastSlotKey)
        {
            LockGuard<TLockable> lock(mSlotLock);
            const auto it = mSlots.UpperBound(lastSlotKey);
            if (!it.IsValid())
                return nullptr;

            lastSlotKey = it->Key;
            return it->Value;
        }

        /**
         * \brief Finalizes the outermost active emission.
         */
        void CompleteEmission()
        {
            LockGuard<TLockable> lock(mSlotLock);
            --mEmissionDepth;
            if (mEmissionDepth != 0)
                return;

            for (auto it = mConnectingSlots.GetIterator(); it.IsValid();)
            {
                if (it->Value->IsConnected())
                    ++it;
                else
                    it = mConnectingSlots.Remove(it);
            }

            mSlots.Merge(mConnectingSlots);

            for (const auto& [slotKey, slot] : mDisconnectingSlots)
                EraseSlot(slot.Get());

            mDisconnectingSlots.Clear();
        }

        /**
         * \brief Erases a slot from the active slot map.
         *
         * \param state State identifying the slot to erase.
         */
        void EraseSlot(SlotState* state)
        {
            const auto [begin, end] = GetGroupRange(mSlots, state->GetGroupId());
            for (auto it = begin; it != end; ++it)
            {
                if (it->Value.Get() == state)
                {
                    mSlots.Remove(it);
                    return;
                }
            }
        }

        /**
         * \brief Returns whether a slot map contains an active slot.
         *
         * \param slots Slot map to inspect.
         *
         * \return True if an active slot is present.
         */
        static bool HasConnectedSlot(const SlotMap& slots)
        {
            for (const auto& [slotKey, slot] : slots)
            {
                if (slot->IsConnected())
                    return true;
            }

            return false;
        }

        /**
         * \brief Marks every slot inactive and releases the map entries.
         *
         * \param slots Slot map to clear.
         */
        static void DisconnectAllSlots(SlotMap& slots)
        {
            for (const auto& [slotKey, slot] : slots)
                slot->DisconnectSilently();

            slots.Clear();
        }

        /**
         * \brief Marks and removes every slot in an invocation group.
         *
         * \param slots Slot map to modify.
         * \param groupId Group identifier to disconnect.
         * \param deferRemoval Whether active-map erasure must wait for an emission to end.
         */
        void DisconnectGroupSlots(SlotMap& slots, unsigned int groupId, bool deferRemoval)
        {
            const auto [begin, end] = GetGroupRange(slots, groupId);
            for (auto it = begin; it != end;)
            {
                it->Value->DisconnectSilently();
                if (deferRemoval)
                {
                    mDisconnectingSlots.Put(it->Key, it->Value);
                    ++it;
                }
                else
                {
                    it = slots.Remove(it);
                }
            }
        }

        /**
         * \brief Obtains the contiguous key range belonging to a group.
         *
         * \param slots Slot map to query.
         * \param groupId Group identifier to locate.
         *
         * \return Iterators delimiting the group range.
         */
        static SlotMap::Bounds GetGroupRange(SlotMap& slots, unsigned int groupId)
        {
            return slots.GetBounds(
                SlotKey(groupId, 0),
                SlotKey(groupId, std::numeric_limits<Uint64>::max())
            );
        }

        std::size_t mEmissionDepth = 0;
        Uint64 mNextSlotId = 0;
        mutable TLockable mSlotLock;
        SlotMap mSlots;
        SlotMap mConnectingSlots;
        SlotMap mDisconnectingSlots;
    };
}

#endif // COCKTAILENGINE_CORE_SIGNAL_DETAIL_SIGNALSTATE_HPP
