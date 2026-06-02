#ifndef COCKTAIL_CORE_HASHMAP_HPP
#define COCKTAIL_CORE_HASHMAP_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/LinkedList.hpp>

namespace Ck
{
    /**
     * \brief Represents a key-value pair stored in a map
     *
     * This structure stores a key and its associated value. It is used
     * as the base representation for entries contained within HashMap.
     *
     * \tparam TKey Type of the key
     * \tparam TValue Type of the value
     */
    template <typename TKey, typename TValue>
    struct MapEntry
    {
        using KeyType = TKey;
        using ValueType = TValue;

        /**
         * \brief Constructs an empty map entry
         */
        MapEntry() = default;

        /**
         * \brief Constructs a map entry from a key and value
         *
         * \param key Entry key
         * \param value Entry value
         */
        MapEntry(const KeyType& key, const ValueType& value) :
            Key(key),
            Value(value)
        {
            /// Nothing
        }

        /**
         * \brief Constructs a map entry using perfect forwarding
         *
         * \tparam TOtherKey Key argument type
         * \tparam TOtherValue Value argument type
         *
         * \param key Entry key
         * \param value Entry value
         */
        template <typename TOtherKey = TKey, typename TOtherValue = TValue>
        MapEntry(TOtherKey&& key, TOtherValue&& value) :
            Key(std::forward<TOtherKey>(key)),
            Value(std::forward<TOtherValue>(value))
        {
            /// Nothing
        }

        KeyType Key; ///< Entry key
        ValueType Value; ///< Entry value
    };

    /**
     * \brief Associative container storing key-value pairs using a hash table
     *
     * HashMap provides average constant-time complexity for insertion,
     * lookup, and removal operations. Entries are distributed across
     * buckets according to the hash value of their keys.
     *
     * \tparam TKey Type used for keys
     * \tparam TValue Type used for stored values
     * \tparam THash Hash function used to generate hash codes for keys
     * \tparam TEqual Equality predicate used to compare keys
     * \tparam TAllocator Allocator used for internal memory management
     */
    template <typename TKey, typename TValue, typename THash = std::hash<TKey>, typename TEqual = std::equal_to<TKey>, typename TAllocator = HeapAllocator>
    class HashMap
    {
    public:

        using KeyType = TKey;
        using ValueType = TValue;
        using AllocatorType = TAllocator;
        using SizeType = typename TAllocator::SizeType;

    private:

        struct Entry : MapEntry<KeyType, ValueType>
        {
            Entry() :
                HashCode(0)
            {
                /// Nothing
            }

            Entry(std::size_t hashCode, const KeyType& key, const ValueType& value) :
                MapEntry<KeyType, ValueType>(key, value),
                HashCode(hashCode)
            {
                /// Nothing
            }

            template <typename TOtherKey = TKey, typename TOtherValue>
            Entry(std::size_t hashCode, TOtherKey&& key, TOtherValue&& value) :
                MapEntry<KeyType, ValueType>(std::forward<TOtherKey>(key), std::forward<TOtherValue>(value)),
                HashCode(hashCode)
            {
                /// Nothing
            }

            std::size_t HashCode;
        };

        class Bucket : public Detail::LinkedListBase<Entry, TAllocator>
        {
        public:

            template <typename TOtherKey, typename TOtherValue>
            bool Put(std::size_t hashCode, TOtherKey&& key, TOtherValue&& value)
            {
                Detail::LinkedListNode<Entry, TAllocator>* head = Detail::LinkedListBase<Entry, TAllocator>::GetHead();
                while (head != nullptr)
                {
                    Entry& entry = head->GetValue();
                    if (entry.HashCode == hashCode && mComparator(entry.Key, key))
                    {
                        ObjectMemoryUtils::Destroy(entry.Value);
                        ObjectMemoryUtils::Construct(&entry.Value, std::forward<TOtherValue>(value));

                        return false;
                    }

                    head = head->GetNext();
                }

                Detail::LinkedListBase<Entry, TAllocator>::EmplaceBack(hashCode, std::forward<TOtherKey>(key), std::forward<TOtherValue>(value));
                return true;
            }

            Optional<Entry&> Find(std::size_t hashCode, const KeyType& key)
            {
                Detail::LinkedListNode<Entry, TAllocator>* head = Detail::LinkedListBase<Entry, TAllocator>::GetHead();
                while (head != nullptr)
                {
                    Entry& entry = head->GetValue();
                    if (entry.HashCode == hashCode && mComparator(entry.Key, key))
                        return Optional<Entry&>::Of(head->GetValue());

                    head = head->GetNext();
                }

                return Optional<Entry&>::Empty();
            }

            Optional<const Entry&> Find(std::size_t hashCode, const KeyType& key) const
            {
                Detail::LinkedListNode<Entry, TAllocator>* head = Detail::LinkedListBase<Entry, TAllocator>::GetHead();
                while (head != nullptr)
                {
                    const Entry& entry = head->GetValue();
                    if (entry.HashCode == hashCode && mComparator(entry.Key, key))
                        return Optional<const Entry&>::Of(head->GetValue());

                    head = head->GetNext();
                }

                return Optional<Entry&>::Empty();
            }

            template <typename TOtherKey, typename TOtherValue>
            Entry& Emplace(std::size_t hashCode, TOtherKey&& key, TOtherValue&& value)
            {
                return Detail::LinkedListBase<Entry, TAllocator>::EmplaceBack(hashCode, std::forward<TOtherKey>(key), std::forward<TOtherValue>(value));
            }

            bool Remove(std::size_t hashCode, const KeyType& key)
            {
                Detail::LinkedListNode<Entry, TAllocator>* head = Detail::LinkedListBase<Entry, TAllocator>::GetHead();
                while (head != nullptr)
                {
                    const Entry& value = head->GetValue();
                    if (value.HashCode == hashCode && mComparator(value.Key, key))
                    {
                        Detail::LinkedListBase<Entry, TAllocator>::Unlink(head);
                        return true;
                    }

                    head = head->GetNext();
                }

                return false;
            }

            void Clear()
            {
                Detail::LinkedListNode<Entry, TAllocator>* head = Detail::LinkedListBase<Entry, TAllocator>::GetHead();
                while (head != nullptr)
                    head = Detail::LinkedListBase<Entry, TAllocator>::Unlink(head);
            }

        private:

            TEqual mComparator;
        };

    public:

        class Iterator;
        class ConstIterator;

        /**
         * \brief Forward iterator used to traverse all entries in the hash map
         *
         * The iterator automatically skips empty buckets and provides access
         * to the stored key-value pairs.
         */
        class Iterator
        {
            using BucketIterator = Detail::LinkedListIterator<Entry, TAllocator>;

        public:

            /**
             * \brief Creates an iterator starting at the specified bucket
             *
             * \param owner Hash map being iterated.
             * \param firstBucket Index of the first bucket to inspect.
             */
            explicit Iterator(HashMap& owner, SizeType firstBucket = 0) :
                mOwner(&owner),
                mBucketIndex(firstBucket),
                mBucketIterator(nullptr)
            {
                AdvanceToValid();
            }

            /**
             * \brief Moves the iterator backward
             *
             * \param count Number of elements to rewind
             *
             * \return Reference to this iterator
             */
            Iterator& Rewind(SizeType count = 1)
            {
                for (SizeType i = 0; i < count; i++)
                {
                    mBucketIterator.Rewind();
                    RewindToValid();
                }

                return *this;
            }

            /**
             * \brief Moves the iterator forward
             *
             * \param count Number of elements to advance
             *
             * \return Reference to this iterator
             */
            Iterator& Advance(SizeType count = 1)
            {
                for (SizeType i = 0; i < count; i++)
                {
                    mBucketIterator.Advance();
                    AdvanceToValid();
                }

                return *this;
            }

            /**
             * \brief Checks whether the iterator references a valid entry
             *
             * \return True if the iterator is valid, false otherwise
             */
            bool IsValid() const
            {
                return mBucketIterator.IsValid();
            }

            /**
             * \brief Returns the current entry
             *
             * \return Reference to the current key-value pair
             */
            MapEntry<KeyType, ValueType>& GetValue()
            {
                return mBucketIterator.GetValue();
            }

            /**
             * \brief Returns the current entry
             *
             * \return Reference to the current key-value pair
             */
            const MapEntry<KeyType, ValueType>& GetValue() const
            {
                return mBucketIterator.GetValue();
            }

            /**
             * \brief Dereferences the iterator
             *
             * \return Reference to the current key-value pair
             */
            MapEntry<KeyType, ValueType>& operator*() const
            {
                return mBucketIterator.GetValue();
            }

            /**
             * \brief Provides pointer-like access to the current entry
             *
             * \return Pointer to the current key-value pair
             */
            MapEntry<KeyType, ValueType>* operator->() const
            {
                return &mBucketIterator.GetValue();
            }

            /**
             * \brief Advances the iterator to the next entry
             *
             * \return Reference to this iterator
             */
            Iterator& operator++()
            {
                mBucketIterator = mBucketIterator.Next();
                AdvanceToValid();

                return *this;
            }

            /**
             * \brief Rewind the iterator to the previous entry
             *
             * \return Reference to this iterator
             */
            Iterator& operator--()
            {
                mBucketIterator = mBucketIterator.Previous();
                RewindToValid();

                return *this;
            }

            /**
             * \brief Compares two iterators for equality
             *
             * \param other Iterator to compare against
             *
             * \return True if both iterators reference the same position
             */
            bool operator==(const Iterator& other) const
            {
                return mBucketIterator == other.mBucketIterator;
            }

            /**
             * \brief Compares two iterators for inequality
             *
             * \param other Iterator to compare against
             *
             * \return True if the iterators reference different positions
             */
            bool operator!=(const Iterator& other) const
            {
                return !(*this == other);
            }

        private:

            friend class ConstIterator;

            /**
             * \brief Moves backward until a valid entry is found
             */
            void RewindToValid()
            {
                while (!mBucketIterator.IsValid() && mBucketIndex > 0)
                {
                    Bucket& bucket = mOwner->mBuckets[mBucketIndex--];
                    mBucketIterator = BucketIterator(bucket);

                    if (mBucketIterator.IsValid())
                        return;
                }
            }

            /**
             * \brief Moves forward until a valid entry is found
             */
            void AdvanceToValid()
            {
                while (!mBucketIterator.IsValid() && mBucketIndex < mOwner->mBucketCount)
                {
                    Bucket& bucket = mOwner->mBuckets[mBucketIndex++];
                    mBucketIterator = BucketIterator(bucket);

                    if (mBucketIterator.IsValid())
                        return;
                }
            }

            HashMap* mOwner; ///< Hash map being iterated
            SizeType mBucketIndex; ///< Current bucket index
            BucketIterator mBucketIterator; ///< Iterator within the current bucket
        };

        /**
         * \brief Read-only forward iterator used to traverse all entries in the hash map
         *
         * The iterator automatically skips empty buckets and provides
         * read-only access to stored key-value pairs.
         */
        class ConstIterator
        {
            using BucketIterator = Detail::LinkedListConstIterator<Entry, TAllocator>;

        public:

            /**
             * \brief Creates a const iterator starting at the specified bucket
             *
             * \param owner Hash map being iterated
             * \param firstBucket Index of the first bucket to inspect
             */
            ConstIterator(const HashMap& owner, SizeType firstBucket = 0) :
                mOwner(&owner),
                mBucketIndex(firstBucket),
                mBucketIterator(nullptr)
            {
                AdvanceToValid();
            }

            /**
             * \brief Constructs a const iterator from a mutable iterator
             *
             * \param other Iterator to convert
             */
            ConstIterator(Iterator other) :
                mOwner(other.mOwner),
                mBucketIndex(other.mBucketIndex),
                mBucketIterator(other.mBucketIterator)
            {
                /// Nothing
            }

            /**
             * \brief Moves the iterator backward
             *
             * \param count Number of elements to rewind
             *
             * \return Reference to this iterator
             */
            ConstIterator& Rewind(SizeType count = 1)
            {
                for (SizeType i = 0; i < count; i++)
                {
                    mBucketIterator.Rewind();
                    RewindToValid();
                }

                return *this;
            }

            /**
             * \brief Moves the iterator forward
             *
             * \param count Number of elements to advance
             *
             * \return Reference to this iterator
             */
            ConstIterator& Advance(SizeType count = 1)
            {
                for (SizeType i = 0; i < count; i++)
                {
                    mBucketIterator.Advance();
                    AdvanceToValid();
                }

                return *this;
            }

            /**
             * \brief Checks whether the iterator references a valid entry
             *
             * \return True if the iterator is valid, false otherwise
             */
            bool IsValid() const
            {
                return mBucketIterator.IsValid();
            }

            /**
             * \brief Returns the current entry
             *
             * \return Constant reference to the current key-value pair
             */
            const MapEntry<KeyType, ValueType>& GetValue() const
            {
                return mBucketIterator.GetValue();
            }

            /**
             * \brief Dereferences the iterator
             *
             * \return Constant reference to the current key-value pair
             */
            const MapEntry<KeyType, ValueType>& operator*() const
            {
                return mBucketIterator.GetValue();
            }

            /**
             * \brief Provides pointer-like access to the current entry
             *
             * \return Pointer to the current key-value pair
             */
            const MapEntry<KeyType, ValueType>* operator->() const
            {
                return &mBucketIterator.GetValue();
            }

            /**
             * \brief Advances the iterator to the next entry
             *
             * \return Reference to this iterator
             */
            ConstIterator& operator++()
            {
                mBucketIterator = mBucketIterator.Next();
                AdvanceToValid();

                return *this;
            }

            /**
             * \brief Rewind the iterator to the previous entry
             *
             * \return Reference to this iterator
             */
            Iterator& operator--()
            {
                mBucketIterator = mBucketIterator.Previous();
                RewindToValid();

                return *this;
            }

            /**
             * \brief Compares the iterator with a mutable iterator
             *
             * \param other Iterator to compare against
             *
             * \return True if both iterators reference the same position
             */
            bool operator==(const Iterator& other) const
            {
                return mBucketIterator == other.mBucketIterator;
            }

            /**
             * \brief Compares two const iterators for equality
             *
             * \param other Iterator to compare against
             *
             * \return True if both iterators reference the same position
             */
            bool operator==(const ConstIterator& other) const
            {
                return mBucketIterator == other.mBucketIterator;
            }

            /**
             * \brief Compares the iterator with a mutable iterator
             *
             * \param other Iterator to compare against
             *
             * \return True if the iterators reference different positions
             */
            bool operator!=(const Iterator& other) const
            {
                return !(*this == other);
            }

            /**
             * \brief Compares two const iterators for inequality
             *
             * \param other Iterator to compare against
             *
             * \return True if the iterators reference different positions
             */
            bool operator!=(const ConstIterator& other) const
            {
                return !(*this == other);
            }

        private:

            /**
             * \brief Moves backward until a valid entry is found
             */
            void RewindToValid()
            {
                while (!mBucketIterator.IsValid() && mBucketIndex > 0)
                {
                    Bucket& bucket = mOwner->mBuckets[mBucketIndex--];
                    mBucketIterator = BucketIterator(bucket);

                    if (mBucketIterator.IsValid())
                        return;
                }
            }

            /**
             * \brief Moves forward until a valid entry is found
             */
            void AdvanceToValid()
            {
                while (!mBucketIterator.IsValid() && mBucketIndex < mOwner->mBucketCount)
                {
                    const Bucket& bucket = mOwner->mBuckets[mBucketIndex++];
                    mBucketIterator = BucketIterator(bucket);

                    if (mBucketIterator.IsValid())
                        return;
                }
            }

            const HashMap* mOwner; ///< Hash map being iterated.
            SizeType mBucketIndex; ///< Current bucket index.
            BucketIterator mBucketIterator; ///< Iterator within the current bucket.
        };

        /**
         * \brief Constructs a hash map with the specified initial bucket count
         *
         * The actual bucket count is rounded up to the next power of two,
         * with a minimum of 8 buckets.
         *
         * \param bucketCount Desired initial bucket count
         */
        HashMap(SizeType bucketCount = 0) :
            mSize(0)
        {
            mBucketCount = NextPowerOfTwo(std::max<SizeType>(8, bucketCount));
            mBuckets = MakeUnique<Bucket[]>(mBucketCount);
        }

        /**
         * \brief Constructs a hash map by copying another hash map
         *
         * All entries from the source hash map are duplicated into this instance
         *
         * \param other Hash map to copy
         */
        HashMap(const HashMap& other) :
            mSize(0),
            mBucketCount(other.mBucketCount),
            mHasher(other.mHasher)
        {
            mBuckets = MakeUnique<Bucket[]>(mBucketCount);

            ConstIterator iterator(other);
            while (iterator.IsValid())
            {
                const MapEntry<KeyType, ValueType>& entry = iterator.GetValue();
                Put(entry.Key, entry.Value);

                iterator.Advance();
            }
        }

        /**
         * \brief Constructs a hash map by taking ownership of another hash map's resources
         *
         * After the move, the source hash map is left in a valid but unspecified state
         *
         * \param other Hash map to move from
         */
        HashMap(HashMap&& other) noexcept
        {
            mSize = std::exchange(other.mSize, 0);
            mBucketCount = std::exchange(other.mBucketCount, 0);
            mHasher = std::move(other.mHasher);
            mBuckets = std::move(other.mBuckets);
        }

        /**
         * \brief Replaces the contents of this hash map with a copy of another hash map
         *
         * Existing entries are removed before copying the contents of the source hash map.
         *
         * \param other Hash map to copy
         *
         * \return Reference to this hash map
         */
        HashMap& operator=(const HashMap& other)
        {
            if (this != &other)
            {
                Clear();

                mHasher = other.mHasher;
                mBucketCount = other.mBucketCount;
                mBuckets = MakeUnique<Bucket[]>(other.mBucketCount);

                ConstIterator iterator(other);
                while (iterator.IsValid())
                {
                    const MapEntry<KeyType, ValueType>& entry = iterator.GetValue();
                    Put(entry.Key, entry.Value);

                    iterator.Advance();
                }
            }

            return *this;
        }

        /**
         * \brief Replaces the contents of this hash map by taking ownership of another hash map's resources
         *
         * Existing resources are released and ownership of the source hash map's
         * resources is transferred to this instance.
         *
         * \param other Hash map to move from
         *
         * \return Reference to this hash map
         */
        HashMap& operator=(HashMap&& other) noexcept
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

        /**
         * \brief Inserts or replaces a key-value pair
         *
         * If the key already exists, its associated value is replaced.
         * Otherwise, a new entry is inserted.
         *
         * \tparam TOtherKey Key type
         * \tparam TOtherValue Value type
         *
         * \param key Key to insert or update
         * \param value Value associated with the key
         */
        template <typename TOtherKey, typename TOtherValue>
        void Put(TOtherKey&& key, TOtherValue&& value)
        {
            std::size_t hashCode;
            Bucket& bucket = GetBucket(key, hashCode);
            if (bucket.Put(hashCode, std::forward<TOtherKey>(key), std::forward<TOtherValue>(value)))
            {
                ++mSize;
                if (mSize > mBucketCount * 3 / 4)
                    Rehash(mBucketCount << 1);
            }
        }

        /**
         * \brief Inserts a key-value pair only if the key is not already present
         *
         * \tparam TOtherKey Key type
         * \tparam TOtherValue Value type
         *
         * \param key Key to insert
         * \param value Value associated with the key
         *
         * \return True if a new entry was inserted, false if the key already existed
         */
        template <typename TOtherKey, typename TOtherValue>
        bool PutIfMissing(TOtherKey&& key, TOtherValue&& value)
        {
            std::size_t hashCode;
            Bucket& bucket = GetBucket(key, hashCode);
            if (!bucket.Find(hashCode, key).IsEmpty())
                return false;

            bucket.Emplace(hashCode, std::forward<TOtherKey>(key), std::forward<TOtherValue>(value));
            ++mSize;

            if (mSize > mBucketCount * 3 / 4)
                Rehash(mBucketCount << 1);

            return true;
        }

        /**
         * \brief Returns the value associated with a key, creating it if necessary
         *
         * If the key does not exist, the provided function is called to generate
         * a value which is then inserted into the map.
         *
         * \tparam TOtherKey Key type
         * \tparam F Value factory type
         *
         * \param key Key to search for
         * \param function Function used to create a value when the key is missing
         *
         * \return Reference to the existing or newly created value
         */
        template <typename TOtherKey, typename F>
        ValueType& ComputeIfMissing(TOtherKey&& key, F&& function)
        {
            std::size_t hashCode;
            Bucket& bucket = GetBucket(key, hashCode);
            if (Optional<Entry&> entry = bucket.Find(hashCode, key); !entry.IsEmpty())
                return entry.Get().Value;

            FunctionReturnType<F> value = function(key);
            Entry& entry = bucket.Emplace(hashCode, std::forward<TOtherKey>(key), std::move(value));
            ++mSize;

            if (mSize > mBucketCount * 3 / 4)
            {
                Rehash(mBucketCount << 1);
                return TryFindEntry(key).Get().Value;
            }

            return entry.Value;
        }

        /**
         * \brief Replaces the value associated with an existing key
         *
         * \tparam TOtherKey Key type
         * \tparam TOtherValue Value type
         *
         * \param key Key to update
         * \param value New value
         *
         * \return True if the key was found and replaced, false otherwise
         */
        template <typename TOtherKey, typename TOtherValue>
        bool Replace(TOtherKey&& key, TOtherValue&& value)
        {
            return !TryFindEntry(key)
                        .Then([&value](Entry& entry) {
                            entry.Value = std::forward<TOtherValue>(value);
                        })
                        .IsEmpty();
        }

        /**
         * \brief Retrieves the value associated with a key
         *
         * \param key Key to search for
         *
         * \return Pointer to the value if found, nullptr otherwise
         */
        ValueType* Get(const KeyType& key)
        {
            return TryGet(key)
                .Map([](ValueType& value) -> ValueType* {
                    return &value;
                })
                .GetOr(nullptr);
        }

        /**
         * \brief Retrieves the value associated with a key
         *
         * \param key Key to search for
         *
         * \return Pointer to the value if found, nullptr otherwise
         */
        const ValueType* Get(const KeyType& key) const
        {
            return TryGet(key)
                .Map([](const ValueType& value) -> const ValueType* {
                    return &value;
                })
                .GetOr(nullptr);
        }

        /**
         * \brief Attempts to retrieve the value associated with a key
         *
         * \tparam TOtherKey Key type
         *
         * \param key Key to search for
         *
         * \return An Optional containing the value if found
         */
        template <typename TOtherKey>
        Optional<ValueType&> TryGet(TOtherKey&& key)
        {
            return TryFindEntry(key).Map([](Entry& entry) -> ValueType& {
                return entry.Value;
            });
        }

        /**
         * \brief Attempts to retrieve the value associated with a key
         *
         * \tparam TOtherKey Key type
         *
         * \param key Key to search for
         *
         * \return An Optional containing the value if found
         */
        template <typename TOtherKey>
        Optional<const ValueType&> TryGet(TOtherKey&& key) const
        {
            return TryFindEntry(key).Map([](const Entry& entry) -> const ValueType& {
                return entry.Value;
            });
        }

        /**
         * \brief Retrieves the value associated with a key or creates a default one
         *
         * If the key does not exist, a default-constructed value is inserted
         *
         * \tparam TOtherKey Key type
         *
         * \param key Key to search for
         *
         * \return Reference to the existing or newly inserted value
         */
        template <typename TOtherKey>
        ValueType& GetOrAdd(TOtherKey&& key)
        {
            std::size_t hashCode;
            Bucket& bucket = GetBucket(key, hashCode);
            if (Optional<Entry&> entry = bucket.Find(hashCode, key); !entry.IsEmpty())
                return entry.Get().Value;

            Entry& entry = bucket.Emplace(hashCode, std::forward<TOtherKey>(key), ValueType());
            ++mSize;

            if (mSize > mBucketCount * 3 / 4)
            {
                Rehash(mBucketCount << 1);
                return TryFindEntry(key).Get().Value;
            }

            return entry.Value;
        }

        /**
         * \brief Checks whether the specified key exists
         *
         * \tparam TOtherKey Key type
         *
         * \param key Key to search for
         *
         * \return True if the key exists, false otherwise
         */
        template <typename TOtherKey>
        bool Contains(TOtherKey&& key) const
        {
            return !TryFindEntry(key).IsEmpty();
        }

        /**
         * \brief Removes an entry from the map
         *
         * \tparam TOtherKey Key type
         *
         * \param key Key to remove
         *
         * \return True if an entry was removed, false otherwise
         */
        template <typename TOtherKey>
        bool Remove(TOtherKey&& key)
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
         * \brief Applies a function to all entries in the map
         *
         * \tparam TFunction Type of the function
         * \param function Function to apply to each element
         *
         * \warning The \p function should be read-only.
         *          Any insertion/deletion of an element will cause an unpredictable iteration behavior.
         */
        template <typename TFunction>
        void ForEach(TFunction function) const
        {
            static_assert(FunctionTraits<TFunction>::Arity <= 2, "Transform function takes only 0, 1 or 2 parameters");

            for (SizeType i = 0; i < mBucketCount; i++)
            {
                Detail::LinkedListIterator<Entry, AllocatorType> it(mBuckets[i]);
                while (it.IsValid())
                {
                    if constexpr (FunctionTraits<TFunction>::Arity == 0)
                    {
                        function();
                    }
                    else if constexpr (FunctionTraits<TFunction>::Arity == 1)
                    {
                        function(it->Value);
                    }
                    else if constexpr (FunctionTraits<TFunction>::Arity == 2)
                    {
                        function(it->Value, it->Key);
                    }

                    it.Advance();
                }
            }
        }

        /**
         * \brief Returns an iterator pointing to the first element of the map
         *
         * This function provides a convenient way to manually obtain a mutable iterator
         * to the beginning of the hash map. If the map is empty, the returned iterator
         * will be invalid (i.e., \c IsValid() will return false).
         *
         * \return An iterator positioned at the head of the map
         */
        Iterator GetIterator()
        {
            return Iterator(*this);
        }

        /**
         * \brief Returns a constant iterator pointing to the first element of the map
         *
         * This overload is used when the map is accessed through a const reference.
         * It provides read-only traversal of the container. If the map is empty,
         * the returned iterator will be invalid.
         *
         * \return A const iterator positioned at the head of the map
         */
        ConstIterator GetIterator() const
        {
            return ConstIterator(*this);
        }

        /**
         * \brief Returns an iterator pointing to the last element of the map
         *
         * This function provides a convenient way to manually obtain a mutable iterator
         * to the beginning of the hash map. If the map is empty, the returned iterator
         * will be invalid (i.e., \c IsValid() will return false).
         *
         * \return An iterator positioned at the last element of the map
         */
        Iterator GetLastIterator()
        {
            Iterator it(*this, mBucketCount);
            it.Rewind();

            return it;
        }

        /**
         * \brief Returns a constant iterator pointing to the last element of the map
         *
         * This overload is used when the map is accessed through a const reference.
         * It provides read-only traversal of the container. If the map is empty,
         * the returned iterator will be invalid.
         *
         * \return A const iterator positioned at the last element of the map
         */
        ConstIterator GetLastIterator() const
        {
            ConstIterator it(*this, mBucketCount);
            it.Rewind();

            return it;
        }

        /**
         * \brief Returns all values contained in the map
         *
         * \return An array containing all stored values
         */
        Array<ValueType, AllocatorType> GetValues() const
        {
            Array<ValueType, AllocatorType> values;
            values.Reserve(mSize);

            for (SizeType i = 0; i < mBucketCount; i++)
            {
                const Bucket& bucket = mBuckets[i];
                Detail::LinkedListConstIterator<Entry, TAllocator> iterator(bucket);
                while (iterator.IsValid())
                {
                    const Entry& entry = iterator.GetValue();
                    values.Add(entry.Value);

                    iterator = iterator.Next();
                }
            }

            return values;
        }

        /**
         * \brief Ensures that the map can hold at least the specified number of elements
         *
         * May trigger a rehash operation if additional buckets are required.
         *
         * \param capacity Desired element capacity
         */
        void Reserve(SizeType capacity)
        {
            SizeType requiredBucketCount = NextPowerOfTwo(std::max<SizeType>(8, capacity * 4 / 3 + 1));

            if (requiredBucketCount > mBucketCount)
                Rehash(requiredBucketCount);
        }

        /**
         * \brief Removes all entries from the map.
         */
        void Clear()
        {
            for (SizeType i = 0; i < mBucketCount; i++)
            {
                Bucket& bucket = mBuckets[i];
                bucket.Clear();
            }

            mSize = 0;
        }

        /**
         * \brief Returns the number of stored entries
         *
         * \return Current element count
         */
        SizeType GetSize() const
        {
            return mSize;
        }

        /**
         * \brief Returns the number of buckets currently allocated
         *
         * \return Bucket count
         */
        SizeType GetBucketCount() const
        {
            return mBucketCount;
        }

        /**
         * \brief Checks whether the map is empty
         *
         * \return True if the map contains no entries, false otherwise
         */
        bool IsEmpty() const
        {
            return mSize == 0;
        }

        /**
         * \brief Retrieves or creates the value associated with a key
         *
         * Equivalent to calling GetOrAdd().
         *
         * \param key Key to access
         *
         * \return Reference to the associated value
         */
        ValueType& operator[](const KeyType& key)
        {
            return GetOrAdd(key);
        }

    private:

        /**
         * \brief Rebuilds the hash table using a new bucket count
         *
         * All existing entries are redistributed into a newly allocated
         * bucket array according to their stored hash codes. The operation
         * preserves all key-value pairs while changing the bucket layout.
         *
         * \param newBucketCount Number of buckets to allocate
         */
        void Rehash(SizeType newBucketCount)
        {
            UniquePtr<Bucket[]> newBuckets = MakeUnique<Bucket[]>(newBucketCount);

            for (SizeType i = 0; i < mBucketCount; i++)
            {
                Bucket& oldBucket = mBuckets[i];
                Detail::LinkedListIterator<Entry, TAllocator> iterator(oldBucket);
                while (iterator.IsValid())
                {
                    Entry& entry = iterator.GetValue();

                    SizeType newIndex = entry.HashCode & (newBucketCount - 1);
                    newBuckets[newIndex].Emplace(entry.HashCode, std::move(entry.Key), std::move(entry.Value));

                    iterator = iterator.Next();
                }
            }

            mBuckets = std::move(newBuckets);
            mBucketCount = newBucketCount;
        }

        /**
         * \brief Returns the bucket corresponding to the specified key
         *
         * Computes the hash code of the key and determines the target bucket
         * using the current bucket count.
         *
         * \tparam TOtherKey Key type
         *
         * \param key Key whose bucket should be retrieved
         * \param hashCode Receives the computed hash code
         *
         * \return Reference to the corresponding bucket
         */
        template <typename TOtherKey>
        Bucket& GetBucket(TOtherKey&& key, std::size_t& hashCode)
        {
            hashCode = mHasher(key);
            SizeType bucketIndex = hashCode & (mBucketCount - 1);
            return mBuckets[bucketIndex];
        }

        /**
         * \brief Returns the bucket corresponding to the specified key
         *
         * Computes the hash code of the key and determines the target bucket
         * using the current bucket count.
         *
         * \tparam TOtherKey Key type
         *
         * \param key Key whose bucket should be retrieved
         * \param hashCode Receives the computed hash code
         *
         * \return Constant reference to the corresponding bucket
         */
        template <typename TOtherKey>
        const Bucket& GetBucket(TOtherKey&& key, std::size_t& hashCode) const
        {
            hashCode = mHasher(key);
            SizeType bucketIndex = hashCode & (mBucketCount - 1);
            return mBuckets[bucketIndex];
        }

        /**
         * \brief Attempts to locate an entry associated with the specified key
         *
         * Searches the bucket corresponding to the key and returns the matching
         * entry if one exists.
         *
         * \tparam TOtherKey Key type
         * \param key Key to search for
         *
         * \return An Optional containing the matching entry if found,
         *         or an empty Optional otherwise
         */
        template <typename TOtherKey>
        Optional<Entry&> TryFindEntry(TOtherKey&& key)
        {
            std::size_t hashCode;
            return GetBucket(key, hashCode).Find(hashCode, key);
        }

        /**
         * \brief Attempts to locate an entry associated with the specified key
         *
         * Searches the bucket corresponding to the key and returns the matching
         * entry if one exists.
         *
         * \tparam TOtherKey Key type
         * \param key Key to search for
         *
         * \return An Optional containing the matching entry if found,
         *         or an empty Optional otherwise
         */
        template <typename TOtherKey>
        Optional<const Entry&> TryFindEntry(TOtherKey&& key) const
        {
            std::size_t hashCode;
            return GetBucket(key, hashCode).Find(hashCode, key);
        }

        SizeType mSize; ///< Number of entries currently stored in the hash map
        SizeType mBucketCount; ///< Total number of allocated buckets
        THash mHasher; ///< Hash function used to compute key hash codes
        UniquePtr<Bucket[]> mBuckets; ///< Array containing all hash buckets
    };

    template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
    typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::Iterator begin(HashMap<TKey, TValue, THash, TEqual, TAllocator>& hashMap)
    {
        return typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::Iterator(hashMap);
    }

    template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
    typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::ConstIterator begin(const HashMap<TKey, TValue, THash, TEqual, TAllocator>& hashMap)
    {
        return typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::ConstIterator(hashMap);
    }

    template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
    typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::Iterator end(HashMap<TKey, TValue, THash, TEqual, TAllocator>& hashMap)
    {
        return typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::Iterator(hashMap, hashMap.GetBucketCount());
    }

    template <typename TKey, typename TValue, typename THash, typename TEqual, typename TAllocator>
    typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::ConstIterator end(const HashMap<TKey, TValue, THash, TEqual, TAllocator>& hashMap)
    {
        return typename HashMap<TKey, TValue, THash, TEqual, TAllocator>::ConstIterator(hashMap, hashMap.GetBucketCount());
    }
}

#endif // COCKTAIL_CORE_HASHMAP_HPP
