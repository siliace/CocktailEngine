#ifndef COCKTAIL_CORE_HASHMAP_HPP
#define COCKTAIL_CORE_HASHMAP_HPP

#include <Cocktail/Core/HashSet.hpp>

namespace Ck
{
    /**
     * \brief Represents a key-value pair stored in a HashMap
     *
     * \c MapEntry is the public-facing element type for \c HashMap.
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
            Key(std::forward<TOtherKey>(key)),
            Value(std::forward<TOtherValue>(value))
        {
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

    // -----------------------------------------------------------------------
    // Entry policy for HashMap
    // -----------------------------------------------------------------------

    namespace Detail
    {
        /**
         * \brief Entry policy that stores \c MapEntry<TKey, TValue> nodes
         *
         * This policy is used by \c HashTableBase to handle the HashMap-specific
         * semantics: keys are stored together with their values, and a \c Put
         * on a duplicate key replaces the existing value in place.
         *
         * \tparam TKey   Key type
         * \tparam TValue Value type
         */
        template <typename TKey, typename TValue>
        struct MapEntryPolicy
        {
            /**
             * \brief The type stored in each bucket node
             */
            using EntryType = MapEntry<TKey, TValue>;

            /**
             * \brief The type used to look up entries
             */
            using KeyType = TKey;

            /**
             * \brief Extract the lookup key from a stored entry
             *
             * \param entry The entry to extract the key from
             *
             * \return Const reference to the entry's key
             */
            static const KeyType& GetKey(const EntryType& entry)
            {
                return entry.Key;
            }

            /**
             * \brief Replace the value of an existing entry
             *
             * Called by \c Bucket::Put when a node with a matching key is found.
             * The key is left unchanged; only the value is updated.
             *
             * \tparam TOtherKey   Forwarded key argument (unused — key remains unchanged)
             * \tparam TOtherValue Forwarded replacement value
             *
             * \param entry Entry whose value should be updated
             * \param value New value to assign
             */
            template <typename TOtherKey, typename TOtherValue>
            static void ReplaceValue(EntryType& entry, TOtherKey&& /*key*/, TOtherValue&& value)
            {
                ObjectMemoryUtils::Destroy(&entry.Value);
                ObjectMemoryUtils::Construct(&entry.Value, std::forward<TOtherValue>(value));
            }
        };
    }

    /**
     * \brief Associative container storing key-value pairs using an open-addressing hash table
     *
     * \c HashMap provides average O(1) complexity for insertion, lookup, and
     * removal.  Entries are distributed across buckets according to the hash
     * value of their keys; collisions are resolved by chaining (each bucket is
     * a linked list).  A rehash is triggered automatically when the load factor
     * exceeds 0.75.
     *
     * \par Iteration order
     * Iteration is not in insertion order or key order — it follows bucket order.
     * The order may change after a rehash.
     *
     * \par Thread safety
     * This class is not thread-safe.  External synchronisation is required when
     * concurrent reads and writes are needed.
     *
     * \tparam TKey       Type of the keys
     * \tparam TValue     Type of the associated values
     * \tparam THash      Callable that produces a \c std::size_t hash for a \c TKey
     * \tparam TEqual     Binary predicate that returns \c true when two \c TKey values are equal
     * \tparam TAllocator Allocator type used for bucket-node memory
     */
    template <typename TKey, typename TValue, typename THash = std::hash<TKey>, typename TEqual = std::equal_to<TKey>, typename TAllocator = HeapAllocator>
    class HashMap : public Detail::HashTableBase<Detail::MapEntryPolicy<TKey, TValue>, THash, TEqual, TAllocator>
    {
        using Base = Detail::HashTableBase<Detail::MapEntryPolicy<TKey, TValue>, THash, TEqual, TAllocator>;
        using Node = typename Base::Node;

    public:

        /**
         * \brief Key type stored in each entry
         */
        using KeyType = TKey;

        /**
         * \brief Value type stored in each entry
         */
        using ValueType = TValue;

        /**
         * \brief Allocator type used for internal storage
         */
        using AllocatorType = TAllocator;

        /**
         * \brief Type used for sizes and indices
         */
        using SizeType = typename TAllocator::SizeType;

        /**
         * \brief Mutable iterator — dereferences to \c MapEntry<TKey, TValue>&
         *
         * \see Detail::HashTableIterator
         */
        using Iterator = Detail::HashTableIterator<Base, MapEntry<TKey, TValue>>;

        /**
         * \brief Read-only iterator — dereferences to \c const MapEntry<TKey, TValue>&
         *
         * \see Detail::HashTableConstIterator
         */
        using ConstIterator = Detail::HashTableConstIterator<Base, MapEntry<TKey, TValue>>;

        // -----------------------------------------------------------------------
        // Construction / assignment
        // -----------------------------------------------------------------------

        /**
         * \brief Constructs an empty map with the given initial bucket count
         *
         * The actual bucket count is rounded up to the next power of two with a
         * minimum of 8.
         *
         * \param bucketCount Desired initial bucket count (0 = use the default minimum)
         */
        explicit HashMap(SizeType bucketCount = 0) :
            Base(bucketCount)
        {
            /// Nothing
        }

        /**
         * \brief Copy constructor
         *
         * Deep-copies all entries from \p other.
         *
         * \param other Map to copy from
         */
        HashMap(const HashMap& other) :
            Base(other)
        {
            /// Nothing
        }

        /**
         * \brief Move constructor
         *
         * Transfers ownership of \p other's bucket array.
         * \p other is left empty after the operation.
         *
         * \param other Map to move from
         */
        HashMap(HashMap&& other) noexcept :
            Base(std::move(other))
        {
            /// Nothing
        }

        /**
         * \brief Copy assignment operator
         *
         * Clears this map, then deep-copies all entries from \p other.
         *
         * \param other Map to copy from
         *
         * \return Reference to \c *this
         */
        HashMap& operator=(const HashMap& other)
        {
            Base::operator=(other);
            return *this;
        }

        /**
         * \brief Move assignment operator
         *
         * Transfers ownership of \p other's bucket array.
         * \p other is left empty after the operation.
         *
         * \param other Map to move from
         *
         * \return Reference to \c *this
         */
        HashMap& operator=(HashMap&& other) noexcept
        {
            Base::operator=(std::move(other));
            return *this;
        }

        // -----------------------------------------------------------------------
        // Insertion
        // -----------------------------------------------------------------------

        /**
         * \brief Insert a new key-value pair, or replace the value of an existing key
         *
         * If a \c MapEntry with the same key already exists its \c Value member
         * is replaced in-place without reallocating the node.  Otherwise a new
         * entry is inserted.
         *
         * \tparam TOtherKey   Type of the key argument (forwarded)
         * \tparam TOtherValue Type of the value argument (forwarded)
         *
         * \param key   Key to insert or update
         * \param value Value to associate with \p key
         */
        template <typename TOtherKey, typename TOtherValue>
        void Put(TOtherKey&& key, TOtherValue&& value)
        {
            Base::PutEntry(key, std::forward<TOtherKey>(key), std::forward<TOtherValue>(value));
        }

        /**
         * \brief Insert a key-value pair only when the key is not already present
         *
         * If a \c MapEntry with the same key already exists, this is a no-op.
         *
         * \tparam TOtherKey   Type of the key argument (forwarded)
         * \tparam TOtherValue Type of the value argument (forwarded)
         *
         * \param key   Key to check and potentially insert
         * \param value Value to insert when the key is absent
         *
         * \return \c true if a new entry was inserted, \c false if the key was
         *         already present
         */
        template <typename TOtherKey, typename TOtherValue>
        bool PutIfMissing(TOtherKey&& key, TOtherValue&& value)
        {
            return Base::PutEntryIfMissing(key, std::forward<TOtherKey>(key), std::forward<TOtherValue>(value));
        }

        /**
         * \brief Return the value for \p key, inserting a computed value when absent
         *
         * When the key is not present, \p function is called with \p key to produce
         * the value to insert.  The newly inserted value is returned.
         * When the key already exists its current value is returned and \p function
         * is never called.
         *
         * \tparam TOtherKey Type of the key (forwarded)
         * \tparam F         Callable with signature \c ValueType(const KeyType&)
         *
         * \param key      Key to look up or create
         * \param function Factory called with \p key when the entry is absent
         *
         * \return Mutable reference to the existing or newly created value
         *
         * \warning If the insertion triggers a rehash the internal node address
         *          changes; the returned reference remains valid through a
         *          post-rehash lookup.
         */
        template <typename TOtherKey, typename F>
        ValueType& ComputeIfMissing(TOtherKey&& key, F&& function)
        {
            Optional<Node&> existing = Base::FindNode(static_cast<const KeyType&>(key));
            if (!existing.IsEmpty())
                return existing.Get().Value;

            FunctionReturnType<F> value = function(key);
            std::size_t hashCode;
            auto& bucket = Base::GetBucket(key, hashCode);
            Node& node = bucket.Emplace(hashCode, std::forward<TOtherKey>(key), std::move(value));
            ++this->mSize;
            if (this->mSize > this->mBucketCount * 3 / 4)
            {
                Base::Reserve(this->mSize);
                return Base::FindNode(static_cast<const KeyType&>(key)).Get().Value;
            }
            return node.Value;
        }

        /**
         * \brief Replace the value of an existing entry without inserting
         *
         * Locates the entry for \p key.  If found its value is updated to
         * \p value.  If the key is absent, the map is left unchanged.
         *
         * \tparam TOtherKey   Type of the key argument
         * \tparam TOtherValue Type of the replacement value (forwarded)
         *
         * \param key   Key of the entry to update
         * \param value New value to assign
         *
         * \return \c true if the key was found and the value was updated,
         *         \c false if the key does not exist
         */
        template <typename TOtherKey, typename TOtherValue>
        bool Replace(TOtherKey&& key, TOtherValue&& value)
        {
            return !Base::FindNode(static_cast<const KeyType&>(key))
                        .Then([&value](Node& node) {
                            node.Value = std::forward<TOtherValue>(value);
                        })
                        .IsEmpty();
        }

        // -----------------------------------------------------------------------
        // Lookup
        // -----------------------------------------------------------------------

        /**
         * \brief Return a pointer to the value for \p key, or \c nullptr if absent
         *
         * \param key Key to look up
         *
         * \return Pointer to the associated value, or \c nullptr if not found
         */
        ValueType* Get(const KeyType& key)
        {
            return TryGet(key)
                .Map([](ValueType& v) -> ValueType* {
                    return &v;
                })
                .GetOr(nullptr);
        }

        /**
         * \brief Return a const pointer to the value for \p key, or \c nullptr if absent
         *
         * \param key Key to look up
         *
         * \return Const pointer to the associated value, or \c nullptr if not found
         */
        const ValueType* Get(const KeyType& key) const
        {
            return TryGet(key)
                .Map([](const ValueType& v) -> const ValueType* {
                    return &v;
                })
                .GetOr(nullptr);
        }

        /**
         * \brief Return an \c Optional mutable reference to the value for \p key
         *
         * \tparam TOtherKey Type of the key argument
         *
         * \param key Key to look up
         *
         * \return \c Optional<ValueType&> containing the value if found, empty otherwise
         */
        template <typename TOtherKey>
        Optional<ValueType&> TryGet(TOtherKey&& key)
        {
            return Base::FindNode(static_cast<const KeyType&>(key)).Map([](Node& node) -> ValueType& {
                return node.Value;
            });
        }

        /**
         * \brief Return an \c Optional const reference to the value for \p key
         *
         * \tparam TOtherKey Type of the key argument
         *
         * \param key Key to look up
         *
         * \return \c Optional<const ValueType&> containing the value if found, empty otherwise
         */
        template <typename TOtherKey>
        Optional<const ValueType&> TryGet(TOtherKey&& key) const
        {
            return Base::FindNode(static_cast<const KeyType&>(key)).Map([](const Node& node) -> const ValueType& {
                return node.Value;
            });
        }

        /**
         * \brief Return the value for \p key, inserting a default-constructed value when absent
         *
         * If the key does not exist a new entry is inserted with a
         * default-constructed \c ValueType and a reference to it is returned.
         * This mirrors the behaviour of \c std::unordered_map::operator[].
         *
         * \tparam TOtherKey Type of the key argument (forwarded)
         *
         * \param key Key to look up or create
         *
         * \return Mutable reference to the existing or newly created value
         *
         * \warning If the insertion triggers a rehash the internal node address
         *          changes; the returned reference remains valid through a
         *          post-rehash lookup.
         */
        template <typename TOtherKey>
        ValueType& GetOrAdd(TOtherKey&& key)
        {
            Optional<Node&> existing = Base::FindNode(static_cast<const KeyType&>(key));
            if (!existing.IsEmpty())
                return existing.Get().Value;

            std::size_t hashCode;
            auto& bucket = Base::GetBucket(key, hashCode);
            Node& node = bucket.Emplace(hashCode, std::forward<TOtherKey>(key), ValueType());
            ++this->mSize;
            if (this->mSize > this->mBucketCount * 3 / 4)
            {
                Base::Reserve(this->mSize);
                return Base::FindNode(static_cast<const KeyType&>(key)).Get().Value;
            }
            return node.Value;
        }

        /**
         * \brief Check whether \p key exists in the map
         *
         * \tparam TOtherKey Type of the key argument
         *
         * \param key Key to search for
         *
         * \return \c true if an entry with \p key exists, \c false otherwise
         */
        template <typename TOtherKey>
        bool Contains(TOtherKey&& key) const
        {
            return !Base::FindNode(static_cast<const KeyType&>(key)).IsEmpty();
        }

        // -----------------------------------------------------------------------
        // Removal
        // -----------------------------------------------------------------------

        /**
         * \brief Remove the entry for \p key
         *
         * \tparam TOtherKey Type of the key argument
         *
         * \param key Key of the entry to remove
         *
         * \return \c true if an entry was found and removed, \c false otherwise
         */
        template <typename TOtherKey>
        bool Remove(TOtherKey&& key)
        {
            return Base::RemoveEntry(static_cast<const KeyType&>(key));
        }

        // -----------------------------------------------------------------------
        // Iteration helpers
        // -----------------------------------------------------------------------

        /**
         * \brief Apply \p function to every entry in the map
         *
         * The callable receives either:
         * - no arguments (arity 0),
         * - the value only \c (const ValueType&) (arity 1), or
         * - the value and the key \c (const ValueType&, const KeyType&) (arity 2).
         *
         * \tparam TFunction Callable type; arity must be 0, 1, or 2
         *
         * \param function The function to apply to each entry
         *
         * \warning Do not insert or remove entries during iteration — doing so
         *          may trigger a rehash and invalidate the internal iterators.
         */
        template <typename TFunction>
        void ForEach(TFunction function) const
        {
            static_assert(FunctionTraits<TFunction>::Arity <= 2, "ForEach function accepts 0, 1, or 2 parameters");

            for (SizeType i = 0; i < this->mBucketCount; i++)
            {
                Detail::LinkedListConstIterator<Node, AllocatorType> it(this->mBuckets[i]);
                while (it.IsValid())
                {
                    if constexpr (FunctionTraits<TFunction>::Arity == 0)
                        function();
                    else if constexpr (FunctionTraits<TFunction>::Arity == 1)
                        function(it->Value);
                    else
                        function(it->Value, it->Key);

                    it.Advance();
                }
            }
        }

        /**
         * \brief Return a mutable iterator positioned at the first entry
         *
         * \return Iterator at the beginning of the map
         */
        Iterator GetIterator()
        {
            return Iterator(*this);
        }

        /**
         * \brief Return a read-only iterator positioned at the first entry
         *
         * \return Const iterator at the beginning of the map
         */
        ConstIterator GetIterator() const
        {
            return ConstIterator(*this);
        }

        /**
         * \brief Return a mutable iterator positioned at the last entry
         *
         * \return Iterator at the last valid entry of the map
         */
        Iterator GetLastIterator()
        {
            Iterator it(*this, this->mBucketCount);
            it.Rewind();
            return it;
        }

        /**
         * \brief Return a read-only iterator positioned at the last entry
         *
         * \return Const iterator at the last valid entry of the map
         */
        ConstIterator GetLastIterator() const
        {
            ConstIterator it(*this, this->mBucketCount);
            it.Rewind();
            return it;
        }

        // -----------------------------------------------------------------------
        // Bulk operations
        // -----------------------------------------------------------------------

        /**
         * \brief Collect all keys stored in the map into a \c HashSet
         *
         * \return HashSet containing a copy of every key in the map
         */
        HashSet<ValueType, THash, TEqual, AllocatorType> GetKeys() const
        {
            HashSet<ValueType, THash, TEqual, AllocatorType> keys;
            keys.Reserve(this->mSize);
            ForEach([&keys](const ValueType& v, const KeyType& k) {
                keys.Add(k);
            });
            return keys;
        }

        /**
         * \brief Collect all values stored in the map into an \c Array
         *
         * The order of values in the returned array matches the iteration order
         * of the map (bucket order).
         *
         * \return Array containing a copy of every value in the map
         */
        Array<ValueType, AllocatorType> GetValues() const
        {
            Array<ValueType, AllocatorType> values;
            values.Reserve(this->mSize);
            ForEach([&values](const ValueType& v) {
                values.Add(v);
            });
            return values;
        }

        using Base::Clear;
        using Base::GetBucketCount;
        using Base::GetSize;
        using Base::IsEmpty;
        using Base::Reserve;

        /**
         * \brief Access or insert the value for \p key
         *
         * Equivalent to \c GetOrAdd(key).  If the key does not exist a
         * default-constructed value is inserted and returned.
         *
         * \param key Key to access
         *
         * \return Mutable reference to the associated value
         */
        ValueType& operator[](const KeyType& key)
        {
            return GetOrAdd(key);
        }
    };

    /// \name ADL begin/end support for range-based for loops
    /// \{

    /** \brief Returns a mutable iterator to the first entry of \p m */
    template <typename K, typename V, typename H, typename E, typename A>
    typename HashMap<K, V, H, E, A>::Iterator begin(HashMap<K, V, H, E, A>& m)
    {
        return m.GetIterator();
    }

    /** \brief Returns a read-only iterator to the first entry of \p m */
    template <typename K, typename V, typename H, typename E, typename A>
    typename HashMap<K, V, H, E, A>::ConstIterator begin(const HashMap<K, V, H, E, A>& m)
    {
        return m.GetIterator();
    }

    /** \brief Returns a mutable past-the-end iterator for \p m */
    template <typename K, typename V, typename H, typename E, typename A>
    typename HashMap<K, V, H, E, A>::Iterator end(HashMap<K, V, H, E, A>& m)
    {
        return typename HashMap<K, V, H, E, A>::Iterator(m, m.GetBucketCount());
    }

    /** \brief Returns a read-only past-the-end iterator for \p m */
    template <typename K, typename V, typename H, typename E, typename A>
    typename HashMap<K, V, H, E, A>::ConstIterator end(const HashMap<K, V, H, E, A>& m)
    {
        return typename HashMap<K, V, H, E, A>::ConstIterator(m, m.GetBucketCount());
    }

    /// \}
}

#endif // COCKTAIL_CORE_HASHMAP_HPP
