#ifndef COCKTAIL_CORE_SIGNAL_DETAIL_SLOT_HPP
#define COCKTAIL_CORE_SIGNAL_DETAIL_SLOT_HPP

#include <Cocktail/Core/Signal/Detail/SlotState.hpp>

namespace Ck::Detail
{
	/**
	 * \brief 
	 */
	template <typename... Args>
	class Slot : public Extends<Slot<Args...>, SlotState>
	{
	public:

		/**
		 * \brief 
		 * \param args 
		 */
		void Invoke(Args&&... args)
		{
			if (Super::IsConnected() && !Super::IsDisabled())
				DoInvoke(std::forward<Args>(args)...);
		}
		
	protected:

		/**
		 * \brief
		 * \param container
		 * \param groupId
		 */
		Slot(SlotContainer* container, unsigned int groupId) :
			Super(container, groupId)
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
	class CallableSlot final : public Extends<CallableSlot<Callable, Args...>, Slot<Args...>>
	{
	public:

		/**
		 * \brief
		 * \param callable
		 * \param container 
		 * \param groupId 
		 */
		CallableSlot(Callable&& callable, SlotContainer* container, unsigned int groupId) :
			Super(container, groupId),
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

	template <typename Function, typename Object, typename... Args>
	class FunctionMemberSlot : public Extends<FunctionMemberSlot<Function, Object, Args...>, Slot<Args...>>
	{
	public:

		FunctionMemberSlot(Function&& function, Object&& object, SlotContainer* container, unsigned int groupId) :
			Super(container, groupId),
			mFunction(std::forward<Function>(function)),
			mObject(std::forward<Object>(object))
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
		}

	private:

		std::decay_t<Function> mFunction;
		std::decay_t<Object> mObject;
	};
}

#endif // COCKTAIL_CORE_SIGNAL_DETAIL_SLOT_HPP
