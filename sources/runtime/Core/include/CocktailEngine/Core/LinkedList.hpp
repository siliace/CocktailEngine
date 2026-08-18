#ifndef COCKTAILENGINE_CORE_CONTAINER_LINKEDLIST_HPP
#define COCKTAILENGINE_CORE_CONTAINER_LINKEDLIST_HPP

#include <CocktailEngine/Core/Utility/Detail/LinkedListBase.hpp>

namespace Ck
{
    template <typename E, typename TAllocator>
    class LinkedList;

    /**
     * \class LinkedList
     *
     * \brief A doubly-linked list implementation
     *
     * This class provides a dynamic linked-list container supporting insertion,
     * removal, indexed access, iteration, and search operations.
     *
     * \tparam E The type of elements stored in the list
     * \tparam TAllocator Allocator used for memory management
     */
    template <typename E, typename TAllocator = HeapAllocator>
    class LinkedList : public Detail::LinkedListBase<E, TAllocator>
    {
    public:

        /**
         * \brief Type of elements stored in the LinkedList
         */
        using ElementType = E;

        /**
         * \brief Allocator type used by this linked list
         */
        using AllocatorType = TAllocator;

        /**
         * \brief Type used for sizes and indices
         */
        using SizeType = typename AllocatorType::SizeType;

        /**
         * \brief Iterator used to traverse a LinkedList instance
         */
        using Iterator = Detail::LinkedListIterator<E, TAllocator>;

        /**
         * \brief Constant iterator used to traverse a LinkedList instance
         */
        using ConstIterator = Detail::LinkedListConstIterator<E, TAllocator>;

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
            Detail::LinkedListBase<E, TAllocator>(other),
            mSize(other.mSize)
        {
            /// Nothing
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
            Detail::LinkedListBase<E, TAllocator>(Move(other))
        {
            mSize = std::exchange(other.mSize, 0);
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
                Detail::LinkedListBase<E, TAllocator>::operator=(other);
                mSize = other.mSize;
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
                Detail::LinkedListBase<E, TAllocator>::operator=(Move(other));
                mSize = std::exchange(other.mSize, 0);
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
            Detail::LinkedListBase<E, TAllocator>::AddFront(value);
            ++mSize;
        }

        /**
         * \brief Insert a value at the front of the list by moving it
         *
         * \param value The value to move into the list
         */
        void AddFront(E&& value)
        {
            Detail::LinkedListBase<E, TAllocator>::AddFront(Move(value));
            ++mSize;
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
            E& value = Detail::LinkedListBase<E, TAllocator>::EmplaceFront(Forward<TArgs>(args)...);
            ++mSize;

            return value;
        }

        /**
         * \brief Append a value to the end of the list by copying it
         *
         * \param value The value to move into the list
         */
        void AddBack(const E& value)
        {
            Detail::LinkedListBase<E, TAllocator>::AddBack(value);
            ++mSize;
        }

        /**
         * \brief Append a value to the end of the list by moving it
         *
         * \param value The value to move into the list
         */
        void AddBack(E&& value)
        {
            Detail::LinkedListBase<E, TAllocator>::AddBack(Move(value));
            ++mSize;
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
            E& value = Detail::LinkedListBase<E, TAllocator>::EmplaceBack(Forward<TArgs>(args)...);
            ++mSize;

            return value;
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
            auto node = Detail::LinkedListBase<E, TAllocator>::Insert(where.mNode, value);
            ++mSize;

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
            ConstIterator it(this->GetHead());
            while (it.IsValid())
            {
                if (it.GetValue() == value)
                    return it;

                it.Advance();
            }

            return ConstIterator(nullptr);
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
            ConstIterator it(this->GetTail());
            while (it.IsValid())
            {
                if (it.GetValue() == value)
                    return it;

                it.Rewind();
            }

            return ConstIterator(nullptr);
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

            Detail::LinkedListNode<E, TAllocator>* node = Detail::LinkedListBase<E, TAllocator>::Unlink(where.mNode);

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
            return Iterator(Detail::LinkedListBase<E, TAllocator>::GetHead());
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
            return ConstIterator(Detail::LinkedListBase<E, TAllocator>::GetHead());
        }

        /**
         * \brief Returns an iterator pointing to the last element of the list
         *
         * This function provides a convenient way to manually obtain a mutable iterator
         * to the beginning of the linked list. If the list is empty, the returned iterator
         * will be invalid (i.e., \c IsValid() will return false).
         *
         * \return An iterator positioned at the tail of the list
         */
        Iterator GetLastIterator()
        {
            return Iterator(Detail::LinkedListBase<E, TAllocator>::GetTail());
        }

        /**
         * \brief Returns a constant iterator pointing to the last element of the list
         *
         * This overload is used when the list is accessed through a const reference.
         * It provides read-only traversal of the container. If the list is empty,
         * the returned iterator will be invalid.
         *
         * \return A const iterator positioned at the tail of the list
         */
        ConstIterator GetLastIterator() const
        {
            return ConstIterator(Detail::LinkedListBase<E, TAllocator>::GetTail());
        }

        /**
         * \brief Get an Iterator on the element following the last element of the list
         *
         * This iterator is a placeholder and should not be accessed.
         *
         * \return The Iterator
         */
        Iterator GetEndIterator()
        {
            return Iterator(nullptr);
        }

        /**
         * \brief Get an ConstIterator on the element following the last element of the list
         *
         * This iterator is a placeholder and should not be accessed.
         *
         * \return The ConstIterator
         */
        ConstIterator GetEndIterator() const
        {
            return Iterator(nullptr);
        }

        /**
         * \brief Remove all elements from the list
         */
        void Clear()
        {
            Detail::LinkedListBase<E, TAllocator>::Clear();
            mSize = 0;
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

    template <typename E, typename TAllocator>
    typename LinkedList<E, TAllocator>::Iterator begin(LinkedList<E, TAllocator>& list)
    {
        return list.GetIterator();
    }

    template <typename E, typename TAllocator>
    typename LinkedList<E, TAllocator>::Iterator end(LinkedList<E, TAllocator>& list)
    {
        return list.GetEndIterator();
    }

    template <typename E, typename TAllocator>
    typename LinkedList<E, TAllocator>::ConstIterator begin(const LinkedList<E, TAllocator>& list)
    {
        return list.GetIterator();
    }

    template <typename E, typename TAllocator>
    typename LinkedList<E, TAllocator>::ConstIterator end(const LinkedList<E, TAllocator>& list)
    {
        return list.GetEndIterator();
    }
}

#endif // COCKTAILENGINE_CORE_CONTAINER_LINKEDLIST_HPP
