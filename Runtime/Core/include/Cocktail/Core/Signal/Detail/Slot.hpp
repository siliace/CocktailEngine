#ifndef COCKTAIL_CORE_SIGNAL_DETAIL_SLOT_HPP
#define COCKTAIL_CORE_SIGNAL_DETAIL_SLOT_HPP

#include <functional>

#include <Cocktail/Core/Signal/Detail/SlotState.hpp>

namespace Ck::Detail
{
	/**
	 * \brief 
	 */
	template <typename... Args>
	class Slot : public SlotState
	{
	public:

		/**
		 * \brief 
		 * \param args 
		 */
		void Invoke(Args&&... args)
		{
			if (SlotState::IsConnected() && !SlotState::IsDisabled())
				DoInvoke(std::forward<Args>(args)...);
		}
		
	protected:

		/**
		 * \brief
		 * \param container
		 * \param groupId
		 */
		Slot(SlotContainer* container, unsigned int groupId) :
			SlotState(container, groupId)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param args 
		 */
		virtual void DoInvoke(Args&&... args) = 0;
	};

	/**
	 * \brief 
	 */
	template <typename Callable, typename... Args>
	class CallableSlot final : public Slot<Args...>
	{
	public:

		/**
		 * \brief
		 * \param callable
		 * \param container 
		 * \param groupId 
		 */
		CallableSlot(Callable&& callable, SlotContainer* container, unsigned int groupId) :
			Slot<Args...>(container, groupId),
			mCallable(std::forward<Callable>(callable))
		{
			/// Nothing
		}

	protected:

		/**
		 * \brief 
		 * \param args 
		 */
		void DoInvoke(Args&&... args) override
		{
			std::invoke(mCallable, std::forward<Args>(args)...);
		}

	private:

		Callable mCallable;
	};
}

#endif // COCKTAIL_CORE_SIGNAL_DETAIL_SLOT_HPP
