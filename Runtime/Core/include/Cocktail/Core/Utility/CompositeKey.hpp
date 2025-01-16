#ifndef COCKTAIL_CORE_UTILITY_COMPOSITEKEY_HPP
#define COCKTAIL_CORE_UTILITY_COMPOSITEKEY_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
	namespace Detail
	{
		template <std::size_t I, typename T>
		struct CompositeKeyItem
		{
			T value;

			bool operator==(const CompositeKeyItem& other) const
			{
				return value == other.value;
			}

			bool operator!=(const CompositeKeyItem& other) const
			{
				return value != other.value;
			}
		};

		template <std::size_t I, typename... Items>
		struct CompositeKeyImpl;

		template <std::size_t I>
		struct CompositeKeyImpl<I>
		{
			constexpr std::size_t GetSize() const
			{
				return I;
			}
		};

		/**
		 * \brief 
		 */
		template <std::size_t I, typename T, typename... Ts>
		struct CompositeKeyImpl<I, T, Ts...> : CompositeKeyItem<I, T>, CompositeKeyImpl<I + 1, Ts...>
		{
			/**
			 * \brief 
			 * \param first 
			 * \param rest 
			 */
			explicit CompositeKeyImpl(T first, Ts... rest) :
				CompositeKeyItem<I, T>{ first },
				CompositeKeyImpl<I + 1, Ts...>(std::forward<Ts>(rest)...)
			{
				/// Nothing
			}

			/**
			 * \brief 
			 * \param other 
			 * \return 
			 */
			bool operator==(const CompositeKeyImpl& other) const
			{
				if constexpr (sizeof...(Ts) > 0)
					return CompositeKeyItem<I, T>::operator==(other) && CompositeKeyImpl<I + 1, Ts...>::operator==(other);

				return CompositeKeyItem<I, T>::operator==(other);
			}

			/**
			 * \brief 
			 * \param other 
			 * \return 
			 */
			bool operator!=(const CompositeKeyImpl& other) const
			{
				return !(*this == other);
			}

			/**
			 * \brief 
			 * \return 
			 */
			std::size_t GetHashCode() const
			{
				std::size_t hashCode = 0;
				HashCombine(hashCode, CompositeKeyItem<I, T>::value);
				if constexpr (sizeof...(Ts) > 0)
					hashCode = HashMerge(hashCode, CompositeKeyImpl<I + 1, Ts...>::GetHashCode());

				return hashCode;
			}
		};
	}

	template <typename... Ts>
	using CompositeKey = Detail::CompositeKeyImpl<0, Ts...>;
}

namespace std
{
#ifdef COCKTAIL_OS_GNU_LINUX
	template <typename>
	struct hash;
#endif

	template <typename... Ts>
	struct hash<Ck::CompositeKey<Ts...>>
	{
		size_t operator()(const Ck::CompositeKey<Ts...>& key) const noexcept
		{
			return key.GetHashCode();
		}
	};

	template <std::size_t I, typename HeadItem, typename... TailItems>
	HeadItem& get(Ck::Detail::CompositeKeyImpl<I, HeadItem, TailItems...>& tuple)
	{
		return tuple.template CompositeKeyItem<I, HeadItem>::value;
	}

	template <std::size_t I, typename HeadItem, typename... TailItems>
	const HeadItem& get(const Ck::Detail::CompositeKeyImpl<I, HeadItem, TailItems...>& tuple)
	{
		return tuple.template CompositeKeyItem<I, HeadItem>::value;
	}
} 

#endif // COCKTAIL_CORE_UTILITY_COMPOSITEKEY_HPP
