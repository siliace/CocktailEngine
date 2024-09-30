#ifndef COCKTAIL_CORE_UTILITY_PARAMETERPACK_HPP
#define COCKTAIL_CORE_UTILITY_PARAMETERPACK_HPP

namespace Ck
{
	/**
	 * \brief 
	 * \tparam Ts 
	 */
	template <typename... Ts>
	struct ParameterPack
	{
		/**
		 * \brief 
		 * \tparam S 
		 */
		template <template <typename...> class S>
		using Apply = S<Ts...>;

		/**
		 * \brief 
		 * \tparam S 
		 * \tparam Ts2 
		 */
		template <template <typename...> class S, typename... Ts2>
		using ApplyBefore = S<Ts..., Ts2...>;

		/**
		 * \brief 
		 * \tparam S 
		 * \tparam Ts2 
		 */
		template <template <typename...> class S, typename... Ts2>
		using ApplyAfter = S<Ts2..., Ts...>;
	};

	/**
	 * \brief 
	 * \tparam Ts 
	 */
	template <typename... Ts>
	using DecayParameterPack = ParameterPack<std::decay_t<Ts>...>;
}

#endif // COCKTAIL_CORE_UTILITY_PARAMETERPACK_HPP
