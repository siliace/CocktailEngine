#ifndef COCKTAIL_CORE_CONTAINER_LINKEDLIST_HPP
#define COCKTAIL_CORE_CONTAINER_LINKEDLIST_HPP

#include <Cocktail/Core/Memory/UniquePtr.hpp>

namespace Ck
{
    template <typename E>
    class LinkedList;

    namespace Detail
    {
        template <typename E>
        class LinkedListIterator;

        template <typename E>
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
         */
        template <typename E>
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
                mValue(std::move(value)),
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
                mValue(std::forward<TArgs>(args)...),
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
            UniquePtr<LinkedListNode> DropNext()
            {
                if (!mNext)
                    return nullptr;

                UniquePtr<LinkedListNode> next = std::move(mNext);
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
            void SetNext(UniquePtr<LinkedListNode> next)
            {
                // We don't want you to use SetNext to drop the rest of the chain.
                // Instead, use DropNext and just ignore the return value.
                assert(mNext == nullptr || next != nullptr);

                if (next)
                    next->mPrevious = this;

                mNext = std::move(next);
            }

        private:

            E mValue; /*!< The stored value */
            LinkedListNode* mPrevious; /*!< Pointer to the previous node (non-owned) */
            UniquePtr<LinkedListNode> mNext; /*!< Owned pointer to the next node in the chain */
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
         */
        template <typename E>
        class LinkedListBase
        {
        protected:

            /**
             * \brief Default constructor
             *
             * Initializes an empty linked structure.
             * Both head and tail pointers are set to nullptr.
             */
            LinkedListBase() :
                mTail(nullptr)
            {
                /// Nothing
            }

            /**
             * \brief Insert a value at the head of the list.
             *
             * \param value The value to copy into the node.
             */
            void AddHead(const E& value)
            {
                EmplaceHead(value);
            }

            /**
             * \brief Insert a value at the head of the list using move semantics.
             *
             * \param value The value to move into the node.
             */
            void AddHead(E&& value)
            {
                EmplaceHead(std::move(value));
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
            E& EmplaceHead(TArgs&&... args)
            {
                UniquePtr<LinkedListNode<E>> head = MakeUnique<LinkedListNode<E>>(InPlace, std::forward<TArgs>(args)...);
                if (mHead == nullptr)
                {
                    assert(mTail == nullptr);

                    mHead = std::move(head);
                    mTail = mHead.Get();
                }
                else
                {
                    head->SetNext(std::move(mHead));
                    mHead = std::move(head);
                }

                return mHead->GetValue();
            }

            /**
             * \brief Insert a value at the tail of the list
             *
             * \param value The value to copy into the node
             */
            void AddTail(const E& value)
            {
                EmplaceTail(value);
            }

            /**
             * \brief Insert a value at the tail using move semantics
             *
             * \param value The value to move into the node
             */
            void AddTail(E&& value)
            {
                EmplaceTail(std::move(value));
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
            E& EmplaceTail(TArgs&&... args)
            {
                UniquePtr<LinkedListNode<E>> tail = MakeUnique<LinkedListNode<E>>(InPlace, std::forward<TArgs>(args)...);
                if (mHead == nullptr)
                {
                    assert(mTail == nullptr);

                    mHead = std::move(tail);
                    mTail = mHead.Get();
                }
                else
                {
                    LinkedListNode<E>* previousTail = mTail;

                    mTail = tail.Get();
                    previousTail->SetNext(std::move(tail));
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
            LinkedListNode<E>* Insert(LinkedListNode<E>* where, const E& value)
            {
                UniquePtr<LinkedListNode<E>> newNode = MakeUnique<LinkedListNode<E>>(value);
                LinkedListNode<E>* inserted = newNode.Get();

                if (where)
                {
                    if (LinkedListNode<E>* previous = where->GetPrevious())
                    {
                        newNode->SetNext(previous->DropNext());
                        previous->SetNext(std::move(newNode));
                    }
                    else
                    {
                        const bool headIsTail = mHead.Get() == mTail;

                        mHead = std::move(newNode);
                        if (headIsTail)
                            mTail = mHead.Get();
                    }
                }
                else
                {
                    if (mTail)
                    {
                        mTail->SetNext(std::move(newNode));
                        mTail = mTail->GetNext();
                    }
                    else
                    {
                        mHead = std::move(newNode);
                        mTail = mHead.Get();
                    }
                }

                return inserted;
            }

            template <typename U>
            friend class LinkedListIterator;

            template <typename U>
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
            LinkedListNode<E>* Unlink(LinkedListNode<E>* node)
            {
                LinkedListNode<E>* previous = node->GetPrevious();
                LinkedListNode<E>* next = node->GetNext();

                if (previous)
                {
                    if (node == mTail)
                        mTail = previous;

                    previous->SetNext(node->DropNext());
                }
                else
                {
                    mHead = std::move(node->DropNext());
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
            LinkedListNode<E>* GetHead() const
            {
                return mHead.Get();
            }

            /**
             * \brief Get the last node in the chain
             *
             * \return A pointer to the tail node, or nullptr if the list is empty
             */
            LinkedListNode<E>* GetTail() const
            {
                return mTail;
            }

        private:

            UniquePtr<LinkedListNode<E>> mHead;
            LinkedListNode<E>* mTail;
        };

        /**
         * \brief Constant iterator used to traverse a LinkedListBase instance
         *
         * This iterator provides read-only access to the elements of a linked list.
         * It internally stores a pointer to a \c LinkedListNode and allows forward
         * and backward navigation within the list.
         *
         * \tparam E The type of elements stored in the list
         */
        template <typename E>
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
            explicit LinkedListConstIterator(const LinkedListBase<E>& list, unsigned int firstIndex = 0) :
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
                    mNode = mNode->GetPrevious();

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
                    mNode = mNode->GetNext();

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
                return LinkedListConstIterator(GetNode()->GetPrevious());
            }

            /**
             * \brief Create a new iterator pointing to the next node
             *
             * \return A new iterator positioned on the next element
             */
            LinkedListConstIterator Next() const
            {
                return LinkedListConstIterator(GetNode()->GetNext());
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
                return Next();
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
                return Previous();
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

            template <typename U>
            friend class Ck::LinkedList;

            /**
             * \brief Internal constructor used by LinkedList classes
             *
             * Creates an iterator directly from a raw node pointer.
             *
             * \param node The node to point to
             */
            explicit LinkedListConstIterator(LinkedListNode<E>* node) :
                mNode(node)
            {
                /// Nothing
            }

            /**
             * \brief Get the underlying node pointer
             *
             * \return The current node
             */
            LinkedListNode<E>* GetNode() const
            {
                return mNode;
            }

        private:

            LinkedListNode<E>* mNode; /*!< Pointer to the current node */
        };

        /**
         * \brief Mutable iterator for LinkedListBase
         *
         * This iterator extends LinkedListConstIterator and provides mutable access
         * to the elements of a linked list. It supports the same navigation features
         * while allowing modification of the referenced value.
         *
         * \tparam E The type of elements stored in the list
         */
        template <typename E>
        class LinkedListIterator : public LinkedListConstIterator<E>
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
                LinkedListConstIterator<E>(nullptr)
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
            explicit LinkedListIterator(LinkedListBase<E>& list, unsigned int firstIndex = 0) :
                LinkedListConstIterator<E>(list, firstIndex)
            {
                /// Nothing
            }

            /**
             * \brief Get a mutable reference to the current value
             *
             * \return A reference to the stored value
             */
            E& GetValue() const
            {
                return LinkedListConstIterator<E>::GetNode()->GetValue();
            }

            /**
             * \brief Create a new iterator pointing to the previous node
             *
             * \return A new mutable iterator positioned on the previous element
             */
            LinkedListIterator Previous() const
            {
                return LinkedListIterator(LinkedListConstIterator<E>::GetNode()->GetPrevious());
            }

            /**
             * \brief Create a new iterator pointing to the next node
             *
             * \return A new mutable iterator positioned on the next element
             */
            LinkedListIterator Next() const
            {
                return LinkedListIterator(LinkedListConstIterator<E>::GetNode()->GetNext());
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
                LinkedListConstIterator<E>::Advance(1);
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
                return Next();
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
                LinkedListConstIterator<E>::Rewind(1);
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
                return Previous();
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
                return &LinkedListConstIterator<E>::GetNode()->GetValue();
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
                return LinkedListConstIterator<E>::GetNode()->GetValue();
            }

        protected:

            template <typename U>
            friend class Ck::LinkedList;

            /**
             * \brief Internal constructor used by LinkedList classes
             *
             * Creates an iterator directly from a raw node pointer.
             *
             * \param node The node to point to
             */
            explicit LinkedListIterator(LinkedListNode<E>* node) :
                LinkedListConstIterator<E>(node)
            {
                /// Nothing
            }
        };
    }

    /**
     * \class LinkedList
     *
     * \brief A doubly-linked list implementation
     *
     * This class provides a dynamic linked-list container supporting insertion,
     * removal, indexed access, iteration, and search operations.
     *
     * \tparam E The type of elements stored in the list
     */
    template <typename E>
    class LinkedList : public Detail::LinkedListBase<E>
    {
    public:

        using SizeType = unsigned int;
        using ElementType = E;

        using Iterator = Detail::LinkedListIterator<E>;
        using ConstIterator = Detail::LinkedListConstIterator<E>;

        /**
         * \brief Default constructor
         *
         * Creates an empty linked list.
         */
        LinkedList() :
            mSize(0)
        {
            /// Nothing
        }

        /**
         * \brief Construct a list with repeated values
         *
         * Creates a new instance of linked list with \p size elements, each initialized to \p value.
         *
         * \param size Number of elements to insert
         * \param value The value to copy into each inserted element
         */
        explicit LinkedList(unsigned int size, const E& value = {}) :
            mSize(0)
        {
            for (unsigned int i = 0; i < size; i++)
                LinkedList::AddBack(value);
        }

        /**
         * \brief Copy constructor
         *
         * Creates a deep copy of another linked list
         *
         * \param other The list to copy
         */
        LinkedList(const LinkedList& other) :
            mSize(0)
        {
            for (ConstIterator it(other); it.IsValid(); it.Advance())
                LinkedList::Add(it.GetValue());
        }

        /**
         * \brief Move constructor
         *
         * Transfers ownership of the nodes from \p other to this list.
         * The \p other list is left empty after the operation.
         *
         * \param other The list to move from
         */
        LinkedList(LinkedList&& other) noexcept :
            mSize(0)
        {
            for (Iterator it(other); it.IsValid();)
            {
                Add(std::move(it.GetValue()));
                it = other.Remove(it);
            }
        }

        /**
         * \brief Copy assignment operator
         *
         * Replaces the contents of this list with a deep copy of \p other.
         *
         * \param other The list to copy
         *
         * \return A reference to this list
         */
        LinkedList& operator=(const LinkedList& other)
        {
            if (this != &other)
            {
                Clear();

                for (ConstIterator it(other); it.IsValid(); it.Advance())
                    Add(it.GetValue());
            }

            return *this;
        }

        /**
         * \brief Move assignment operator
         *
         * Replaces the contents of this list by moving the elements from \p other.
         * The \p other list is left empty afterward.
         *
         * \param other The list to move from
         *
         * \return A reference to this list
         */
        LinkedList& operator=(LinkedList&& other) noexcept
        {
            if (this != &other)
            {
                Clear();

                for (ConstIterator it(other); it.IsValid(); it.Advance())
                {
                    AddTail(std::move(it.GetValue()));
                    it = other.Remove(it);
                }
            }

            return *this;
        }

        /**
         * \brief Insert a value at the front of the list
         *
         * \param value The value to insert
         */
        void AddFront(const E& value)
        {
            ++mSize;
            Detail::LinkedListBase<E>::AddHead(value);
        }

        /**
         * \brief Insert a value at the front of the list by moving it
         *
         * \param value The value to move into the list
         */
        void AddFront(E&& value)
        {
            ++mSize;
            Detail::LinkedListBase<E>::AddHead(std::move(value));
        }

        /**
         * \brief Construct a new element in-place at the front of the list
         *
         * Perfect-forwards the provided arguments to the constructor of \c T.
         *
         * \tparam TArgs Argument types forwarded to the constructor of \c T
         *
         * \param args Arguments forwarded to the new element
         *
         * \return A reference to the newly constructed element
         */
        template <typename... TArgs>
        E& EmplaceFront(TArgs&&... args)
        {
            ++mSize;
            return Detail::LinkedListBase<E>::EmplaceHead(std::forward<TArgs>(args)...);
        }

        /**
         * \brief Append a value to the end of the list by copying it
         *
         * \param value The value to move into the list
         */
        void AddBack(const E& value)
        {
            ++mSize;
            Detail::LinkedListBase<E>::AddTail(value);
        }

        /**
         * \brief Append a value to the end of the list by moving it
         *
         * \param value The value to move into the list
         */
        void AddBack(E&& value)
        {
            ++mSize;
            Detail::LinkedListBase<E>::AddTail(std::move(value));
        }

        /**
         * \brief Construct a new element in-place at the end of the list
         *
         * Perfect-forwards the provided arguments to the constructor of \c T.
         *
         * \tparam TArgs Argument types forwarded to the constructor of \c T
         *
         * \param args Arguments forwarded to the new element
         *
         * \return A reference to the newly constructed element
         */
        template <typename... TArgs>
        E& EmplaceBack(TArgs&&... args)
        {
            ++mSize;
            return Detail::LinkedListBase<E>::EmplaceTail(std::forward<TArgs>(args)...);
        }

        /**
         * \brief Inserts a new element before the position indicated by the given iterator
         *
         * This method creates a new node containing the specified value and inserts it
         * immediately before the element referenced by \p where.
         *
         * The method updates the internal linked-list structure, increments the list
         * size, and returns an iterator pointing to the newly inserted element.
         *
         * \param where Iterator indicating the position before which the element is inserted.
         *              Must be a valid iterator originating from this list.
         *
         * \param value The value to insert.
         *
         * \return Iterator pointing to the newly created element.
         */
        Iterator Insert(Iterator where, const E& value)
        {
            ++mSize;
            auto node = Detail::LinkedListBase<E>::Insert(where.GetNode(), value);

            return Iterator(node);
        }

        /**
         * \brief Finds the first occurrence of the specified value in the list
         *
         * Performs a forward linear search starting from the head of the list and
         * returns an iterator pointing to the first node whose value compares equal to
         * \p value.
         *
         * If no matching element is found, an invalid iterator is returned.
         *
         * \param value The value to search for
         *
         * \return Iterator pointing to the first matching element, or an invalid iterator if not found
         */
        Iterator FindFirst(const E& value)
        {
            Iterator it(this->GetHead());
            while (it.IsValid())
            {
                if (it.GetValue() == value)
                    return it;

                it.Advance();
            }

            return Iterator(nullptr);
        }

        /**
         * \brief Finds the first occurrence of the specified value in the list
         *
         * Performs a forward linear search starting from the head of the list and
         * returns an iterator pointing to the first node whose value compares equal to
         * \p value.
         *
         * If no matching element is found, an invalid iterator is returned.
         *
         * \param value The value to search for
         *
         * \return Iterator pointing to the first matching element, or an invalid iterator if not found
         */
        ConstIterator FindFirst(const E& value) const
        {
            Iterator it(this->GetHead());
            while (it.IsValid())
            {
                if (it.GetValue() == value)
                    return it;

                it.Advance();
            }

            return Iterator(nullptr);
        }

        /**
         * \brief Finds the last occurrence of the specified value in the list
         *
         * Performs a forward linear search starting from the tail of the list and
         * returns an iterator pointing to the first node whose value compares equal to
         * \p value.
         *
         * If no matching element is found, an invalid iterator is returned.
         *
         * \param value The value to search for
         *
         * \return Iterator pointing to the first matching element, or an invalid iterator if not found
         */
        Iterator FindLast(const E& value)
        {
            Iterator it(this->GetTail());
            while (it.IsValid())
            {
                if (it.GetValue() == value)
                    return it;

                it.Rewind();
            }

            return Iterator(nullptr);
        }

        /**
         * \brief Finds the last occurrence of the specified value in the list
         *
         * Performs a forward linear search starting from the tail of the list and
         * returns an iterator pointing to the first node whose value compares equal to
         * \p value.
         *
         * If no matching element is found, an invalid iterator is returned.
         *
         * \param value The value to search for
         *
         * \return Iterator pointing to the first matching element, or an invalid iterator if not found
         */
        ConstIterator FindLast(const E& value) const
        {
            Iterator it(this->GetTail());
            while (it.IsValid())
            {
                if (it.GetValue() == value)
                    return it;

                it.Rewind();
            }

            return Iterator(nullptr);
        }

        /**
         * \brief Remove the element at the position of the provided iterator
         *
         * The iterator becomes invalid after the removal.
         *
         * \param where The iterator pointing to the element to remove
         *
         * \return An iterator to the next element after the removed one
         */
        Iterator Remove(Iterator where)
        {
            assert(where.IsValid());

            Detail::LinkedListNode<E>* node = Detail::LinkedListBase<E>::Unlink(where.GetNode());

            --mSize;

            return Iterator(node);
        }

        /**
         * \brief Returns an iterator pointing to the first element of the list
         *
         * This function provides a convenient way to manually obtain a mutable iterator
         * to the beginning of the linked list. If the list is empty, the returned iterator
         * will be invalid (i.e., \c IsValid() will return false).
         *
         * \return An iterator positioned at the head of the list
         */
        Iterator GetIterator()
        {
            return Iterator(Detail::LinkedListBase<E>::GetHead());
        }

        /**
         * \brief Returns a constant iterator pointing to the first element of the list
         *
         * This overload is used when the list is accessed through a const reference.
         * It provides read-only traversal of the container. If the list is empty,
         * the returned iterator will be invalid.
         *
         * \return A const iterator positioned at the head of the list
         */
        ConstIterator GetIterator() const
        {
            return ConstIterator(Detail::LinkedListBase<E>::GetHead());
        }

        /**
         * \brief Returns an iterator pointing to the first element of the last
         *
         * This function provides a convenient way to manually obtain a mutable iterator
         * to the beginning of the linked list. If the list is empty, the returned iterator
         * will be invalid (i.e., \c IsValid() will return false).
         *
         * \return An iterator positioned at the tail of the list
         */
        Iterator GetLastIterator()
        {
            return Iterator(Detail::LinkedListBase<E>::GetTail());
        }

        /**
         * \brief Returns a constant iterator pointing to the first element of the last
         *
         * This overload is used when the list is accessed through a const reference.
         * It provides read-only traversal of the container. If the list is empty,
         * the returned iterator will be invalid.
         *
         * \return A const iterator positioned at the tail of the list
         */
        ConstIterator GetLastIterator() const
        {
            return Iterator(Detail::LinkedListBase<E>::GetTail());
        }

        /**
         * \brief Remove all elements from the list
         */
        void Clear()
        {
            Iterator it(*this);
            while (it.IsValid())
                it = Remove(it);
        }

        /**
         * \brief Check whether the list is empty
         *
         * \return True if the list contains no elements
         */
        bool IsEmpty() const
        {
            return mSize == 0;
        }

        /**
         * \brief Get the number of elements stored in the list
         *
         * \return The current size of the list
         */
        unsigned int GetSize() const
        {
            return mSize;
        }

    private:

        unsigned int mSize; /*!< The number of node in the chain */
    };

    template <typename E>
    typename LinkedList<E>::Iterator begin(LinkedList<E>& list)
    {
        return list.GetIterator();
    }

    template <typename E>
    typename LinkedList<E>::Iterator end(LinkedList<E>&)
    {
        return typename LinkedList<E>::Iterator(nullptr);
    }

    template <typename E>
    typename LinkedList<E>::ConstIterator begin(const LinkedList<E>& list)
    {
        return list.GetIterator();
    }

    template <typename E>
    typename LinkedList<E>::ConstIterator end(const LinkedList<E>&)
    {
        return typename LinkedList<E>::ConstIterator(nullptr);
    }
}

#endif // COCKTAIL_CORE_CONTAINER_LINKEDLIST_HPP
