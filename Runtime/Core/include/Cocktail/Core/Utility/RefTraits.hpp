#ifndef COCKTAIL_CORE_UTILITY_REFTRAITS_HPP
#define COCKTAIL_CORE_UTILITY_REFTRAITS_HPP

namespace Ck
{
	template <typename T>
	struct RemoveRef
	{
		using Type = T;
	};

	template <typename T>
	class Ref;

	template <typename T>
	struct RemoveRef<Ref<T>>
	{
		using Type = T;
	};

	template <typename T>
	using RemoveRefType = typename RemoveRef<T>::Type;
}
#endif // COCKTAIL_CORE_UTILITY_REFTRAITS_HPP
