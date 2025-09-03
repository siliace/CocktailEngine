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
		void Invoke(std::tuple<Args...> args)
		{
			if (SlotState::IsConnected() && !SlotState::IsDisabled())
				DoInvoke(std::move(args));
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
		virtual void DoInvoke(std::tuple<Args...>&& args) = 0;
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
		void DoInvoke(std::tuple<Args...>&& args) override
		{
			std::apply(mCallable, std::forward<std::tuple<Args...>>(args));
		}

	private:

		Callable mCallable;
	};

	template <typename T, typename... Args>
	class ObjectSlot : public Slot<Args...>
	{
	public:

		using ObjectType = T;
		using ReferenceType = T&;
		using FunctionType = void(T::*)(Args...);

		/**
		 * \brief
		 * \param object
		 * \param function
		 * \param container
		 * \param groupId
		 */
		ObjectSlot(ReferenceType object, FunctionType function, SlotContainer* container, unsigned int groupId) :
			Slot<Args...>(container, groupId),
			mObject(object),
			mFunction(function)
		{
			/// Nothing
		}

	protected:

		/**
		 * \brief 
		 * \param args 
		 */
		void DoInvoke(std::tuple<Args...>&& args) override
		{
			std::apply([&](Args&&... innerArgs) {
				std::invoke(mFunction, mObject, std::forward<Args>(innerArgs)...);
			}, std::forward<std::tuple<Args...>>(args));
		}

	private:

		ReferenceType mObject;
		FunctionType mFunction;
	};

	template <typename T, typename... Args>
	class ConstantObjectSlot : public Slot<Args...>
	{
	public:

		using ObjectType = T;
		using ReferenceType = const T&;
		using FunctionType = void(T::*)(Args...) const;

		/**
		 * \brief
		 * \param object
		 * \param function
		 * \param container
		 * \param groupId
		 */
		ConstantObjectSlot(ReferenceType object, FunctionType function, SlotContainer* container, unsigned int groupId) :
			Slot<Args...>(container, groupId),
			mObject(object),
			mFunction(function)
		{
			/// Nothing
		}

	protected:

		/**
		 * \brief
		 * \param args
		 */
		void DoInvoke(std::tuple<Args...>&& args) override
		{
			std::apply([&](Args&&... innerArgs) {
				std::invoke(mFunction, mObject, std::forward<Args>(innerArgs)...);
			}, std::forward<std::tuple<Args...>>(args));
		}

	private:

		ReferenceType mObject;
		FunctionType mFunction;
	};
}

#endif // COCKTAIL_CORE_SIGNAL_DETAIL_SLOT_HPP
