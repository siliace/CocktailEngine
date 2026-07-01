#ifndef COCKTAIL_CORE_HASHTABLE_HPP
#define COCKTAIL_CORE_HASHTABLE_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/LinkedList.hpp>

namespace Ck::Detail
{
    template <typename, typename>
    class HashTableIterator;

    template <typename, typename>
    class HashTableConstIterator;

    /**
     * \brief Low-level hash table implementation shared by HashMap and HashSet
     *
     * This class owns the bucket array and implements all structural operations:
     * insertion, lookup, removal, rehashing, and iteration. It is parameterized
     * by a \p TEntryPolicy that controls what is stored per entry and how the
     * lookup key is extracted from a stored entry.
     *
     * \par Entry Policy contract
     * \p TEntryPolicy must provide:
     * \code
     * // The type stored in each bucket node
     * using EntryType = ...;
     *
     * // The type used to look up an entry
     * using KeyType = ...;
     *
     * // Extract the lookup key from a stored entry
     * static const KeyType& GetKey(const EntryType&);
     *
     * // Update the non-key portion of an existing entry (no-op for sets)
     * template <typename... TArgs>
     * static void ReplaceValue(EntryType&, TArgs&&...);
     * \endcode
     *
     * \par Rehashing
     * A rehash is triggered automatically when the load factor exceeds 0.75.
     * The bucket count is always a power of two so that bucket selection
     * reduces to a bitmask operation: \c hashCode & (bucketCount - 1).
     *
     * \tparam TEntryPolicy  Policy describing the entry layout and key extraction
     * \tparam THash         Hash function for \c KeyType
     * \tparam TEqual        Equality comparator for \c KeyType
     * \tparam TAllocator    Allocator for bucket nodes
     */
    template <typename TEntryPolicy, typename THash, typename TEqual, typename TAllocator>
    class HashTableBase
    {
    public:

        /**
         * \brief The type stored in each bucket node, as defined by \p TEntryPolicy
         */
        using EntryType = typename TEntryPolicy::EntryType;

        /**
         * \brief The type used to look up entries, as defined by \p TEntryPolicy
         */
        using KeyType = typename TEntryPolicy::KeyType;

        /**
         * \brief Allocator type used by this hash table
         */
        using AllocatorType = TAllocator;

        /**
         * \brief Type used for sizes and bucket indices
         */
        using SizeType = typename TAllocator::SizeType;

    protected:

        template <typename, typename>
        friend class HashTableIterator;

        template <typename, typename>
        friend class HashTableConstIterator;

        // ---------------------------------------------------------------
        // Internal node stored in each bucket
        // ---------------------------------------------------------------

        /**
         * \brief Internal node type stored inside each bucket
         *
         * A \c Node inherits from \c EntryType so that the stored entry and the
         * cached hash code live in a single allocation.  Caching the hash code
         * avoids recomputing it during lookup and rehashing.
         */
        struct Node : EntryType
        {
            /**
             * \brief Default constructor — zero-initialises the hash code
             */
            Node() :
                HashCode(0)
            {
                /// Nothing
            }

            /**
             * \brief Forwarding constructor
             *
             * Constructs the \c EntryType portion by forwarding \p args, and
             * stores the pre-computed \p hashCode.
             *
             * \param hashCode Pre-computed hash code for the entry's key
             * \param args     Arguments forwarded to the \c EntryType constructor
             */
            template <typename... TArgs>
            explicit Node(std::size_t hashCode, TArgs&&... args) :
                EntryType(std::forward<TArgs>(args)...),
                HashCode(hashCode)
            {
                /// Nothing
            }

            /**
             * \brief Cached hash code of the entry's key
             *
             * Storing the hash code per-node makes rehashing O(n) instead of
             * requiring a rehash of every key, and speeds up equality checks
             * by short-circuiting on hash mismatch.
             */
            std::size_t HashCode;
        };

        // ---------------------------------------------------------------
        // Single bucket — a linked list of nodes
        // ---------------------------------------------------------------

        /**
         * \brief A single hash bucket, implemented as a linked list of \c Node objects
         *
         * All entries whose key maps to the same bucket index reside in the same
         * \c Bucket.  Collisions are resolved by chaining: each lookup walks the
         * list checking both the cached hash code and the equality predicate.
         *
         * \note The equality predicate \c TEqual is stored per-bucket rather than
         *       per-table to keep the bucket self-contained.  In practice all
         *       buckets in a table share the same predicate configuration.
         */
        class Bucket : public LinkedListBase<Node, TAllocator>
        {
        public:

            /**
             * \brief Insert a new entry or replace the value of an existing one
             *
             * Walks the bucket chain looking for a node whose hash code and key match
             * \p key.  If found, \c TEntryPolicy::ReplaceValue is called to update the
             * stored entry in-place.  Otherwise a new node is appended to the chain.
             *
             * \param hashCode Pre-computed hash code for \p key
             * \param key      The lookup key
             * \param args     Arguments forwarded to \c EntryType on a new insertion,
             *                 or to \c TEntryPolicy::ReplaceValue on an update
             *
             * \return \c true if a new node was appended, \c false if an existing
             *         entry was updated
             */
            template <typename... TArgs>
            bool Put(std::size_t hashCode, const KeyType& key, TArgs&&... args)
            {
                LinkedListNode<Node, TAllocator>* head = LinkedListBase<Node, TAllocator>::GetHead();
                while (head != nullptr)
                {
                    Node& node = head->GetValue();
                    if (node.HashCode == hashCode && mEqual(TEntryPolicy::GetKey(node), key))
                    {
                        TEntryPolicy::ReplaceValue(node, std::forward<TArgs>(args)...);
                        return false;
                    }
                    head = head->GetNext();
                }

                LinkedListBase<Node, TAllocator>::EmplaceBack(hashCode, std::forward<TArgs>(args)...);
                return true;
            }

            /**
             * \brief Find an entry by hash code and key
             *
             * \param hashCode Pre-computed hash code for \p key
             * \param key      The lookup key
             *
             * \return An \c Optional reference to the matching \c Node, or empty if
             *         no entry with \p key exists in this bucket
             */
            Optional<Node&> Find(std::size_t hashCode, const KeyType& key)
            {
                LinkedListNode<Node, TAllocator>* head = LinkedListBase<Node, TAllocator>::GetHead();
                while (head != nullptr)
                {
                    Node& node = head->GetValue();
                    if (node.HashCode == hashCode && mEqual(TEntryPolicy::GetKey(node), key))
                        return Optional<Node&>::Of(node);
                    head = head->GetNext();
                }
                return Optional<Node&>::Empty();
            }

            /**
             * \brief Find an entry by hash code and key (const overload)
             *
             * \param hashCode Pre-computed hash code for \p key
             * \param key      The lookup key
             *
             * \return An \c Optional const reference to the matching \c Node, or empty
             */
            Optional<const Node&> Find(std::size_t hashCode, const KeyType& key) const
            {
                const LinkedListNode<Node, TAllocator>* head = LinkedListBase<Node, TAllocator>::GetHead();
                while (head != nullptr)
                {
                    const Node& node = head->GetValue();
                    if (node.HashCode == hashCode && mEqual(TEntryPolicy::GetKey(node), key))
                        return Optional<const Node&>::Of(node);
                    head = head->GetNext();
                }
                return Optional<const Node&>::Empty();
            }

            /**
             * \brief Unconditionally append a new node to the bucket
             *
             * This bypasses the duplicate-key check and is used during rehashing,
             * where the caller guarantees uniqueness.
             *
             * \param hashCode Pre-computed hash code for the entry's key
             * \param args     Arguments forwarded to the \c EntryType constructor
             *
             * \return Reference to the newly inserted \c Node
             */
            template <typename... TArgs>
            Node& Emplace(std::size_t hashCode, TArgs&&... args)
            {
                return LinkedListBase<Node, TAllocator>::EmplaceBack(hashCode, std::forward<TArgs>(args)...);
            }

            /**
             * \brief Remove the entry whose key matches \p key
             *
             * \param hashCode Pre-computed hash code for \p key
             * \param key      The key of the entry to remove
             *
             * \return \c true if a matching entry was found and removed, \c false
             *         if no such entry existed in this bucket
             */
            bool Remove(std::size_t hashCode, const KeyType& key)
            {
                LinkedListNode<Node, TAllocator>* head = LinkedListBase<Node, TAllocator>::GetHead();
                while (head != nullptr)
                {
                    const Node& node = head->GetValue();
                    if (node.HashCode == hashCode && mEqual(TEntryPolicy::GetKey(node), key))
                    {
                        LinkedListBase<Node, TAllocator>::Unlink(head);
                        return true;
                    }
                    head = head->GetNext();
                }
                return false;
            }

            /**
             * \brief Remove all nodes from this bucket
             */
            void Clear()
            {
                LinkedListNode<Node, TAllocator>* head = LinkedListBase<Node, TAllocator>::GetHead();
                while (head != nullptr)
                    head = LinkedListBase<Node, TAllocator>::Unlink(head);
            }

        private:

            /**
             * \brief Equality predicate instance used to compare keys within this bucket
             */
            TEqual mEqual;
        };

        // ---------------------------------------------------------------
        // Constructors / assignment
        // ---------------------------------------------------------------

        /**
         * \brief Constructs an empty hash table with the given initial bucket count
         *
         * The actual bucket count is rounded up to the next power of two with a
         * minimum of 8, so that bucket selection can use a bitmask.
         *
         * \param bucketCount Desired initial bucket count (0 = use default minimum)
         */
        explicit HashTableBase(SizeType bucketCount = 0) :
            mSize(0)
        {
            mBucketCount = NextPowerOfTwo(std::max<SizeType>(8, bucketCount));
            mBuckets = MakeUnique<Bucket[]>(mBucketCount);
        }

        /**
         * \brief Copy constructor
         *
         * All entries from \p other are deep-copied into this table.  The new
         * table has the same bucket count as \p other so that load factor and
         * iteration order are preserved.
         *
         * \param other Table to copy from
         */
        HashTableBase(const HashTableBase& other) :
            mSize(0),
            mBucketCount(other.mBucketCount),
            mHasher(other.mHasher)
        {
            mBuckets = MakeUnique<Bucket[]>(mBucketCount);
            CopyFrom(other);
        }

        /**
         * \brief Move constructor
         *
         * Transfers ownership of the bucket array from \p other to this table.
         * \p other is left in a valid but empty state (size 0, zero buckets).
         *
         * \param other Table to move from
         */
        HashTableBase(HashTableBase&& other) noexcept
        {
            mSize = std::exchange(other.mSize, 0);
            mBucketCount = std::exchange(other.mBucketCount, 0);
            mHasher = std::move(other.mHasher);
            mBuckets = std::move(other.mBuckets);
        }

        /**
         * \brief Copy assignment operator
         *
         * Clears this table, then deep-copies all entries from \p other.
         *
         * \param other Table to copy from
         *
         * \return Reference to \c *this
         */
        HashTableBase& operator=(const HashTableBase& other)
        {
            if (this != &other)
            {
                Clear();
                mHasher = other.mHasher;
                mBucketCount = other.mBucketCount;
                mBuckets = MakeUnique<Bucket[]>(other.mBucketCount);
                CopyFrom(other);
            }
            return *this;
        }

        /**
         * \brief Move assignment operator
         *
         * Transfers ownership of \p other's bucket array.
         * \p other is left empty after the operation.
         *
         * \param other Table to move from
         *
         * \return Reference to \c *this
         */
        HashTableBase& operator=(HashTableBase&& other) noexcept
        {
            if (this != &other)
            {
                mSize = std::exchange(other.mSize, 0);
                mBucketCount = std::exchange(other.mBucketCount, 0);
                mHasher = std::move(other.mHasher);
                mBuckets = std::move(other.mBuckets);
            }
            return *this;
        }

        // ---------------------------------------------------------------
        // Core operations
        // ---------------------------------------------------------------

        /**
         * \brief Insert a new entry or replace the value of an existing one
         *
         * The entry's key is extracted from \p lookupKey.  If a node with the
         * same key already exists its value is updated via
         * \c TEntryPolicy::ReplaceValue; otherwise a new node is appended and
         * the table is rehashed if the load factor exceeds the threshold.
         *
         * \tparam TLookupKey Type of the key (may be a reference to avoid copying)
         * \tparam TArgs      Types forwarded to \c EntryType on insertion or to
         *                    \c TEntryPolicy::ReplaceValue on update
         *
         * \param lookupKey Key used for bucket selection and duplicate detection
         * \param args      Constructor / replacement arguments
         *
         * \return \c true if a new entry was inserted, \c false if an existing
         *         entry was updated
         */
        template <typename TLookupKey, typename... TArgs>
        bool PutEntry(TLookupKey&& lookupKey, TArgs&&... args)
        {
            std::size_t hashCode;
            Bucket& bucket = GetBucket(lookupKey, hashCode);
            const KeyType& canonicalKey = static_cast<const KeyType&>(lookupKey);
            if (bucket.Put(hashCode, canonicalKey, std::forward<TArgs>(args)...))
            {
                ++mSize;
                MaybeRehash();
                return true;
            }
            return false;
        }

        /**
         * \brief Insert a new entry only if the key is not already present
         *
         * Unlike \c PutEntry, this method never calls \c ReplaceValue.
         * If a node with \p lookupKey already exists, the operation is a no-op.
         *
         * \tparam TLookupKey Type of the key
         * \tparam TArgs      Types forwarded to \c EntryType on a new insertion
         *
         * \param lookupKey Key to check and potentially insert
         * \param args      Constructor arguments used only when inserting
         *
         * \return \c true if a new entry was inserted, \c false if the key was
         *         already present
         */
        template <typename TLookupKey, typename... TArgs>
        bool PutEntryIfMissing(TLookupKey&& lookupKey, TArgs&&... args)
        {
            std::size_t hashCode;
            Bucket& bucket = GetBucket(lookupKey, hashCode);
            const KeyType& canonicalKey = static_cast<const KeyType&>(lookupKey);
            if (!bucket.Find(hashCode, canonicalKey).IsEmpty())
                return false;

            bucket.Emplace(hashCode, std::forward<TArgs>(args)...);
            ++mSize;
            MaybeRehash();
            return true;
        }

        /**
         * \brief Find the internal \c Node for \p key
         *
         * \param key Key to search for
         *
         * \return An \c Optional mutable reference to the matching \c Node,
         *         or empty if not found
         */
        Optional<Node&> FindNode(const KeyType& key)
        {
            std::size_t hashCode;
            return GetBucket(key, hashCode).Find(hashCode, key);
        }

        /**
         * \brief Find the internal \c Node for \p key (const overload)
         *
         * \param key Key to search for
         *
         * \return An \c Optional const reference to the matching \c Node,
         *         or empty if not found
         */
        Optional<const Node&> FindNode(const KeyType& key) const
        {
            std::size_t hashCode;
            return GetBucket(key, hashCode).Find(hashCode, key);
        }

        /**
         * \brief Remove the entry whose key equals \p key
         *
         * \param key Key of the entry to remove
         *
         * \return \c true if an entry was found and removed, \c false otherwise
         */
        bool RemoveEntry(const KeyType& key)
        {
            std::size_t hashCode;
            Bucket& bucket = GetBucket(key, hashCode);
            if (bucket.Remove(hashCode, key))
            {
                --mSize;
                return true;
            }
            return false;
        }

        /**
         * \brief Remove all entries from the table
         *
         * Every bucket is cleared; the bucket array itself is kept at its
         * current capacity.
         */
        void Clear()
        {
            for (SizeType i = 0; i < mBucketCount; i++)
                mBuckets[i].Clear();
            mSize = 0;
        }

        /**
         * \brief Pre-allocate enough buckets to hold at least \p capacity entries
         *        without triggering a rehash
         *
         * The required bucket count is computed from \p capacity using the 0.75
         * load factor threshold, then rounded up to the next power of two.
         * If the resulting count does not exceed the current bucket count, this
         * is a no-op.
         *
         * \param capacity Minimum number of entries to support without rehashing
         */
        void Reserve(SizeType capacity)
        {
            SizeType required = NextPowerOfTwo(std::max<SizeType>(8, capacity * 4 / 3 + 1));
            if (required > mBucketCount)
                Rehash(required);
        }

        /**
         * \brief Returns the number of entries currently stored in the table
         */
        SizeType GetSize() const
        {
            return mSize;
        }

        /**
         * \brief Returns the current number of buckets
         *
         * This is always a power of two and is at least 8.
         */
        SizeType GetBucketCount() const
        {
            return mBucketCount;
        }

        /**
         * \brief Returns \c true if the table contains no entries
         */
        bool IsEmpty() const
        {
            return mSize == 0;
        }

        /**
         * \brief Resolve a key to its bucket and output the pre-computed hash code
         *
         * The bucket index is \c hashCode & (mBucketCount - 1), which is safe
         * because \c mBucketCount is always a power of two.
         *
         * \tparam TLookupKey Key type (forwarded to the hasher)
         *
         * \param key      The key to resolve
         * \param hashCode Output parameter: receives the hash value
         *
         * \return Mutable reference to the resolved bucket
         */
        template <typename TLookupKey>
        Bucket& GetBucket(TLookupKey&& key, std::size_t& hashCode)
        {
            hashCode = mHasher(key);
            return mBuckets[hashCode & (mBucketCount - 1)];
        }

        /**
         * \brief Resolve a key to its bucket and output the pre-computed hash code (const overload)
         *
         * \tparam TLookupKey Key type (forwarded to the hasher)
         *
         * \param key      The key to resolve
         * \param hashCode Output parameter: receives the hash value
         *
         * \return Const reference to the resolved bucket
         */
        template <typename TLookupKey>
        const Bucket& GetBucket(TLookupKey&& key, std::size_t& hashCode) const
        {
            hashCode = mHasher(key);
            return mBuckets[hashCode & (mBucketCount - 1)];
        }

    private:

        /**
         * \brief Deep-copy all entries from \p other into this table
         *
         * The hash codes cached in each source node are reused directly so that
         * no rehashing is required.  Nodes are placed into buckets according to
         * \c hashCode & (mBucketCount - 1).
         *
         * \pre \c mBuckets must already be allocated with \c mBucketCount buckets.
         *
         * \param other Table to copy entries from
         */
        void CopyFrom(const HashTableBase& other)
        {
            for (SizeType i = 0; i < other.mBucketCount; i++)
            {
                LinkedListConstIterator<Node, TAllocator> it(other.mBuckets[i]);
                while (it.IsValid())
                {
                    const Node& src = it.GetValue();
                    SizeType idx = src.HashCode & (mBucketCount - 1);
                    mBuckets[idx].Emplace(src.HashCode, static_cast<const EntryType&>(src));
                    ++mSize;
                    it = it.Next();
                }
            }
        }

        /**
         * \brief Trigger a rehash if the current load factor exceeds 0.75
         *
         * Called after every successful insertion.  Doubles the bucket count
         * when the threshold is crossed.
         */
        void MaybeRehash()
        {
            if (mSize > mBucketCount * 3 / 4)
                Rehash(mBucketCount << 1);
        }

        /**
         * \brief Redistribute all entries into a new bucket array of size \p newBucketCount
         *
         * Each entry is moved (not copied) into its new bucket using the cached
         * hash code.  The old bucket array is released after migration.
         *
         * \param newBucketCount New number of buckets; must be a power of two
         */
        void Rehash(SizeType newBucketCount)
        {
            UniquePtr<Bucket[]> newBuckets = MakeUnique<Bucket[]>(newBucketCount);

            for (SizeType i = 0; i < mBucketCount; i++)
            {
                LinkedListIterator<Node, TAllocator> it(mBuckets[i]);
                while (it.IsValid())
                {
                    Node& node = it.GetValue();
                    SizeType newIndex = node.HashCode & (newBucketCount - 1);
                    newBuckets[newIndex].Emplace(node.HashCode, std::move(static_cast<EntryType&>(node)));
                    it = it.Next();
                }
            }

            mBuckets = std::move(newBuckets);
            mBucketCount = newBucketCount;
        }

    protected:

        SizeType mSize; ///< Number of entries currently stored
        SizeType mBucketCount; ///< Current bucket count (always a power of two)
        THash mHasher; ///< Hash function instance
        UniquePtr<Bucket[]> mBuckets; ///< Owning array of buckets
    };

    // -------------------------------------------------------------------
    // Iterator helpers shared by HashMap and HashSet
    // -------------------------------------------------------------------

    /**
     * \brief Mutable forward iterator over a \c HashTableBase
     *
     * Traverses all entries in bucket order.  Within a bucket, entries are
     * visited in insertion order (linked-list order).
     *
     * The iterator skips empty buckets automatically so the caller always
     * receives valid entries until \c IsValid() returns \c false.
     *
     * \par Invalidation
     * Any operation that causes a rehash (insertion beyond the load threshold)
     * invalidates all outstanding iterators.
     *
     * \tparam THashTable  The concrete hash table type being iterated
     * \tparam TValueType  The type exposed through \c GetValue() and \c operator*
     *                     (e.g. \c MapEntry<K,V> for HashMap, \c EntryType for HashSet)
     */
    template <typename THashTable, typename TValueType>
    class HashTableIterator
    {
        using BucketIterator = LinkedListIterator<typename THashTable::Node, typename THashTable::AllocatorType>;

    public:

        /**
         * \brief Construct an iterator positioned at the first valid entry at or
         *        after bucket \p firstBucket
         *
         * \param owner       The hash table to iterate
         * \param firstBucket Index of the first bucket to consider (0 = beginning)
         */
        explicit HashTableIterator(THashTable& owner, typename THashTable::SizeType firstBucket = 0) :
            mOwner(&owner),
            mBucketIndex(firstBucket),
            mBucketIterator(nullptr)
        {
            AdvanceToValid();
        }

        /**
         * \brief Move the iterator backward by \p count entries
         *
         * Crosses bucket boundaries as needed.
         *
         * \param count Number of entries to step backward
         *
         * \return Reference to \c *this
         */
        HashTableIterator& Rewind(typename THashTable::SizeType count = 1)
        {
            for (typename THashTable::SizeType i = 0; i < count; i++)
            {
                mBucketIterator.Rewind();
                RewindToValid();
            }
            return *this;
        }

        /**
         * \brief Move the iterator forward by \p count entries
         *
         * Crosses bucket boundaries as needed.
         *
         * \param count Number of entries to step forward
         *
         * \return Reference to \c *this
         */
        HashTableIterator& Advance(typename THashTable::SizeType count = 1)
        {
            for (typename THashTable::SizeType i = 0; i < count; i++)
            {
                mBucketIterator.Advance();
                AdvanceToValid();
            }
            return *this;
        }

        /**
         * \brief Check whether the iterator points to a valid entry
         *
         * \return \c true if the iterator is positioned on an entry, \c false
         *         if all entries have been visited
         */
        bool IsValid() const
        {
            return mBucketIterator.IsValid();
        }

        /**
         * \brief Get a mutable reference to the current entry
         *
         * \return Reference to the current \c TValueType
         */
        TValueType& GetValue()
        {
            return static_cast<TValueType&>(mBucketIterator.GetValue());
        }

        /**
         * \brief Get a const reference to the current entry
         *
         * \return Const reference to the current \c TValueType
         */
        const TValueType& GetValue() const
        {
            return static_cast<const TValueType&>(mBucketIterator.GetValue());
        }

        /**
         * \brief Dereferences the iterator
         *
         * \return Reference to the current entry
         */
        TValueType& operator*() const
        {
            return static_cast<TValueType&>(mBucketIterator.GetValue());
        }

        /**
         * \brief Member-access through the iterator
         *
         * \return Pointer to the current entry
         */
        TValueType* operator->() const
        {
            return &static_cast<TValueType&>(mBucketIterator.GetValue());
        }

        /**
         * \brief Pre-increment — advance to the next entry
         *
         * \return Reference to \c *this after advancing
         */
        HashTableIterator& operator++()
        {
            mBucketIterator = mBucketIterator.Next();
            AdvanceToValid();
            return *this;
        }

        /**
         * \brief Pre-decrement — retreat to the previous entry
         *
         * \return Reference to \c *this after retreating
         */
        HashTableIterator& operator--()
        {
            mBucketIterator = mBucketIterator.Previous();
            RewindToValid();
            return *this;
        }

        /**
         * \brief Equality comparison
         *
         * Two iterators are equal when they point to the same node within the
         * same bucket chain.
         */
        bool operator==(const HashTableIterator& other) const
        {
            return mBucketIterator == other.mBucketIterator;
        }

        /**
         * \brief Inequality comparison
         */
        bool operator!=(const HashTableIterator& other) const
        {
            return !(*this == other);
        }

    private:

        template <typename, typename>
        friend class HashTableConstIterator;

        /**
         * \brief Move back through buckets until a non-empty position is found
         *        or the beginning of the table is reached
         */
        void RewindToValid()
        {
            while (!mBucketIterator.IsValid() && mBucketIndex > 0)
            {
                mBucketIterator = BucketIterator(mOwner->mBuckets[mBucketIndex--]);
                if (mBucketIterator.IsValid())
                    return;
            }
        }

        /**
         * \brief Advance through buckets until a non-empty position is found
         *        or the end of the table is reached
         */
        void AdvanceToValid()
        {
            while (!mBucketIterator.IsValid() && mBucketIndex < mOwner->mBucketCount)
            {
                mBucketIterator = BucketIterator(mOwner->mBuckets[mBucketIndex++]);
                if (mBucketIterator.IsValid())
                    return;
            }
        }

        THashTable* mOwner;                        ///< Pointer to the iterated hash table
        typename THashTable::SizeType mBucketIndex; ///< Index of the bucket currently being traversed
        BucketIterator mBucketIterator;            ///< Iterator into the current bucket's linked list
    };

    /**
     * \brief Read-only forward iterator over a \c HashTableBase
     *
     * Semantically identical to \c HashTableIterator but provides only
     * const access to the stored entries.  Can be implicitly constructed
     * from a \c HashTableIterator.
     *
     * \tparam THashTable  The concrete hash table type being iterated
     * \tparam TValueType  The type exposed through \c GetValue() and \c operator*
     */
    template <typename THashTable, typename TValueType>
    class HashTableConstIterator
    {
        using BucketIterator = LinkedListConstIterator<typename THashTable::Node, typename THashTable::AllocatorType>;

    public:

        /**
         * \brief Construct a const iterator positioned at the first valid entry
         *        at or after bucket \p firstBucket
         *
         * \param owner       The hash table to iterate (const reference)
         * \param firstBucket Index of the first bucket to consider (0 = beginning)
         */
        explicit HashTableConstIterator(const THashTable& owner, typename THashTable::SizeType firstBucket = 0) :
            mOwner(&owner),
            mBucketIndex(firstBucket),
            mBucketIterator(nullptr)
        {
            AdvanceToValid();
        }

        /**
         * \brief Implicit conversion from a mutable iterator
         *
         * Allows a \c HashTableIterator to be passed wherever a
         * \c HashTableConstIterator is expected.
         *
         * \param other The mutable iterator to copy state from
         */
        HashTableConstIterator(HashTableIterator<THashTable, TValueType> other) :
            mOwner(other.mOwner),
            mBucketIndex(other.mBucketIndex),
            mBucketIterator(other.mBucketIterator)
        {
        }

        /**
         * \brief Move the iterator backward by \p count entries
         *
         * \param count Number of entries to step backward
         *
         * \return Reference to \c *this
         */
        HashTableConstIterator& Rewind(typename THashTable::SizeType count = 1)
        {
            for (typename THashTable::SizeType i = 0; i < count; i++)
            {
                mBucketIterator.Rewind();
                RewindToValid();
            }
            return *this;
        }

        /**
         * \brief Move the iterator forward by \p count entries
         *
         * \param count Number of entries to step forward
         *
         * \return Reference to \c *this
         */
        HashTableConstIterator& Advance(typename THashTable::SizeType count = 1)
        {
            for (typename THashTable::SizeType i = 0; i < count; i++)
            {
                mBucketIterator.Advance();
                AdvanceToValid();
            }
            return *this;
        }

        /**
         * \brief Check whether the iterator points to a valid entry
         *
         * \return \c true if positioned on an entry, \c false when exhausted
         */
        bool IsValid() const
        {
            return mBucketIterator.IsValid();
        }

        /**
         * \brief Get a const reference to the current entry
         *
         * \return Const reference to the current \c TValueType
         */
        const TValueType& GetValue() const
        {
            return static_cast<const TValueType&>(mBucketIterator.GetValue());
        }

        /**
         * \brief Dereferences the iterator
         *
         * \return Const reference to the current entry
         */
        const TValueType& operator*() const
        {
            return static_cast<const TValueType&>(mBucketIterator.GetValue());
        }

        /**
         * \brief Member-access through the iterator
         *
         * \return Const pointer to the current entry
         */
        const TValueType* operator->() const
        {
            return &static_cast<const TValueType&>(mBucketIterator.GetValue());
        }

        /**
         * \brief Pre-increment — advance to the next entry
         *
         * \return Reference to \c *this after advancing
         */
        HashTableConstIterator& operator++()
        {
            mBucketIterator = mBucketIterator.Next();
            AdvanceToValid();
            return *this;
        }

        /**
         * \brief Pre-decrement — retreat to the previous entry
         *
         * \return Reference to \c *this after retreating
         */
        HashTableConstIterator& operator--()
        {
            mBucketIterator = mBucketIterator.Previous();
            RewindToValid();
            return *this;
        }

        /**
         * \brief Equality comparison
         */
        bool operator==(const HashTableConstIterator& other) const
        {
            return mBucketIterator == other.mBucketIterator;
        }

        /**
         * \brief Inequality comparison
         */
        bool operator!=(const HashTableConstIterator& other) const
        {
            return !(*this == other);
        }

    private:

        /**
         * \brief Move back through buckets until a non-empty position is found
         *        or the beginning of the table is reached
         */
        void RewindToValid()
        {
            while (!mBucketIterator.IsValid() && mBucketIndex > 0)
            {
                mBucketIterator = BucketIterator(mOwner->mBuckets[mBucketIndex--]);
                if (mBucketIterator.IsValid())
                    return;
            }
        }

        /**
         * \brief Advance through buckets until a non-empty position is found
         *        or the end of the table is reached
         */
        void AdvanceToValid()
        {
            while (!mBucketIterator.IsValid() && mBucketIndex < mOwner->mBucketCount)
            {
                mBucketIterator = BucketIterator(mOwner->mBuckets[mBucketIndex++]);
                if (mBucketIterator.IsValid())
                    return;
            }
        }

        const THashTable* mOwner;           ///< Pointer to the iterated hash table (const)
        typename THashTable::SizeType mBucketIndex; ///< Index of the bucket currently being traversed
        BucketIterator mBucketIterator;     ///< Iterator into the current bucket's linked list
    };
}

#endif // COCKTAIL_CORE_HASHTABLE_HPP
