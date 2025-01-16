#ifndef COCKTAIL_CORE_SIGNAL_DETAIL_SIGNALBASE_HPP
#define COCKTAIL_CORE_SIGNAL_DETAIL_SIGNALBASE_HPP

#include <memory>
#include <map>

#include <Cocktail/Core/Signal/Connection.hpp>
#include <Cocktail/Core/Signal/Detail/Slot.hpp>
#include <Cocktail/Core/Signal/Detail/SlotContainer.hpp>

namespace Ck::Detail
{
	/**
     * \brief 
     */
    template <typename Lockable, typename... Args>
    class SignalBase : public SlotContainer
    {
    public:

	    /**
         * \brief Default constructor
         */
        SignalBase() = default;

	    /**
         * \brief Copy constructor
         */
        SignalBase(const SignalBase&) = delete;

	    /**
         * \brief Move constructor
         * \param other 
         */
        SignalBase(SignalBase&& other) noexcept
        {
            std::lock_guard<Lockable> lhs(mSlotLock);
            std::lock_guard<Lockable> rhs(other.mSlotLock);

            mSlots = other.mSlots;
        }

	    /**
         * \brief 
         * \param other 
         * \return 
         */
        SignalBase& operator=(const SignalBase& other) = delete;

	    /**
         * \brief 
         * \param other 
         * \return 
         */
        SignalBase& operator=(SignalBase&& other) noexcept
        {
            std::lock_guard<Lockable> lhs(mSlotLock);
            std::lock_guard<Lockable> rhs(other.mSlotLock);

            mSlots = other.mSlots;

            return *this;
        }

	    /**
         * \brief Connect a callable with compatible arguments
         * \param callable The callable
         * \param groupId The groupId identifier used to order Slot invocation
         * \return A connection used to manage the Slot
         */
        template <typename Callable>
        Connection Connect(Callable&& callable, unsigned int groupId = 0)
        {
            // The real slot type
            using SlotType = CallableSlot<Callable, Args...>;

            // Create the slot into the signal
            std::shared_ptr<Slot<Args...>> slot = CreateSlot<SlotType, Callable>(std::forward<Callable>(callable), groupId);

            // Create a connection to the slot we created
        	return Connection(slot);
        }

	    /**
         * \brief 
         * \param args 
         */
        void Emit(Args... args)
        {
            std::lock_guard<Lockable> lg(mSlotLock);
            for (const auto& pair : mSlots)
				pair.second->Invoke(std::forward<Args>(args)...);
        }

        bool IsBound() const
        {
            return !mSlots.empty();
        }

		/**
		 * \brief Disconnect every Slot from the Signal
		 */
        void Disconnect() override
        {
			std::lock_guard<Lockable> lg(mSlotLock);
            mSlots.clear();
        }

		/**
		 * \brief Disconnect a whole group from the Signal
		 * \param groupId The group to disconnect
		 */
        void Disconnect(unsigned int groupId) override
        {
			std::lock_guard<Lockable> lg(mSlotLock);
            for (auto it = mSlots.begin(); it != mSlots.end();)
            {
                if (it->first == groupId)
                {
                    it = mSlots.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

    protected:
        
		/**
		 * \brief Disconnect a Slot from the Signal
		 * \param state The Slot to disconnect
		 */
        void Disconnect(SlotState* state) override
        {
            unsigned int groupId = state->GetGroupId();

            std::lock_guard<Lockable> lg(mSlotLock);
            auto beginEndPair = mSlots.equal_range(groupId);
			auto begin = beginEndPair.first;
			auto end = beginEndPair.second;
            for (auto it = begin; it != end; ++it)
            {
	            if (it->second.get() == state)
	            {
                    mSlots.erase(it);
                    return;
	            }
            }
        }

    private:

        /**
         * \brief Create a new Slot into the Signal
         * \param args Arguments to use to construct the new Slot
         * \param groupId The group of the Slot to create
         * \return The created slot
         */
        template <typename T, typename... SlotArgs>
        std::shared_ptr<Slot<Args...>> CreateSlot(SlotArgs&&... args, unsigned int groupId)
        {
            std::shared_ptr<Slot<Args...>> slot = std::make_shared<T>(std::forward<SlotArgs>(args)..., this, groupId);

            {
				std::lock_guard<Lockable> lg(mSlotLock);
                mSlots.insert({ groupId, slot });
            }

            return slot;
        }

        Lockable mSlotLock;
        std::multimap<unsigned int, std::shared_ptr<Slot<Args...>>> mSlots;
    };
}

#endif // COCKTAIL_CORE_SIGNAL_DETAIL_SIGNALBASE_HPP
