#ifndef COCKTAIL_CORE_UTILITY_FUNCTIONTRAITS_HPP
#define COCKTAIL_CORE_UTILITY_FUNCTIONTRAITS_HPP

#include <tuple>

#include <Cocktail/Core/Utility/ParameterPack.hpp>

namespace Ck
{
	template <typename Function>
	struct FunctionTraits : FunctionTraits<decltype(&std::remove_reference_t<Function>::operator())>
	{
	};

	/**
	 * \brief 
	 * \tparam Class 
	 * \tparam ReturnType 
	 * \tparam Args 
	 */
	template <typename Class, typename ReturnType, typename... Args>
	struct FunctionTraits<ReturnType(Class::*)(Args ...) const>
	{
		/**
		 * \brief 
		 */
		using Return = ReturnType;

		/**
		 * \brief 
		 */
		constexpr static std::size_t Arity = sizeof...(Args);

		/**
		 * \brief 
		 */
		using Arguments = ParameterPack<Args...>;
		/**
		 * \brief 
		 * \tparam I 
		 */
		template <std::size_t I>
		using Arg = std::tuple_element_t<I, std::tuple<Args...>>;
	};

	template <typename Callable>
	using FunctionReturnType = typename FunctionTraits<Callable>::Return;

	template <typename Callable>
	using FunctionArgumentsTuple = typename FunctionTraits<Callable>::Arguments::template Apply<std::tuple>;

	template <typename Callable, std::size_t I>
	using FunctionParameterType = typename FunctionTraits<Callable>::template Arg<I>;
}

#endif // COCKTAIL_CORE_UTILITY_FUNCTIONTRAITS_HPP
