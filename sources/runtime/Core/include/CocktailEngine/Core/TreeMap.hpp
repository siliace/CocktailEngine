#ifndef COCKTAILENGINE_CORE_TREEMAP_HPP
#define COCKTAILENGINE_CORE_TREEMAP_HPP

#include <functional>
#include <initializer_list>
#include <utility>

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Memory/Allocator/SizedHeapAllocator.hpp>
#include <CocktailEngine/Core/TreeSet.hpp>
#include <CocktailEngine/Core/Utility/Detail/RedBlackTree.hpp>
#include <CocktailEngine/Core/Utility/FunctionTraits.hpp>
#include <CocktailEngine/Core/Utility/MapEntry.hpp>

namespace Ck
{
    namespace Detail
    {
        /**
         * \brief Entry policy that stores \c MapEntry<TKey, TValue> nodes
         *
         * This policy is used by \c RedBlackTree to handle the TreeMap-specific
         * semantics: keys are stored together with their values, and a \c Put on an
         * already present key replaces the existing value in place.
         *
         * \tparam TKey   Key type
         * \tparam TValue Value type
         */
        template <typename TKey, typename TValue>
        struct TreeMapEntryPolicy
        {
            /**
             * \brief The type stored in each node
             */
            using EntryType = MapEntry<TKey, TValue>;

            /**
             * \brief The type used to order and look up entries
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
             * Called when an insertion lands on an already present key. The key is
             * left untouched — rewriting it would move the entry in the ordering —
             * so only the value is replaced.
             *
             * \tparam TOtherKey   Forwarded key argument (unused, the key is kept)
             * \tparam TOtherValue Forwarded replacement value
             *
             * \param entry Entry whose value should be updated
             * \param value New value to assign
             */
            template <typename TOtherKey, typename TOtherValue>
            static void ReplaceValue(EntryType& entry, TOtherKey&& /*key*/, TOtherValue&& value)
            {
                ObjectMemoryUtils::Destroy(&entry.Value);
                ObjectMemoryUtils::Construct(&entry.Value, Forward<TOtherValue>(value));
            }
        };
    }

    /**
     * \brief Associative container storing key-value pairs in a red-black tree
     *
     * \c TreeMap keeps its entries sorted by key at all times, which is the
     * difference that matters against \c HashMap: iteration is in ascending key
     * order rather than bucket order, and the container answers range queries such
     * as \c Floor, \c Ceiling, \c LowerBound or \c GetBounds. The price is
     * logarithmic rather than constant-time lookup, and the key type needs an
     * ordering instead of a hash.
     *
     * \par Complexity
     * \c Put, \c Get and \c Remove are O(log n) in the worst case. Iterating the
     * whole map is O(n).
     *
     * \par Iterator stability
     * Entries live in individually allocated nodes and removal splices nodes
     * instead of moving entries, so an insertion or a removal never invalidates
     * references, pointers or iterators to the other entries. Only the removed
     * entry is invalidated. This is a stronger guarantee than \c HashMap offers,
     * where a rehash relocates every entry.
     *
     * \par Thread safety
     * This class is not thread-safe. External synchronisation is required when
     * concurrent reads and writes are needed.
     *
     * \code
     * TreeMap<int, String> levels;
     * levels.Put(30, "hard");
     * levels.Put(10, "easy");
     * levels.Put(20, "normal");
     *
     * for (const auto& [score, name] : levels)
     *     Log(score, name); // 10 easy, 20 normal, 30 hard
     *
     * // Hardest level reachable with 25 points
     * Optional<const MapEntry<int, String>&> entry = levels.Floor(25);
     * \endcode
     *
     * \tparam TKey        Type of the keys
     * \tparam TValue      Type of the associated values
     * \tparam TComparator Strict weak ordering over \c TKey
     * \tparam TAllocator  Allocator type used for node memory
     */
    template <typename TKey, typename TValue, typename TComparator = std::less<TKey>, typename TAllocator = HeapAllocator>
    class TreeMap : public Detail::RedBlackTree<Detail::TreeMapEntryPolicy<TKey, TValue>, TComparator, TAllocator>
    {
        using Base = Detail::RedBlackTree<Detail::TreeMapEntryPolicy<TKey, TValue>, TComparator, TAllocator>;
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
         * \brief Type of the key-value pair exposed by the iterators
         */
        using EntryType = MapEntry<TKey, TValue>;

        /**
         * \brief Allocator type used for internal storage
         */
        using AllocatorType = TAllocator;

        /**
         * \brief Type used for sizes and counts
         */
        using SizeType = typename TAllocator::SizeType;

        /**
         * \brief Mutable iterator — dereferences to \c MapEntry<TKey, TValue>&
         *
         * \see Detail::RedBlackTreeIterator
         */
        using Iterator = Detail::RedBlackTreeIterator<Base, EntryType>;

        /**
         * \brief Read-only iterator — dereferences to \c const MapEntry<TKey, TValue>&
         *
         * \see Detail::RedBlackTreeConstIterator
         */
        using ConstIterator = Detail::RedBlackTreeConstIterator<Base, EntryType>;

        struct Bounds
        {
            Iterator Begin;
            Iterator End;
        };

        struct ConstBounds
        {
            ConstIterator Begin;
            ConstIterator End;
        };

        // -----------------------------------------------------------------------
        // Construction / assignment
        // -----------------------------------------------------------------------

        /**
         * \brief Constructs an empty map
         */
        TreeMap() :
            Base()
        {
            /// Nothing
        }

        /**
         * \brief Copy constructor
         *
         * Deep-copies all entries from \p other, reusing its tree shape.
         *
         * \param other Map to copy from
         */
        TreeMap(const TreeMap& other) :
            Base(other)
        {
            /// Nothing
        }

        /**
         * \brief Move constructor
         *
         * Transfers ownership of \p other's nodes.
         * \p other is left empty after the operation.
         *
         * \param other Map to move from
         */
        TreeMap(TreeMap&& other) noexcept :
            Base(Move(other))
        {
            /// Nothing
        }

        /**
         * \brief Constructs the map from an initializer list of entries
         *
         * Later entries with an already present key replace the earlier value.
         *
         * \param entries Initializer list of key-value pairs to insert
         */
        TreeMap(std::initializer_list<EntryType> entries) :
            Base()
        {
            for (const EntryType& entry : entries)
                Put(entry.Key, entry.Value);
        }

        /**
         * \brief Copy assignment operator
         *
         * \param other Map to copy from
         *
         * \return Reference to \c *this
         */
        TreeMap& operator=(const TreeMap& other)
        {
            Base::operator=(other);
            return *this;
        }

        /**
         * \brief Move assignment operator
         *
         * \p other is left empty after the operation.
         *
         * \param other Map to move from
         *
         * \return Reference to \c *this
         */
        TreeMap& operator=(TreeMap&& other) noexcept
        {
            Base::operator=(Move(other));
            return *this;
        }

        // -----------------------------------------------------------------------
        // Insertion
        // -----------------------------------------------------------------------

        /**
         * \brief Insert a new key-value pair, or replace the value of an existing key
         *
         * If an entry with an equivalent key already exists its \c Value member is
         * assigned in place and the node is kept, so references to that entry stay
         * valid. Otherwise a new entry is inserted.
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
            Base::PutEntry(key, Forward<TOtherKey>(key), Forward<TOtherValue>(value));
        }

        /**
         * \brief Insert a key-value pair only when the key is not already present
         *
         * If an entry with an equivalent key already exists, this is a no-op.
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
            return Base::PutEntryIfMissing(key, Forward<TOtherKey>(key), Forward<TOtherValue>(value));
        }

        /**
         * \brief Return the value for \p key, inserting a computed value when absent
         *
         * When the key is not present, \p function is called with \p key to produce
         * the value to insert, and the newly inserted value is returned. When the
         * key already exists its current value is returned and \p function is never
         * called.
         *
         * \tparam TOtherKey Type of the key (forwarded)
         * \tparam F         Callable with signature \c ValueType(const KeyType&)
         *
         * \param key      Key to look up or create
         * \param function Factory called with \p key when the entry is absent
         *
         * \return Mutable reference to the existing or newly created value
         */
        template <typename TOtherKey, typename F>
        ValueType& ComputeIfMissing(TOtherKey&& key, F&& function)
        {
            Optional<Node&> existing = Base::FindNode(static_cast<const KeyType&>(key));
            if (!existing.IsEmpty())
                return existing.Get().Value;

            FunctionReturnType<F> value = function(key);
            return Base::EmplaceEntry(static_cast<const KeyType&>(key), Forward<TOtherKey>(key), Move(value)).Value;
        }

        /**
         * \brief Replace the value of an existing entry without inserting
         *
         * \tparam TOtherValue Type of the replacement value (forwarded)
         *
         * \param key   Key of the entry to update
         * \param value New value to assign
         *
         * \return \c true if the key was found and the value was updated,
         *         \c false if the key does not exist
         */
        template <typename TOtherValue>
        bool Replace(const KeyType& key, TOtherValue&& value)
        {
            Optional<Node&> existing = Base::FindNode(key);
            if (existing.IsEmpty())
                return false;

            existing.Get().Value = Forward<TOtherValue>(value);
            return true;
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
            Optional<Node&> node = Base::FindNode(key);
            return node.IsEmpty() ? nullptr : &node.Get().Value;
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
            Optional<const Node&> node = Base::FindNode(key);
            return node.IsEmpty() ? nullptr : &node.Get().Value;
        }

        /**
         * \brief Return an \c Optional mutable reference to the value for \p key
         *
         * \param key Key to look up
         *
         * \return \c Optional<ValueType&> containing the value if found, empty otherwise
         */
        Optional<ValueType&> TryGet(const KeyType& key)
        {
            return Base::FindNode(key).Map([](Node& node) -> ValueType& {
                return node.Value;
            });
        }

        /**
         * \brief Return an \c Optional const reference to the value for \p key
         *
         * \param key Key to look up
         *
         * \return \c Optional<const ValueType&> containing the value if found, empty otherwise
         */
        Optional<const ValueType&> TryGet(const KeyType& key) const
        {
            return Base::FindNode(key).Map([](const Node& node) -> const ValueType& {
                return node.Value;
            });
        }

        /**
         * \brief Return the value for \p key, inserting a default-constructed value when absent
         *
         * Mirrors the behaviour of \c std::map::operator[]. Unlike \c HashMap, the
         * returned reference is not invalidated by subsequent insertions.
         *
         * \tparam TOtherKey Type of the key argument (forwarded)
         *
         * \param key Key to look up or create
         *
         * \return Mutable reference to the existing or newly created value
         */
        template <typename TOtherKey>
        ValueType& GetOrAdd(TOtherKey&& key)
        {
            Optional<Node&> existing = Base::FindNode(static_cast<const KeyType&>(key));
            if (!existing.IsEmpty())
                return existing.Get().Value;

            return Base::EmplaceEntry(static_cast<const KeyType&>(key), Forward<TOtherKey>(key), ValueType()).Value;
        }

        /**
         * \brief Check whether \p key exists in the map
         *
         * \param key Key to search for
         *
         * \return \c true if an entry with \p key exists, \c false otherwise
         */
        bool Contains(const KeyType& key) const
        {
            return !Base::FindNode(key).IsEmpty();
        }

        /**
         * \brief Return an iterator on the entry for \p key
         *
         * \param key Key to search for
         *
         * \return Iterator on the matching entry, or a past-the-end iterator
         */
        Iterator Find(const KeyType& key)
        {
            Optional<Node&> node = Base::FindNode(key);
            return Iterator(*this, node.IsEmpty() ? nullptr : &node.Get());
        }

        /**
         * \brief Return a read-only iterator on the entry for \p key
         *
         * \param key Key to search for
         *
         * \return Const iterator on the matching entry, or a past-the-end iterator
         */
        ConstIterator Find(const KeyType& key) const
        {
            Optional<const Node&> node = Base::FindNode(key);
            return ConstIterator(*this, node.IsEmpty() ? nullptr : const_cast<Node*>(&node.Get()));
        }

        // -----------------------------------------------------------------------
        // Ordered queries
        // -----------------------------------------------------------------------

        /**
         * \brief Returns the entry holding the smallest key
         *
         * \return \c Optional reference to the first entry, empty when the map is empty
         */
        Optional<EntryType&> GetFirst()
        {
            Node* node = Base::GetFirstNode();
            return node != nullptr ? Optional<EntryType&>::Of(*node) : Optional<EntryType&>::Empty();
        }

        /**
         * \brief Returns the entry holding the smallest key
         *
         * \return \c Optional const reference to the first entry, empty when the map is empty
         */
        Optional<const EntryType&> GetFirst() const
        {
            Node* node = Base::GetFirstNode();
            return node != nullptr ? Optional<const EntryType&>::Of(*node) : Optional<const EntryType&>::Empty();
        }

        /**
         * \brief Returns the entry holding the largest key
         *
         * \return \c Optional reference to the last entry, empty when the map is empty
         */
        Optional<EntryType&> GetLast()
        {
            Node* node = Base::GetLastNode();
            return node != nullptr ? Optional<EntryType&>::Of(*node) : Optional<EntryType&>::Empty();
        }

        /**
         * \brief Returns the entry holding the largest key
         *
         * \return \c Optional const reference to the last entry, empty when the map is empty
         */
        Optional<const EntryType&> GetLast() const
        {
            Node* node = Base::GetLastNode();
            return node != nullptr ? Optional<const EntryType&>::Of(*node) : Optional<const EntryType&>::Empty();
        }

        /**
         * \brief Returns the entry with the largest key that does not order after \p key
         *
         * \param key Key to floor
         *
         * \return \c Optional reference to the matching entry, empty when every key
         *         orders after \p key
         */
        Optional<EntryType&> Floor(const KeyType& key)
        {
            Node* node = Base::FloorNode(key);
            return node != nullptr ? Optional<EntryType&>::Of(*node) : Optional<EntryType&>::Empty();
        }

        /**
         * \brief Returns the entry with the largest key that does not order after \p key
         *
         * \param key Key to floor
         *
         * \return \c Optional const reference to the matching entry, empty when
         *         every key orders after \p key
         */
        Optional<const EntryType&> Floor(const KeyType& key) const
        {
            Node* node = Base::FloorNode(key);
            return node != nullptr ? Optional<const EntryType&>::Of(*node) : Optional<const EntryType&>::Empty();
        }

        /**
         * \brief Returns the entry with the smallest key that does not order before \p key
         *
         * \param key Key to ceil
         *
         * \return \c Optional reference to the matching entry, empty when every key
         *         orders before \p key
         */
        Optional<EntryType&> Ceiling(const KeyType& key)
        {
            Node* node = Base::LowerBoundNode(key);
            return node != nullptr ? Optional<EntryType&>::Of(*node) : Optional<EntryType&>::Empty();
        }

        /**
         * \brief Returns the entry with the smallest key that does not order before \p key
         *
         * \param key Key to ceil
         *
         * \return \c Optional const reference to the matching entry, empty when
         *         every key orders before \p key
         */
        Optional<const EntryType&> Ceiling(const KeyType& key) const
        {
            Node* node = Base::LowerBoundNode(key);
            return node != nullptr ? Optional<const EntryType&>::Of(*node) : Optional<const EntryType&>::Empty();
        }

        /**
         * \brief Returns an iterator on the first entry whose key does not order
         *        before \p key
         *
         * \param key Key to position on
         *
         * \return Iterator on the matching entry, or a past-the-end iterator
         */
        Iterator LowerBound(const KeyType& key)
        {
            return Iterator(*this, Base::LowerBoundNode(key));
        }

        /**
         * \brief Returns a read-only iterator on the first entry whose key does not
         *        order before \p key
         *
         * \param key Key to position on
         *
         * \return Const iterator on the matching entry, or a past-the-end iterator
         */
        ConstIterator LowerBound(const KeyType& key) const
        {
            return ConstIterator(*this, Base::LowerBoundNode(key));
        }

        /**
         * \brief Returns an iterator on the first entry whose key orders after \p key
         *
         * \param key Key to position past
         *
         * \return Iterator on the matching entry, or a past-the-end iterator
         */
        Iterator UpperBound(const KeyType& key)
        {
            return Iterator(*this, Base::UpperBoundNode(key));
        }

        /**
         * \brief Returns a read-only iterator on the first entry whose key orders
         *        after \p key
         *
         * \param key Key to position past
         *
         * \return Const iterator on the matching entry, or a past-the-end iterator
         */
        ConstIterator UpperBound(const KeyType& key) const
        {
            return ConstIterator(*this, Base::UpperBoundNode(key));
        }

        /**
         * \brief Returns the iterator pair delimiting the inclusive key range
         *        [\p first, \p last]
         *
         * The range is half-open on the iterator side — the second iterator is one
         * past the last matching entry — so it can be walked with a plain loop.
         *
         * \param first Lower bound key, included
         * \param last  Upper bound key, included
         *
         * \return Iterator range covering every entry whose key lies between
         *         \p first and \p last
         */
        Bounds GetBounds(const KeyType& first, const KeyType& last)
        {
            return { LowerBound(first), UpperBound(last) };
        }

        /**
         * \brief Returns the read-only iterator pair delimiting the inclusive key
         *        range [\p first, \p last]
         *
         * \param first Lower bound key, included
         * \param last  Upper bound key, included
         *
         * \return Const iterator range covering every entry whose key lies between
         *         \p first and \p last
         */
        ConstBounds GetBounds(const KeyType& first, const KeyType& last) const
        {
            return { LowerBound(first), UpperBound(last) };
        }

        // -----------------------------------------------------------------------
        // Removal
        // -----------------------------------------------------------------------

        /**
         * \brief Remove the entry for \p key
         *
         * \param key Key of the entry to remove
         *
         * \return \c true if an entry was found and removed, \c false otherwise
         */
        bool Remove(const KeyType& key)
        {
            return Base::RemoveEntry(key);
        }

        /**
         * \brief Remove the entry an iterator points at
         *
         * Every other iterator into the map stays valid.
         *
         * \param position Iterator on the entry to remove, which must be valid
         *
         * \return Iterator on the entry that followed the removed one
         */
        Iterator Remove(const Iterator& position)
        {
            assert(position.IsValid());

            Node* node = position.GetNode();
            Node* next = Base::NextNode(node);
            Base::RemoveNode(node);

            return Iterator(*this, next);
        }

        /**
         * \brief Remove the entry holding the smallest key
         *
         * \return \c true if an entry was removed, \c false when the map is empty
         */
        bool RemoveFirst()
        {
            Node* node = Base::GetFirstNode();
            if (node == nullptr)
                return false;

            Base::RemoveNode(node);
            return true;
        }

        /**
         * \brief Remove the entry holding the largest key
         *
         * \return \c true if an entry was removed, \c false when the map is empty
         */
        bool RemoveLast()
        {
            Node* node = Base::GetLastNode();
            if (node == nullptr)
                return false;

            Base::RemoveNode(node);
            return true;
        }

        /**
         * \brief Move into this map every entry of \p other whose key is absent here
         *
         * Transferred entries are moved out of \p other and removed from it.
         * Entries whose key is already present in this map are left untouched in
         * \p other, which mirrors \c std::map::merge.
         *
         * \param other Map to take entries from
         */
        void Merge(TreeMap& other)
        {
            if (this == &other)
                return;

            for (Iterator it = other.GetIterator(); it.IsValid();)
            {
                const Iterator current = it;
                it.Advance();

                if (Contains(current->Key))
                    continue;

                Base::EmplaceEntry(current->Key, Move(current->Key), Move(current->Value));
                other.Remove(current);
            }
        }

        // -----------------------------------------------------------------------
        // Iteration
        // -----------------------------------------------------------------------

        /**
         * \brief Apply \p function to every entry, in ascending key order
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
         * \warning Do not insert or remove entries during iteration.
         */
        template <typename TFunction>
        void ForEach(TFunction function) const
        {
            static_assert(FunctionTraits<TFunction>::Arity <= 2, "ForEach function accepts 0, 1, or 2 parameters");

            for (Node* node = Base::GetFirstNode(); node != nullptr; node = Base::NextNode(node))
            {
                if constexpr (FunctionTraits<TFunction>::Arity == 0)
                    function();
                else if constexpr (FunctionTraits<TFunction>::Arity == 1)
                    function(node->Value);
                else
                    function(node->Value, node->Key);
            }
        }

        /**
         * \brief Return a mutable iterator positioned on the smallest key
         *
         * \return Iterator at the beginning of the map
         */
        Iterator GetIterator()
        {
            return Iterator(*this);
        }

        /**
         * \brief Return a read-only iterator positioned on the smallest key
         *
         * \return Const iterator at the beginning of the map
         */
        ConstIterator GetIterator() const
        {
            return ConstIterator(*this);
        }

        /**
         * \brief Return a mutable iterator positioned on the largest key
         *
         * \return Iterator on the last entry of the map
         */
        Iterator GetLastIterator()
        {
            return Iterator(*this, Base::GetLastNode());
        }

        /**
         * \brief Return a read-only iterator positioned on the largest key
         *
         * \return Const iterator on the last entry of the map
         */
        ConstIterator GetLastIterator() const
        {
            return ConstIterator(*this, Base::GetLastNode());
        }

        /**
         * \brief Return a mutable past-the-end iterator
         *
         * \return Iterator past the last entry of the map
         */
        Iterator GetEndIterator()
        {
            return Iterator(*this, nullptr);
        }

        /**
         * \brief Return a read-only past-the-end iterator
         *
         * \return Const iterator past the last entry of the map
         */
        ConstIterator GetEndIterator() const
        {
            return ConstIterator(*this, nullptr);
        }

        // -----------------------------------------------------------------------
        // Bulk operations
        // -----------------------------------------------------------------------

        /**
         * \brief Collect all keys stored in the map into a \c TreeSet
         *
         * \return TreeSet containing a copy of every key in the map
         */
        TreeSet<KeyType, TComparator, AllocatorType> GetKeys() const
        {
            TreeSet<KeyType, TComparator, AllocatorType> keys;
            ForEach([&keys](const ValueType& /*value*/, const KeyType& key) {
                keys.Add(key);
            });

            return keys;
        }

        /**
         * \brief Collect all values stored in the map into an \c Array
         *
         * Values appear in ascending key order.
         *
         * \return Array containing a copy of every value in the map
         */
        Array<ValueType, AllocatorType> GetValues() const
        {
            Array<ValueType, AllocatorType> values;
            values.Reserve(Base::GetSize());
            ForEach([&values](const ValueType& value) {
                values.Add(value);
            });

            return values;
        }

        using Base::Clear;
        using Base::GetSize;
        using Base::IsEmpty;

        /**
         * \brief Access or insert the value for \p key
         *
         * Equivalent to \c GetOrAdd(key). If the key does not exist a
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

    /** \brief Returns a mutable iterator to the first entry of \p m, in key order */
    template <typename K, typename V, typename C, typename A>
    typename TreeMap<K, V, C, A>::Iterator begin(TreeMap<K, V, C, A>& m)
    {
        return m.GetIterator();
    }

    /** \brief Returns a read-only iterator to the first entry of \p m, in key order */
    template <typename K, typename V, typename C, typename A>
    typename TreeMap<K, V, C, A>::ConstIterator begin(const TreeMap<K, V, C, A>& m)
    {
        return m.GetIterator();
    }

    /** \brief Returns a mutable past-the-end iterator for \p m */
    template <typename K, typename V, typename C, typename A>
    typename TreeMap<K, V, C, A>::Iterator end(TreeMap<K, V, C, A>& m)
    {
        return m.GetEndIterator();
    }

    /** \brief Returns a read-only past-the-end iterator for \p m */
    template <typename K, typename V, typename C, typename A>
    typename TreeMap<K, V, C, A>::ConstIterator end(const TreeMap<K, V, C, A>& m)
    {
        return m.GetEndIterator();
    }

    /// \}
}

#endif // COCKTAILENGINE_CORE_TREEMAP_HPP
