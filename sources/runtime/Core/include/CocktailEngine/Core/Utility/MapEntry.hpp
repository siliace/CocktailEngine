#ifndef COCKTAILENGINE_CORE_UTILITY_MAPENTRY_HPP
#define COCKTAILENGINE_CORE_UTILITY_MAPENTRY_HPP

namespace Ck
{
    /**
     * \brief Represents a key-value pair stored in a map
     *
     * \c MapEntry is the public-facing element type for \c map.
     * Both the key and value are directly accessible as public members.
     *
     * \tparam TKey   Type of the key
     * \tparam TValue Type of the associated value
     */
    template <typename TKey, typename TValue>
    struct MapEntry
    {
        /**
         * \brief Type of the key
         */
        using KeyType = TKey;

        /**
         * \brief Type of the associated value
         */
        using ValueType = TValue;

        /**
         * \brief Default constructor — value-initialises both \c Key and \c Value
         */
        MapEntry() = default;

        /**
         * \brief Construct from a key and a value (copy)
         *
         * \param key   Key to store
         * \param value Value to associate with the key
         */
        MapEntry(const KeyType& key, const ValueType& value) :
            Key(key),
            Value(value)
        {
            /// Nothing
        }

        /**
         * \brief Construct from a key and a value (perfect forwarding)
         *
         * \tparam TOtherKey   Type forwarded as key
         * \tparam TOtherValue Type forwarded as value
         *
         * \param key   Key to store
         * \param value Value to associate with the key
         */
        template <typename TOtherKey = TKey, typename TOtherValue = TValue>
        MapEntry(TOtherKey&& key, TOtherValue&& value) :
            Key(Forward<TOtherKey>(key)),
            Value(Forward<TOtherValue>(value))
        {
            /// Nothing
        }

        /**
         * \brief The entry's key — used for lookup and hashing
         */
        KeyType Key;

        /**
         * \brief The entry's associated value
         */
        ValueType Value;
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_MAPENTRY_HPP
