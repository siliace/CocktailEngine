#ifndef COCKTAILENGINE_CORE_UTILITY_COMPOSITEKEY_HPP
#define COCKTAILENGINE_CORE_UTILITY_COMPOSITEKEY_HPP

#include <CocktailEngine/Core/Cocktail.hpp>

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

            bool operator<(const CompositeKeyItem& other) const
            {
                return value < other.value;
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
            CompositeKeyImpl() = default;

            /**
             * \brief
             * \param first
             * \param rest
             */
            explicit CompositeKeyImpl(T first, Ts... rest) :
                CompositeKeyItem<I, T>{ first },
                CompositeKeyImpl<I + 1, Ts...>(Forward<Ts>(rest)...)
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
             * \brief Orders two composite keys lexicographically
             *
             * Items are compared in declaration order; the first item that breaks
             * the tie decides. This is what lets a composite key be used in an
             * ordered container such as \c TreeMap, where a prefix of the key then
             * delimits a contiguous range of entries.
             *
             * \param other Key to compare against
             *
             * \return \c true when this key orders before \p other
             */
            bool operator<(const CompositeKeyImpl& other) const
            {
                const T& left = CompositeKeyItem<I, T>::value;
                const T& right = static_cast<const CompositeKeyItem<I, T>&>(other).value;

                if (left < right)
                    return true;
                if (right < left)
                    return false;

                if constexpr (sizeof...(Ts) > 0)
                    return CompositeKeyImpl<I + 1, Ts...>::operator<(other);

                return false;
            }

            /**
             * \brief Orders two composite keys lexicographically
             *
             * \param other Key to compare against
             *
             * \return \c true when this key orders after \p other
             */
            bool operator>(const CompositeKeyImpl& other) const
            {
                return other < *this;
            }

            /**
             * \brief Orders two composite keys lexicographically
             *
             * \param other Key to compare against
             *
             * \return \c true when this key does not order after \p other
             */
            bool operator<=(const CompositeKeyImpl& other) const
            {
                return !(other < *this);
            }

            /**
             * \brief Orders two composite keys lexicographically
             *
             * \param other Key to compare against
             *
             * \return \c true when this key does not order before \p other
             */
            bool operator>=(const CompositeKeyImpl& other) const
            {
                return !(*this < other);
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

#endif // COCKTAILENGINE_CORE_UTILITY_COMPOSITEKEY_HPP
