#ifndef COCKTAIL_CORE_HASHSET_HPP
#define COCKTAIL_CORE_HASHSET_HPP

#include <Cocktail/Core/HashTable.hpp>

namespace Ck
{
    // -----------------------------------------------------------------------
    // Entry policy for HashSet
    // -----------------------------------------------------------------------

    namespace Detail
    {
        /**
         * \brief Entry policy where the element itself is the key
         *
         * A HashSet stores unique elements; the key used for lookup is the
         * element itself. ReplaceValue is a no-op because the "value" is the
         * key — duplicate insertions are simply ignored.
         */
        template <typename TElement>
        struct SetEntryPolicy
        {
            /**
             * \brief The type stored per entry
             *
             * We wrap the element in a trivial struct so that HashTableBase can
             * call the same EntryType(args...) constructor pattern used by
             * MapEntry, while still providing a natural operator* on the
             * iterator.
             */
            struct EntryType
            {
                /**
                 * \brief The type used to look up this entry (the element itself)
                 */
                using KeyType = TElement;

                /**
                 * \brief Default constructor — value-initialises \c Element
                 */
                EntryType() = default;

                /**
                 * \brief Construct from a const lvalue element
                 *
                 * \param element Element to copy into this entry
                 */
                explicit EntryType(const TElement& element) :
                    Element(element)
                {
                    /// Nothing
                }

                /**
                 * \brief Construct from an rvalue element
                 *
                 * \param element Element to move into this entry
                 */
                explicit EntryType(TElement&& element) :
                    Element(std::move(element))
                {
                    /// Nothing
                }

                /**
                 * \brief The stored element — also serves as the lookup key
                 */
                TElement Element;
            };

            /**
             * \brief The type used to look up entries (the element type itself)
             */
            using KeyType = TElement;

            /**
             * \brief Extract the lookup key from a stored entry
             *
             * For a set the key is the element itself.
             *
             * \param entry The entry to extract the key from
             *
             * \return Const reference to the stored element
             */
            static const KeyType& GetKey(const EntryType& entry)
            {
                return entry.Element;
            }

            /**
             * \brief No-op: sets do not have a separate value to replace
             */
            template <typename TOtherElement>
            static void ReplaceValue(EntryType& /*entry*/, TOtherElement&& /*element*/)
            {
                // Duplicate key — nothing to update
            }
        };
    }

    /**
     * \brief Unordered set storing unique elements using a hash table
     *
     * HashSet provides average constant-time complexity for insertion,
     * lookup, and removal operations.
     *
     * \tparam TElement   Type of stored elements
     * \tparam THash      Hash function
     * \tparam TEqual     Equality predicate
     * \tparam TAllocator Allocator for internal storage
     */
    template <typename TElement, typename THash = std::hash<TElement>, typename TEqual = std::equal_to<TElement>, typename TAllocator = HeapAllocator>
    class HashSet : public Detail::HashTableBase<Detail::SetEntryPolicy<TElement>, THash, TEqual, TAllocator>
    {
        using Base = Detail::HashTableBase<Detail::SetEntryPolicy<TElement>, THash, TEqual, TAllocator>;
        using Node = typename Base::Node;
        using EntryType = typename Base::EntryType;

    public:

        /**
         * \brief Type of elements stored in the set
         */
        using ElementType = TElement;

        /**
         * \brief Allocator type used for internal storage
         */
        using AllocatorType = TAllocator;

        /**
         * \brief Type used for sizes and indices
         */
        using SizeType = typename TAllocator::SizeType;

        // Iterators expose the element directly (not the internal EntryType wrapper)
        // by using a small projection — we inherit from EntryType so the cast is valid.

        /**
         * \brief Mutable iterator: dereferences to TElement&
         *
         * Elements in a set are logically immutable (modifying an element would
         * break the hash invariant), so the mutable iterator still returns a
         * const reference. The type is provided for STL range compatibility.
         */
        class Iterator : public Detail::HashTableIterator<Base, EntryType>
        {
            using Inner = Detail::HashTableIterator<Base, EntryType>;

        public:

            /**
             * \brief Construct an iterator positioned at the first valid entry at or
             *        after bucket \p firstBucket
             *
             * \param owner       The set to iterate
             * \param firstBucket Index of the first bucket to consider (0 = beginning)
             */
            explicit Iterator(HashSet& owner, SizeType firstBucket = 0) :
                Inner(static_cast<Base&>(owner), firstBucket)
            {
                /// Nothing
            }

            /**
             * \brief Dereferences to the stored element
             *
             * \return Const reference to the current element
             */
            const TElement& operator*() const
            {
                return Inner::GetValue().Element;
            }

            /**
             * \brief Member-access through the iterator
             *
             * \return Const pointer to the current element
             */
            const TElement* operator->() const
            {
                return &Inner::GetValue().Element;
            }

            /**
             * \brief Get a const reference to the current element
             *
             * \return Const reference to the current element
             */
            const TElement& GetValue() const
            {
                return Inner::GetValue().Element;
            }

            /**
             * \brief Pre-increment — advance to the next element
             *
             * \return Reference to \c *this after advancing
             */
            Iterator& operator++()
            {
                Inner::operator++();
                return *this;
            }

            /**
             * \brief Pre-decrement — retreat to the previous element
             *
             * \return Reference to \c *this after retreating
             */
            Iterator& operator--()
            {
                Inner::operator--();
                return *this;
            }

            /**
             * \brief Equality comparison
             */
            bool operator==(const Iterator& other) const
            {
                return Inner::operator==(other);
            }

            /**
             * \brief Inequality comparison
             */
            bool operator!=(const Iterator& other) const
            {
                return !(*this == other);
            }
        };

        /**
         * \brief Read-only forward iterator over a \c HashSet
         *
         * Semantically identical to \c Iterator.  Can be implicitly constructed
         * from a mutable \c Iterator.
         */
        class ConstIterator : public Detail::HashTableConstIterator<Base, EntryType>
        {
            using Inner = Detail::HashTableConstIterator<Base, EntryType>;

        public:

            /**
             * \brief Construct a const iterator positioned at the first valid entry
             *        at or after bucket \p firstBucket
             *
             * \param owner       The set to iterate (const reference)
             * \param firstBucket Index of the first bucket to consider (0 = beginning)
             */
            explicit ConstIterator(const HashSet& owner, SizeType firstBucket = 0) :
                Inner(static_cast<const Base&>(owner), firstBucket)
            {
                /// Nothing
            }

            /**
             * \brief Implicit conversion from a mutable iterator
             *
             * \param other The mutable iterator to copy state from
             */
            ConstIterator(Iterator other) :
                Inner(other)
            {
                /// Nothing
            }

            /**
             * \brief Dereferences to the stored element
             *
             * \return Const reference to the current element
             */
            const TElement& operator*() const
            {
                return Inner::GetValue().Element;
            }

            /**
             * \brief Member-access through the iterator
             *
             * \return Const pointer to the current element
             */
            const TElement* operator->() const
            {
                return &Inner::GetValue().Element;
            }

            /**
             * \brief Get a const reference to the current element
             *
             * \return Const reference to the current element
             */
            const TElement& GetValue() const
            {
                return Inner::GetValue().Element;
            }

            /**
             * \brief Pre-increment — advance to the next element
             *
             * \return Reference to \c *this after advancing
             */
            ConstIterator& operator++()
            {
                Inner::operator++();
                return *this;
            }

            /**
             * \brief Pre-decrement — retreat to the previous element
             *
             * \return Reference to \c *this after retreating
             */
            ConstIterator& operator--()
            {
                Inner::operator--();
                return *this;
            }

            /**
             * \brief Equality comparison
             */
            bool operator==(const ConstIterator& other) const
            {
                return Inner::operator==(other);
            }

            /**
             * \brief Inequality comparison
             */
            bool operator!=(const ConstIterator& other) const
            {
                return !(*this == other);
            }
        };

        // -----------------------------------------------------------------------
        // Construction / assignment
        // -----------------------------------------------------------------------

        /**
         * \brief Constructs an empty set with the specified initial bucket count
         *
         * The actual bucket count is rounded up to the next power of two,
         * with a minimum of 8 buckets.
         *
         * \param bucketCount Desired initial bucket count
         */
        explicit HashSet(SizeType bucketCount = 0) :
            Base(bucketCount)
        {
            /// Nothing
        }

        /**
         * \brief Copy constructor
         *
         * Deep-copies all elements from \p other.
         *
         * \param other Set to copy from
         */
        HashSet(const HashSet& other) :
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
         * \param other Set to move from
         */
        HashSet(HashSet&& other) noexcept :
            Base(std::move(other))
        {
            /// Nothing
        }

        /**
         * \brief Constructs the set from an initializer list
         *
         * Duplicate values in \p values are silently ignored.
         *
         * \param values Initializer list of elements to insert
         */
        HashSet(std::initializer_list<TElement> values) :
            Base()
        {
            Reserve(static_cast<SizeType>(values.size()));
            for (const TElement& element : values)
                Add(element);
        }

        /**
         * \brief Copy assignment operator
         *
         * Clears this set, then deep-copies all elements from \p other.
         *
         * \param other Set to copy from
         *
         * \return Reference to \c *this
         */
        HashSet& operator=(const HashSet& other)
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
         * \param other Set to move from
         *
         * \return Reference to \c *this
         */
        HashSet& operator=(HashSet&& other) noexcept
        {
            Base::operator=(std::move(other));
            return *this;
        }

        // -----------------------------------------------------------------------
        // Insertion
        // -----------------------------------------------------------------------

        /**
         * \brief Insert an element into the set
         *
         * If the element is already present, this is a no-op.
         *
         * \param element The element to insert
         *
         * \return True if the element was inserted, false if it was already present
         */
        bool Add(const TElement& element)
        {
            return Base::PutEntryIfMissing(element, element);
        }

        /**
         * \brief Insert an element into the set using move semantics
         *
         * \param element The element to insert
         *
         * \return True if the element was inserted, false if it was already present
         */
        bool Add(TElement&& element)
        {
            // We need the key for lookup, but we also want to move the value.
            // Take a const reference to the key before moving.
            const TElement& key = element;
            return Base::PutEntryIfMissing(key, std::move(element));
        }

        /**
         * \brief Construct an element in-place inside the set
         *
         * The element is constructed from \p args. If the resulting value is
         * already present in the set, the construction is discarded.
         *
         * \return True if the element was inserted
         */
        template <typename... TArgs>
        bool Emplace(TArgs&&... args)
        {
            TElement element(std::forward<TArgs>(args)...);
            return Add(std::move(element));
        }

        // -----------------------------------------------------------------------
        // Lookup
        // -----------------------------------------------------------------------

        /**
         * \brief Check whether \p element exists in the set
         *
         * \return True if found, false otherwise
         */
        bool Contains(const TElement& element) const
        {
            return !Base::FindNode(element).IsEmpty();
        }

        // -----------------------------------------------------------------------
        // Removal
        // -----------------------------------------------------------------------

        /**
         * \brief Remove \p element from the set
         *
         * \return True if the element was present and removed
         */
        bool Remove(const TElement& element)
        {
            return Base::RemoveEntry(element);
        }

        // -----------------------------------------------------------------------
        // Set operations
        // -----------------------------------------------------------------------

        /**
         * \brief Returns a new set that is the union of this set and \p other
         *
         * All elements from both sets are included. Duplicates are deduplicated.
         *
         * \param other Set to merge with
         *
         * \return New set containing all elements of both operands
         */
        HashSet Union(const HashSet& other) const
        {
            HashSet result(*this);
            ConstIterator it(other);
            while (it.IsValid())
            {
                result.Add(it.GetValue());
                it.Advance();
            }
            return result;
        }

        /**
         * \brief Returns a new set that is the intersection of this set and \p other
         *
         * Only elements present in both sets are included.
         *
         * \param other Set to intersect with
         *
         * \return New set containing only elements present in both operands
         */
        HashSet Intersection(const HashSet& other) const
        {
            HashSet result;
            ConstIterator it(*this);
            while (it.IsValid())
            {
                if (other.Contains(it.GetValue()))
                    result.Add(it.GetValue());
                it.Advance();
            }
            return result;
        }

        /**
         * \brief Returns a new set that is the difference of this set minus \p other
         *
         * Only elements present in this set but absent from \p other are included.
         *
         * \param other Set to subtract
         *
         * \return New set containing elements in this set that are not in \p other
         */
        HashSet Difference(const HashSet& other) const
        {
            HashSet result;
            ConstIterator it(*this);
            while (it.IsValid())
            {
                if (!other.Contains(it.GetValue()))
                    result.Add(it.GetValue());
                it.Advance();
            }
            return result;
        }

        /**
         * \brief Applies \p function to every element in the set
         *
         * The callable must accept exactly one parameter of type \c const TElement&.
         *
         * \tparam TFunction Callable with signature \c void(const TElement&)
         *
         * \param function The function to apply to each element
         *
         * \warning Do not insert or remove entries during iteration — doing so
         *          may trigger a rehash and invalidate the internal iterators.
         */
        template <typename TFunction>
        void ForEach(TFunction function) const
        {
            static_assert(FunctionTraits<TFunction>::Arity == 1, "ForEach function must accept exactly 1 parameter");

            for (SizeType i = 0; i < this->mBucketCount; i++)
            {
                Detail::LinkedListConstIterator<Node, AllocatorType> it(this->mBuckets[i]);
                while (it.IsValid())
                {
                    function(it->Element);
                    it.Advance();
                }
            }
        }

        // -----------------------------------------------------------------------
        // Iteration
        // -----------------------------------------------------------------------

        /**
         * \brief Return a mutable iterator positioned at the first element
         *
         * \return Iterator at the beginning of the set
         */
        Iterator GetIterator()
        {
            return Iterator(*this);
        }

        /**
         * \brief Return a read-only iterator positioned at the first element
         *
         * \return Const iterator at the beginning of the set
         */
        ConstIterator GetIterator() const
        {
            return ConstIterator(*this);
        }

        /**
         * \brief Return a mutable iterator positioned at the last element
         *
         * \return Iterator at the last valid element of the set
         */
        Iterator GetLastIterator()
        {
            Iterator it(*this, this->mBucketCount);
            it.Rewind();
            return it;
        }

        /**
         * \brief Return a read-only iterator positioned at the last element
         *
         * \return Const iterator at the last valid element of the set
         */
        ConstIterator GetLastIterator() const
        {
            ConstIterator it(*this, this->mBucketCount);
            it.Rewind();
            return it;
        }

        using Base::Clear;
        using Base::GetBucketCount;
        using Base::GetSize;
        using Base::IsEmpty;
        using Base::Reserve;
    };

    /// \name ADL begin/end support for range-based for loops
    /// \{

    /** \brief Returns a mutable iterator to the first element of \p s */
    template <typename E, typename H, typename Eq, typename A>
    typename HashSet<E, H, Eq, A>::Iterator begin(HashSet<E, H, Eq, A>& s)
    {
        return s.GetIterator();
    }

    /** \brief Returns a read-only iterator to the first element of \p s */
    template <typename E, typename H, typename Eq, typename A>
    typename HashSet<E, H, Eq, A>::ConstIterator begin(const HashSet<E, H, Eq, A>& s)
    {
        return s.GetIterator();
    }

    /** \brief Returns a mutable past-the-end iterator for \p s */
    template <typename E, typename H, typename Eq, typename A>
    typename HashSet<E, H, Eq, A>::Iterator end(HashSet<E, H, Eq, A>& s)
    {
        return typename HashSet<E, H, Eq, A>::Iterator(s, s.GetBucketCount());
    }

    /** \brief Returns a read-only past-the-end iterator for \p s */
    template <typename E, typename H, typename Eq, typename A>
    typename HashSet<E, H, Eq, A>::ConstIterator end(const HashSet<E, H, Eq, A>& s)
    {
        return typename HashSet<E, H, Eq, A>::ConstIterator(s, s.GetBucketCount());
    }

    /// \}
}

#endif // COCKTAIL_CORE_HASHSET_HPP
