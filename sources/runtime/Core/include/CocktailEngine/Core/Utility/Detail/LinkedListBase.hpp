#ifndef COCKTAILENGINE_CORE_UTILITY_DETAIL_LINKEDLISTBASE_HPP
#define COCKTAILENGINE_CORE_UTILITY_DETAIL_LINKEDLISTBASE_HPP

#include <CocktailEngine/Core/Memory/UniquePtr.hpp>

namespace Ck
{
    template <typename U, typename TOtherAllocator>
    class LinkedList;

    namespace Detail
    {
        template <typename E, typename TAllocator>
        class LinkedListIterator;

        template <typename E, typename TAllocator>
        class LinkedListConstIterator;

        /**
         * \class LinkedListNode
         * \brief Internal node type used by LinkedListBase to form the linked structure
         *
         * This class represents a single node in a doubly-linked chain.
         * Each node stores a value of type \c T, a pointer to its previous node,
         * and a unique pointer to its next node.
         *
         * A node **owns** its next node through a `UniquePtr`, meaning destroying a node
         * will also destroy the entire chain that follows it unless `DropNext()` is called.
         *
         * \tparam E The type of the stored value
         * \tparam TAllocator Allocator used for memory management
         */
        template <typename E, typename TAllocator>
        class LinkedListNode
        {
        public:

            /**
             * \brief Constructor by copying an existing object
             *
             * Creates a new node by copying of an already existing value.
             *
             * \param value The value to store inside the node
             */
            explicit LinkedListNode(const E& value) :
                mValue(value),
                mPrevious(nullptr)
            {
                /// Nothing
            }

            /**
             * \brief Constructor by moving an existing object
             *
             * Creates a new node by taking ownership of an already existing value
             * using move semantics.
             *
             * \param value The value to store inside the node
             */
            explicit LinkedListNode(E&& value) :
                mValue(Move(value)),
                mPrevious(nullptr)
            {
                /// Nothing
            }

            /**
             * \brief In-place constructor.
             *
             * Constructs the stored value directly inside the node using the provided arguments.
             * This avoiding temporary objects and allows perfect forwarding.
             *
             * \tparam TArgs Argument types forwarded to the constructor of \c T
             *
             * \param args Arguments forwarded to create the stored value
             */
            template <typename... TArgs>
            explicit LinkedListNode(InPlaceTag, TArgs&&... args) :
                mValue(Forward<TArgs>(args)...),
                mPrevious(nullptr)
            {
                /// Nothing
            }

            /**
             * \brief Release ownership of the next node
             *
             * This detaches the next node (and therefore the remainder of the chain)
             * from this node, transferring ownership to the caller.
             *
             * After this call, the node no longer has a successor.
             *
             * \return A UniquePtr to the former next node (may be null)
             */
            UniquePtr<LinkedListNode, AllocatorAwareDeleter<LinkedListNode, TAllocator>> DropNext()
            {
                if (!mNext)
                    return nullptr;

                UniquePtr<LinkedListNode, AllocatorAwareDeleter<LinkedListNode, TAllocator>> next = Move(mNext);
                next->mPrevious = nullptr;

                mNext.Reset();

                return next;
            }

            /**
             * \brief Access the stored value
             *
             * \return A reference to the stored value
             */
            E& GetValue()
            {
                return mValue;
            }

            /**
             * \brief Access the stored value
             *
             * \return A reference to the stored value
             */
            const E& GetValue() const
            {
                return mValue;
            }

            /**
             * \brief Get the previous node in the chain
             *
             * \return A raw pointer to the previous node, or nullptr if none
             */
            LinkedListNode* GetPrevious() const
            {
                return mPrevious;
            }

            /**
             * \brief Get the next node in the chain
             *
             * \return A raw pointer to the next node, or nullptr if none
             */
            LinkedListNode* GetNext() const
            {
                return mNext.Get();
            }

            /**
             * \brief Set the next node in the chain
             *
             * Takes ownership of the provided node and updates its previous pointer accordingly.
             *
             * \param next A UniquePtr owning the node that will become the next element
             */
            void SetNext(UniquePtr<LinkedListNode, AllocatorAwareDeleter<LinkedListNode, TAllocator>> next)
            {
                // We don't want you to use SetNext to drop the rest of the chain.
                // Instead, use DropNext and just ignore the return value.
                assert(mNext == nullptr || next != nullptr);

                if (next)
                    next->mPrevious = this;

                mNext = Move(next);
            }

        private:

            E mValue; /*!< The stored value */
            LinkedListNode* mPrevious; /*!< Pointer to the previous node (non-owned) */
            UniquePtr<LinkedListNode, AllocatorAwareDeleter<LinkedListNode, TAllocator>> mNext; /*!< Owned pointer to the next node in the chain */
        };

        /**
         * \class LinkedListBase
         *
         * \brief Low-level base class providing core linked-list operations
         *
         * This class manages the structural aspects of a singly- or doubly-linked list,
         * using `LinkedListNode<E>` to form a chain of nodes.
         *
         * It does **not** maintain a size counter or provide high-level list utilities.
         * Instead, it supplies fundamental operations for:
         *  - inserting nodes at the head or tail,
         *  - constructing nodes in-place,
         *  - unlinking (detaching) nodes,
         *  - accessing the first and last nodes.
         *
         * Memory ownership of the entire chain is handled through `UniquePtr`,
         * ensuring proper automatic cleanup.
         *
         * \tparam E The type of value stored in each node
         * \tparam TAllocator Allocator used for memory management
         */
        template <typename E, typename TAllocator>
        class LinkedListBase
        {
        protected:

            /**
             * \brief Allocator type used by this linked list
             */
            using AllocatorType = TAllocator;

            /**
             * \brief Node type linked together in this linked list
             */
            using NodeType = LinkedListNode<E, AllocatorType>;

            /**
             * \brief Allocator rebound to the node type
             */
            using NodeAllocatorType = typename AllocatorType::template ForType<NodeType>;

            /**
             * \brief Specialized unique ptr node with using custom allocator
             */
            using NodeUniquePtrType = UniquePtr<NodeType, AllocatorAwareDeleter<NodeType, AllocatorType>>;

            /**
             * \brief Default constructor
             *
             * Initializes an empty linked structure.
             * Both head and tail pointers are set to nullptr.
             */
            explicit LinkedListBase(const NodeAllocatorType& allocator = NodeAllocatorType()) :
                mTail(nullptr),
                mAllocator(AllocatorUtils::CopyPropagateAllocator(allocator))
            {
                /// Nothing
            }

            /**
             * \brief Copy constructor
             *
             * \param other LinkedListBase to copy from
             */
            LinkedListBase(const LinkedListBase& other) :
                mTail(nullptr),
                mAllocator(AllocatorUtils::CopyPropagateAllocator(other.mAllocator))
            {
                if (other.mHead != nullptr)
                {
                    for (LinkedListConstIterator<E, AllocatorType> it(other); it.IsValid(); it.Advance())
                        EmplaceBack(it.GetValue());
                }
            }

            /**
             * \brief Move constructor
             *
             * \param other LinkedListBase to move from
             */
            LinkedListBase(LinkedListBase&& other) noexcept :
                mTail(nullptr),
                mAllocator(AllocatorUtils::MovePropagateAllocator(other.mAllocator))
            {
                if (other.mHead != nullptr)
                {
                    if constexpr (NodeAllocatorType::PropagateOnContainerMove)
                    {
                        mHead = Move(other.mHead);
                        mTail = std::exchange(other.mTail, nullptr);
                    }
                    else
                    {
                        for (LinkedListIterator<E, AllocatorType> source(other); source.IsValid(); source.Advance())
                            EmplaceBack(Move(source.GetValue()));

                        other.Clear();
                    }
                }
            }

            /**
             * \brief Copy assignment operator
             *
             * \param other LinkedListBase to copy from
             *
             * \return Reference to *this
             */
            LinkedListBase& operator=(const LinkedListBase& other)
            {
                if (this == &other)
                    return *this;

                Clear();

                if constexpr (NodeAllocatorType::PropagateOnContainerCopy)
                    mAllocator = other.mAllocator;

                for (LinkedListConstIterator<E, AllocatorType> it(other); it.IsValid(); it.Advance())
                    EmplaceBack(it.GetValue());

                return *this;
            }

            /**
             * \brief Move assignment operator
             *
             * \param other LinkedListBase to move from
             *
             * \return Reference to *this
             */
            LinkedListBase& operator=(LinkedListBase&& other) noexcept
            {
                if (this == &other)
                    return *this;

                Clear();

                if constexpr (NodeAllocatorType::PropagateOnContainerMove)
                {
                    mHead = Move(other.mHead);
                    mTail = std::exchange(other.mTail, nullptr);
                    mAllocator = Move(other.mAllocator);
                }
                else
                {
                    if constexpr (NodeAllocatorType::AlwaysEqual)
                    {
                        mHead = Move(other.mHead);
                        mTail = std::exchange(other.mTail, nullptr);
                    }
                    else
                    {
                        if (mAllocator == other.mAllocator)
                        {
                            mHead = Move(other.mHead);
                            mTail = std::exchange(other.mTail, nullptr);
                        }
                        else
                        {
                            for (LinkedListIterator<E, AllocatorType> source(other); source.IsValid(); source.Advance())
                                EmplaceBack(Move(source.GetValue()));
                        }
                    }

                    other.Clear();
                }

                return *this;
            }

            /**
             * \brief Insert a value at the head of the list.
             *
             * \param value The value to copy into the node.
             */
            void AddFront(const E& value)
            {
                EmplaceFront(value);
            }

            /**
             * \brief Insert a value at the head of the list using move semantics.
             *
             * \param value The value to move into the node.
             */
            void AddFront(E&& value)
            {
                EmplaceFront(Move(value));
            }

            /**
             * \brief Construct a node in-place and insert it at the head
             *
             * If the list is empty, both head and tail become the new node.
             * Otherwise, the new node becomes the new head and takes ownership
             * of the previous head.
             *
             * \tparam TArgs Argument types forwarded to the node constructor
             *
             * \param args Arguments used to construct the stored value
             *
             * \return A reference to the newly constructed element
             */
            template <typename... TArgs>
            E& EmplaceFront(TArgs&&... args)
            {
                NodeUniquePtrType head = MakeUniqueWithAllocator<NodeType, AllocatorType>(mAllocator, InPlace, Forward<TArgs>(args)...);
                if (mHead == nullptr)
                {
                    assert(mTail == nullptr);

                    mHead = Move(head);
                    mTail = mHead.Get();
                }
                else
                {
                    head->SetNext(Move(mHead));
                    mHead = Move(head);
                }

                return mHead->GetValue();
            }

            /**
             * \brief Insert a value at the tail of the list
             *
             * \param value The value to copy into the node
             */
            void AddBack(const E& value)
            {
                EmplaceBack(value);
            }

            /**
             * \brief Insert a value at the tail using move semantics
             *
             * \param value The value to move into the node
             */
            void AddBack(E&& value)
            {
                EmplaceBack(Move(value));
            }

            /**
             * \brief Construct a node in-place and append it to the tail
             *
             * If the list is empty, the new node becomes both head and tail.
             * Otherwise, it becomes the new tail and is attached to the
             * existing last node.
             *
             * \tparam TArgs Argument types forwarded to the node constructor
             *
             * \param args Arguments used to construct the stored value
             *
             * \return A reference to the newly constructed element
             */
            template <typename... TArgs>
            E& EmplaceBack(TArgs&&... args)
            {
                NodeUniquePtrType tail = MakeUniqueWithAllocator<NodeType, AllocatorType>(mAllocator, InPlace, Forward<TArgs>(args)...);
                if (mHead == nullptr)
                {
                    assert(mTail == nullptr);

                    mHead = Move(tail);
                    mTail = mHead.Get();
                }
                else
                {
                    NodeType* previousTail = mTail;

                    mTail = tail.Get();
                    previousTail->SetNext(Move(tail));
                }

                return mTail->GetValue();
            }

            /**
             * \brief Inserts a new node before the specified position in the linked list
             *
             * This function allocates a new node containing the given value and inserts it
             * into the list at the position defined by \p where.
             *
             * The insertion rules are as follows:
             * - If \p where is a valid node, the new node is inserted immediately before it.
             * - If \p where is nullptr but the list is not empty, the new node is appended at the end.
             * - If \p where is nullptr and the list is empty, the new node becomes both the head and tail.
             *
             * The method correctly updates node links and list endpoints as needed.
             *
             * \param where Pointer to the node before which the new element will be inserted,
             *              or nullptr to insert at the end of the list
             *
             * \param value The value to insert
             *
             * \return A raw pointer to the newly inserted node
             */
            NodeType* Insert(NodeType* where, const E& value)
            {
                NodeUniquePtrType newNode = MakeUniqueWithAllocator<NodeType, AllocatorType>(mAllocator, value);
                NodeType* inserted = newNode.Get();

                if (where)
                {
                    if (NodeType* previous = where->GetPrevious())
                    {
                        newNode->SetNext(previous->DropNext());
                        previous->SetNext(Move(newNode));
                    }
                    else
                    {
                        const bool headIsTail = mHead.Get() == mTail;

                        inserted->SetNext(Move(mHead));
                        mHead = Move(newNode);
                        if (headIsTail)
                            mTail = mHead.Get();
                    }
                }
                else
                {
                    if (mTail)
                    {
                        mTail->SetNext(Move(newNode));
                        mTail = mTail->GetNext();
                    }
                    else
                    {
                        mHead = Move(newNode);
                        mTail = mHead.Get();
                    }
                }

                return inserted;
            }

            /**
             * \brief Remove all elements from the list
             */
            void Clear()
            {
                mHead = nullptr;
                mTail = nullptr;
            }

            template <typename U, typename TOtherAllocator>
            friend class LinkedListIterator;

            template <typename U, typename TOtherAllocator>
            friend class LinkedListConstIterator;

            /**
             * \brief Detach a node from the linked structure
             *
             * This method removes a node from the chain without destroying the
             * remainder of the list. The node’s previous and next neighbors are
             * automatically reconnected.
             *
             * If the removed node is the head, the head pointer is updated.
             * If it is the tail, the tail pointer is updated.
             *
             * \param node The node to remove. Must be a valid node in the chain
             *
             * \return A pointer to the node that follows the removed one
             *         or nullptr if the removed element was the last
             */
            NodeType* Unlink(NodeType* node)
            {
                NodeType* previous = node->GetPrevious();
                NodeType* next = node->GetNext();

                if (previous)
                {
                    if (node == mTail)
                        mTail = previous;

                    NodeUniquePtrType tail = node->DropNext();
                    if (tail != nullptr)
                    {
                        previous->SetNext(Move(tail));
                    }
                    else
                    {
                        // Unlinking the tail leaves nothing to relink, dropping the
                        // successor of the previous node releases the unlinked one
                        previous->DropNext();
                    }
                }
                else
                {
                    mHead = Move(node->DropNext());
                    if (node == mTail)
                        mTail = mHead.Get();
                }

                return next;
            }

            /**
             * \brief Get the first node in the chain
             *
             * \return A pointer to the head node, or nullptr if the list is empty
             */
            NodeType* GetHead() const
            {
                return mHead.Get();
            }

            /**
             * \brief Get the last node in the chain
             *
             * \return A pointer to the tail node, or nullptr if the list is empty
             */
            NodeType* GetTail() const
            {
                return mTail;
            }

        private:

            NodeUniquePtrType mHead;
            NodeType* mTail;
            NodeAllocatorType mAllocator;
        };

        /**
         * \brief Constant iterator used to traverse a LinkedListBase instance
         *
         * This iterator provides read-only access to the elements of a linked list.
         * It internally stores a pointer to a \c LinkedListNode and allows forward
         * and backward navigation within the list.
         *
         * \tparam E The type of elements stored in the list
         * \tparam TAllocator Allocator used for memory management
         */
        template <typename E, typename TAllocator>
        class LinkedListConstIterator
        {
        public:

            /**
             * \brief Constructs an invalid (end) iterator
             *
             * This constructor is used to explicitly create an iterator that represents
             * the end of a traversal. It initializes the internal node pointer to
             * \c nullptr, making \c IsValid() return false. Such an iterator is typically
             * used as a sentinel value, notably by free-standing \c end() functions or
             * when comparing against the end of a range in range-based for loops.
             */
            explicit LinkedListConstIterator(std::nullptr_t) :
                mNode(nullptr)
            {
                /// Nothing
            }

            /**
             * \brief Constructor
             *
             * Creates an iterator positioned at the element located at the given index
             * from the head of the list. If \p firstIndex exceeds the list boundaries,
             * the iterator becomes invalid.
             *
             * \param list The list to iterate through
             *
             * \param firstIndex Number of steps to advance from the head
             */
            explicit LinkedListConstIterator(const LinkedListBase<E, TAllocator>& list, unsigned int firstIndex = 0) :
                mNode(list.GetHead())
            {
                Advance(firstIndex);
            }

            /**
             * \brief Move the iterator backward
             *
             * Moves the iterator to the previous node \p count times. If the beginning
             * of the list is reached, the iterator becomes invalid.
             *
             * \param count Number of steps to move backward
             *
             * \return A reference to this iterator after rewinding
             */
            LinkedListConstIterator& Rewind(unsigned int count = 1)
            {
                for (unsigned int i = 0; i < count; i++)
                {
                    if (mNode == nullptr)
                        break;

                    mNode = mNode->GetPrevious();
                }

                return *this;
            }

            /**
             * \brief Move the iterator forward
             *
             * Moves the iterator to the next node \p count times. If the end of the
             * list is reached, the iterator becomes invalid.
             *
             * \param count Number of steps to move forward
             *
             * \return A reference to this iterator after advancing
             */
            LinkedListConstIterator& Advance(unsigned int count = 1)
            {
                for (unsigned int i = 0; i < count; i++)
                {
                    if (mNode == nullptr)
                        break;

                    mNode = mNode->GetNext();
                }

                return *this;
            }

            /**
             * \brief Check whether the iterator points to a valid node
             *
             * \return \c true if the iterator is valid, \c false otherwise
             */
            bool IsValid() const
            {
                return mNode != nullptr;
            }

            /**
             * \brief Get the value referenced by the iterator
             *
             * \return A constant reference to the stored value
             */
            const E& GetValue() const
            {
                return mNode->GetValue();
            }

            /**
             * \brief Create a new iterator pointing to the previous node
             *
             * \return A new iterator positioned on the previous element
             */
            LinkedListConstIterator Previous() const
            {
                return LinkedListConstIterator(mNode->GetPrevious());
            }

            /**
             * \brief Create a new iterator pointing to the next node
             *
             * \return A new iterator positioned on the next element
             */
            LinkedListConstIterator Next() const
            {
                return LinkedListConstIterator(mNode->GetNext());
            }

            /**
             * \brief Equality comparison operator
             *
             * Two iterators are considered equal if they both point to the same node in
             * the linked list. This also applies to end iterators: two invalid iterators
             * (i.e., iterators with a \c nullptr node) compare equal.
             *
             * \param other The iterator to compare with
             *
             * \return \c true if both iterators reference the same node, otherwise \c false
             */
            bool operator==(const LinkedListConstIterator& other) const
            {
                return mNode == other.mNode;
            }

            /**
             * \brief Inequality comparison operator
             *
             * This operator returns the logical negation of operator==. Two iterators
             * are considered different if they reference different nodes in the list,
             * or if one is valid while the other represents the end iterator.
             *
             * \param other The iterator to compare with
             *
             * \return \c true if the iterators do not reference the same node, otherwise \c false
             */
            bool operator!=(const LinkedListConstIterator& other) const
            {
                return !(*this == other);
            }

            /**
             * \brief Returns a new iterator advanced by \p count positions
             *
             * The iterator is copied and moved forward \p count times using the
             * increment operation. If the end of the list is reached before completing
             * all steps, the resulting iterator becomes invalid.
             *
             * \param count Number of positions to advance
             *
             * \return A new iterator moved forward by \p count steps
             */
            LinkedListConstIterator operator+(unsigned int count) const
            {
                LinkedListConstIterator it(*this);
                it.Advance(count);

                return it;
            }

            /**
             * \brief Returns a new iterator moved backward by \p count positions.
             *
             * The iterator is copied and moved backward \p count times using the
             * decrement operation. If the beginning of the list is exceeded, the
             * resulting iterator becomes invalid.
             *
             * \param count Number of positions to rewind
             * \return A new iterator moved backward by \p count steps
             */
            LinkedListConstIterator operator-(unsigned int count) const
            {
                LinkedListConstIterator it(*this);
                it.Rewind(count);

                return it;
            }

            /**
             * \brief Advances the iterator to the next element
             *
             * Moves the iterator to the next node in the linked list. If the iterator is
             * already invalid (i.e., at the end), it remains invalid. The updated iterator
             * is returned, matching standard forward-iterator semantics.
             *
             * \return A reference to this iterator after incrementing
             */
            LinkedListConstIterator& operator++()
            {
                Advance(1);
                return *this;
            }

            /**
             * \brief Advances the iterator to the next element
             *
             * Returns a copy of the iterator before it was incremented, then moves this
             * iterator to the next node. This follows the usual semantics of the postfix
             * increment operator.
             *
             * \return A copy of the iterator prior to incrementing
             */
            LinkedListConstIterator operator++(int)
            {
                LinkedListConstIterator old = *this;
                Advance();

                return old;
            }

            /**
             * \brief Moves the iterator to the previous element
             *
             * Decrements the iterator by moving it to the previous node. If the iterator
             * is invalid or already at the beginning, the result is implementation-defined,
             * but generally the iterator becomes invalid. This operation is only valid if
             * the iterator type supports backward navigation.
             *
             * \return A reference to this iterator after decrementing
             */
            LinkedListConstIterator& operator--()
            {
                Rewind(1);
                return *this;
            }

            /**
             * \brief Moves the iterator to the previous element
             *
             * Returns a copy of the iterator before it was decremented, then moves this
             * iterator to the previous node. Follows standard postfix decrement semantics.
             *
             * \return A copy of the iterator prior to decrementing
             */
            LinkedListConstIterator operator--(int)
            {
                LinkedListConstIterator old = *this;
                Rewind();

                return old;
            }

            /**
             * \brief Provides read-only pointer access to the stored value
             *
             * This operator enables pointer-style access to the element referenced by the
             * iterator.
             *
             * Calling this operator on an invalid iterator (i.e., when the iterator does not
             * reference a node) results in undefined behavior.
             *
             * \return A pointer to the constant value stored in the current node
             */
            const E* operator->() const
            {
                return &mNode->GetValue();
            }

            /**
             * \brief Dereferences the iterator and returns a read-only reference to the value
             *
             * This operator provides access to the value stored in the node pointed to by the
             * iterator. Calling this operator on an invalid iterator (i.e., when \ref IsValid
             * returns false) results in undefined behavior.
             *
             * \return A constant reference to the value of the current node
             */
            const E& operator*() const
            {
                return mNode->GetValue();
            }

        protected:

            template <typename U, typename TOtherAllocator>
            friend class Ck::LinkedList;

            /**
             * \brief Internal constructor used by LinkedList classes
             *
             * Creates an iterator directly from a raw node pointer.
             *
             * \param node The node to point to
             */
            explicit LinkedListConstIterator(LinkedListNode<E, TAllocator>* node) :
                mNode(node)
            {
                /// Nothing
            }

            LinkedListNode<E, TAllocator>* mNode; /*!< Pointer to the current node */
        };

        /**
         * \brief Mutable iterator for LinkedListBase
         *
         * This iterator extends LinkedListConstIterator and provides mutable access
         * to the elements of a linked list. It supports the same navigation features
         * while allowing modification of the referenced value.
         *
         * \tparam E The type of elements stored in the list
         * \tparam TAllocator Allocator used for memory management
         */
        template <typename E, typename TAllocator>
        class LinkedListIterator : public LinkedListConstIterator<E, TAllocator>
        {
        public:

            /**
             * \brief Constructs an invalid (end) iterator
             *
             * This constructor is used to explicitly create an iterator that represents
             * the end of a traversal. It initializes the internal node pointer to
             * \c nullptr, making \c IsValid() return false. Such an iterator is typically
             * used as a sentinel value, notably by free-standing \c end() functions or
             * when comparing against the end of a range in range-based for loops.
             */
            explicit LinkedListIterator(std::nullptr_t) :
                LinkedListConstIterator<E, TAllocator>(nullptr)
            {
                /// Nothing
            }

            /**
             * \brief Constructor
             *
             * Creates an iterator positioned at the element located at the given index
             * from the head of the list.
             *
             * \param list The list to iterate through
             * \param firstIndex Number of steps to advance from the head
             */
            explicit LinkedListIterator(LinkedListBase<E, TAllocator>& list, unsigned int firstIndex = 0) :
                LinkedListConstIterator<E, TAllocator>(list, firstIndex)
            {
                /// Nothing
            }

            /**
             * \brief Move the iterator backward
             *
             * Moves the iterator to the previous node \p count times. If the beginning
             * of the list is reached, the iterator becomes invalid.
             *
             * \param count Number of steps to move backward
             *
             * \return A reference to this iterator after rewinding
             */
            LinkedListIterator& Rewind(unsigned int count = 1)
            {
                for (unsigned int i = 0; i < count; i++)
                {
                    if (this->mNode == nullptr)
                        break;

                    this->mNode = this->mNode->GetPrevious();
                }

                return *this;
            }

            /**
             * \brief Move the iterator forward
             *
             * Moves the iterator to the next node \p count times. If the end of the
             * list is reached, the iterator becomes invalid.
             *
             * \param count Number of steps to move forward
             *
             * \return A reference to this iterator after advancing
             */
            LinkedListIterator& Advance(unsigned int count = 1)
            {
                for (unsigned int i = 0; i < count; i++)
                {
                    if (this->mNode == nullptr)
                        break;

                    this->mNode = this->mNode->GetNext();
                }

                return *this;
            }

            /**
             * \brief Get a mutable reference to the current value
             *
             * \return A reference to the stored value
             */
            E& GetValue() const
            {
                return this->mNode->GetValue();
            }

            /**
             * \brief Create a new iterator pointing to the previous node
             *
             * \return A new mutable iterator positioned on the previous element
             */
            LinkedListIterator Previous() const
            {
                return LinkedListIterator(this->mNode->GetPrevious());
            }

            /**
             * \brief Create a new iterator pointing to the next node
             *
             * \return A new mutable iterator positioned on the next element
             */
            LinkedListIterator Next() const
            {
                return LinkedListIterator(this->mNode->GetNext());
            }

            /**
             * \brief Returns a new iterator advanced by \p count positions
             *
             * The iterator is copied and moved forward \p count times using the
             * increment operation. If the end of the list is reached before completing
             * all steps, the resulting iterator becomes invalid.
             *
             * \param count Number of positions to advance
             *
             * \return A new iterator moved forward by \p count steps
             */
            LinkedListIterator operator+(unsigned int count) const
            {
                LinkedListIterator it(*this);
                it.Advance(count);

                return it;
            }

            /**
             * \brief Returns a new iterator moved backward by \p count positions.
             *
             * The iterator is copied and moved backward \p count times using the
             * decrement operation. If the beginning of the list is exceeded, the
             * resulting iterator becomes invalid.
             *
             * \param count Number of positions to rewind
             * \return A new iterator moved backward by \p count steps
             */
            LinkedListIterator operator-(unsigned int count) const
            {
                LinkedListIterator it(*this);
                it.Rewind(count);

                return it;
            }

            /**
             * \brief Advances the iterator to the next element
             *
             * Moves the iterator to the next node in the linked list. If the iterator is
             * already invalid (i.e., at the end), it remains invalid. The updated iterator
             * is returned, matching standard forward-iterator semantics.
             *
             * \return A reference to this iterator after incrementing
             */
            LinkedListIterator& operator++()
            {
                LinkedListConstIterator<E, TAllocator>::Advance(1);
                return *this;
            }

            /**
             * \brief Advances the iterator to the next element
             *
             * Returns a copy of the iterator before it was incremented, then moves this
             * iterator to the next node. This follows the usual semantics of the postfix
             * increment operator.
             *
             * \return A copy of the iterator prior to incrementing
             */
            LinkedListIterator operator++(int)
            {
                LinkedListIterator old = *this;
                Advance();

                return old;
            }

            /**
             * \brief Moves the iterator to the previous element
             *
             * Decrements the iterator by moving it to the previous node. If the iterator
             * is invalid or already at the beginning, the result is implementation-defined,
             * but generally the iterator becomes invalid. This operation is only valid if
             * the iterator type supports backward navigation.
             *
             * \return A reference to this iterator after decrementing
             */
            LinkedListIterator& operator--()
            {
                LinkedListConstIterator<E, TAllocator>::Rewind(1);
                return *this;
            }

            /**
             * \brief Moves the iterator to the previous element
             *
             * Returns a copy of the iterator before it was decremented, then moves this
             * iterator to the previous node. Follows standard postfix decrement semantics.
             *
             * \return A copy of the iterator prior to decrementing
             */
            LinkedListIterator operator--(int)
            {
                LinkedListIterator old = *this;
                Rewind();

                return old;
            }

            /**
             * \brief Provides access to the stored value
             *
             * This operator enables pointer-style access to the element referenced by the
             * iterator.
             *
             * Calling this operator on an invalid iterator (i.e., when the iterator does not
             * reference a node) results in undefined behavior.
             *
             * \return A pointer to the constant value stored in the current node
             */
            E* operator->() const
            {
                return &this->mNode->GetValue();
            }

            /**
             * \brief Dereferences the iterator and returns a reference to the value
             *
             * This operator provides access to the value stored in the node pointed to by the
             * iterator. Calling this operator on an invalid iterator (i.e., when \ref IsValid
             * returns false) results in undefined behavior.
             *
             * \return A constant reference to the value of the current node
             */
            E& operator*() const
            {
                return this->mNode->GetValue();
            }

        protected:

            template <typename U, typename TOtherAllocator>
            friend class Ck::LinkedList;

            /**
             * \brief Internal constructor used by LinkedList classes
             *
             * Creates an iterator directly from a raw node pointer.
             *
             * \param node The node to point to
             */
            explicit LinkedListIterator(LinkedListNode<E, TAllocator>* node) :
                LinkedListConstIterator<E, TAllocator>(node)
            {
                /// Nothing
            }
        };
    }
}

#endif // COCKTAILENGINE_CORE_UTILITY_DETAIL_LINKEDLISTBASE_HPP
