#ifndef COCKTAIL_CORE_ARRAY_HPP
#define COCKTAIL_CORE_ARRAY_HPP

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>
#include <Cocktail/Core/Utility/FunctionTraits.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>

namespace Ck
{
    COCKTAIL_DECLARE_EXCEPTION_FROM(ArrayEmpty, LogicException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(ArrayOutOfRange, LogicException);

    /**
     * \brief Template class implementing a dynamic array similar to std::vector.
     * \tparam E The element type stored in the array.
     * \tparam TAllocator The allocator type used for memory management.
     */
    template <typename E, typename TAllocator = SizedHeapAllocator<32>>
    class Array
    {
        template <typename, typename>
        friend class Array;

    public:

        using ElementType = E;
        using AllocatorType = TAllocator;

        /**
         * \brief Type used for size and indices, defined by the allocator.
         */
        using SizeType = typename TAllocator::SizeType;

        using ElementAllocatorType = std::conditional_t<
            TAllocator::IsTyped,
            typename TAllocator::template Typed<E>,
            typename TAllocator::Raw
        >;

        using Iterator = ElementType*;
        using ConstIterator = const ElementType*;

        /**
         * \brief Default constructor. Creates an empty array.
         */
        Array() :
            mSize(0)
        {
            mCapacity = mAllocator.GetInitialCapacity();
        }

        /**
         * \brief Constructs the array with an initializer list.
         * \param values The initializer list of elements to initialize the array.
         */
        Array(std::initializer_list<E> values) :
            mSize(0)
        {
            mCapacity = mAllocator.GetInitialCapacity();
            Append(values);
        }

        /**
         * \brief Constructs the array with an initial size
         * Initializing elements are default constructed.
         * \param initialSize Number of elements initially allocated
         */
        explicit Array(SizeType initialSize) :
            mSize(0)
        {
            mCapacity = mAllocator.GetInitialCapacity();
            Resize(initialSize);
        }

        /**
         * \brief Constructs the array with an initial size, initializing elements with a given value.
         * \param initialSize Number of elements initially allocated.
         * \param initialContent Value to initialize each element with (default constructed if omitted).
         */
        template <typename U, typename = std::enable_if_t<std::is_constructible_v<E, U>>>
        Array(SizeType initialSize, const U& initialContent) :
            mSize(0)
        {
            mCapacity = mAllocator.GetInitialCapacity();
            Resize(initialSize, initialContent);
        }

        /**
         * \brief Constructs the array from a raw pointer and element count.
         * \param elements Pointer to the array of elements.
         * \param elementCount Number of elements to copy.
         */
        Array(const E* elements, SizeType elementCount) :
            mSize(0)
        {
            mCapacity = mAllocator.GetInitialCapacity();
            Append(elements, elementCount);
        }

        /**
         * \brief Copy constructor.
         * \param other Array to copy from.
         */
        Array(const Array& other) :
			mSize(0)
        {
            mCapacity = mAllocator.GetInitialCapacity();
            Append(other);
        }

        /**
         * \brief Move constructor.
         * \param other Array to move from.
         */
        Array(Array&& other) noexcept :
			mSize(0)
        {
            mSize = std::exchange(other.mSize, 0);
            mCapacity = std::exchange(other.mCapacity, 0);
            mAllocator = std::move(other.mAllocator);
        }

        /**
         * \brief Destructor. Releases allocated resources.
         */
        ~Array()
        {
            Clear();
        }

        /**
         * \brief Copy assignment operator.
         * \param other Array to copy from.
         * \return Reference to *this.
         */
        Array& operator=(const Array& other)
        {
            if (this == &other)
                return *this;

            Clear();
            Append(other);

            return *this;
        }

        /**
         * \brief Move assignment operator.
         * \param other Array to move from.
         * \return Reference to *this.
         */
        Array& operator=(Array&& other) noexcept
        {
            if (this == &other)
                return *this;

            Clear();
            mSize = std::exchange(other.mSize, 0);
            mCapacity = std::exchange(other.mCapacity, 0);
            mAllocator = std::move(other.mAllocator);

            return *this;
        }

        /**
         * \brief Adds an element to the end of the array (copy).
         * \param element Element to add.
         */
        void Add(const E& element)
        {
            Emplace(element);
        }

        /**
         * \brief Adds an element to the end of the array (move).
         * \param element Element to move and add.
         */
        void Add(E&& element)
        {
            Emplace(std::move(element));
        }

        /**
         * \brief Inserts an element at the specified index (copy).
         * Elements from the index onward are shifted.
         * \param index Position to insert at.
         * \param element Element to insert.
         */
        void AddAt(SizeType index, const E& element)
        {
            EmplaceAt(index, element);
        }

        /**
         * \brief Inserts an element at the specified index (move).
         * Elements from the index onward are shifted.
         * \param index Position to insert at.
         * \param element Element to move and insert.
         */
        void AddAt(SizeType index, E&& element)
        {
            EmplaceAt(index, std::move(element));
        }

        /**
         * \brief Constructs an element in place at the end of the array.
         * \tparam TArgs Types of constructor arguments.
         * \param args Arguments forwarded to the element constructor.
         */
        template <typename... TArgs>
    	E& Emplace(TArgs&&... args)
        {
            E* availableElements = Allocate(1);
            AllocatorUtils::Construct(availableElements, std::forward<TArgs>(args)...);
            ++mSize;

            return availableElements[0];
        }

        /**
         * \brief Constructs an element in place at the specified index.
         * Elements from the index onward are shifted.
         * \tparam TArgs Types of constructor arguments.
         * \param index Position to emplace at.
         * \param args Arguments forwarded to the element constructor.
         */
        template <typename... TArgs>
    	E& EmplaceAt(SizeType index, TArgs&&... args)
        {
            if (index == mSize)
                return Emplace(std::forward<TArgs>(args)...);

            CheckIndex(index);

            Allocate(1);

            ElementType* data = GetData();
            for (SizeType i = mSize; i > index; --i)
            {
                ElementType& current = data[i];
                ElementType& previous = data[i - 1];

                AllocatorUtils::Construct(&current, std::move(previous));
                AllocatorUtils::Destroy(&previous);
            }

            ElementType& element = data[index];
            AllocatorUtils::Construct(&element, std::forward<TArgs>(args)...);
            ++mSize;

            return element;
        }

        /**
         * \brief Inserts elements from another array at the specified index (copy).
         * \param index Position to insert at.
         * \param other Array to insert.
         */
        void Insert(SizeType index, const Array& other)
        {
            if (other.IsEmpty())
                return;

            Insert(index, other.GetData(), other.GetSize());
        }

        /**
         * \brief Inserts elements from an initializer list at the specified index.
         * \param index Position to insert at.
         * \param values Initializer list of elements to insert.
         */
        void Insert(SizeType index, std::initializer_list<E> values)
        {
            CheckIndex(index);

            SizeType elementCount = values.size();
            Reserve(elementCount);

            ElementType* data = GetData();

            for (SizeType i = 0; i < elementCount; ++i)
                AllocatorUtils::Construct(&data[mSize + i], std::move(data[index + i]));

            AllocatorUtils::DestroyRange(elementCount, data + index);

            SizeType i = 0;
            for (auto it = values.begin(); it != values.end(); ++it)
            {
                AllocatorUtils::Construct(&data[index + i], std::move(*it));
                ++i;
            }

            mSize += elementCount;
        }

        /**
         * \brief Inserts elements from a raw pointer at the specified index
         * \param index Position to insert at
         * \param elements Pointer to elements to insert
         * \param elementCount Number of elements to insert
         */
        void Insert(SizeType index, const E* elements, SizeType elementCount)
        {
            CheckIndex(index);

            Reserve(elementCount);

            ElementType* data = GetData();

            for (SizeType i = 0; i < elementCount; ++i)
                AllocatorUtils::Construct(&data[mSize + i], std::move(data[index + i]));

            AllocatorUtils::DestroyRange(elementCount, data + index);

        	for (SizeType i = 0; i < elementCount; ++i)
                AllocatorUtils::Construct(&data[index + i], std::move(elements[i]));

            mSize += elementCount;
        }

        /**
         * \brief Appends elements from another array (copy).
         * \param other Array to append.
         */
        void Append(const Array& other)
        {
            if (other.IsEmpty())
                return;

            Append(other.GetData(), other.GetSize());
        }

        /**
         * \brief Appends elements from an initializer list.
         * \param values Initializer list of elements to append.
         */
        void Append(std::initializer_list<E> values)
        {
            SizeType count = static_cast<SizeType>(values.size());

            E* availableElements = Allocate(count);
            for (auto it = values.begin(); it != values.end(); ++it)
            {
                AllocatorUtils::Construct(availableElements, *it);
                ++availableElements;
            }

            mSize += count;
        }

        /**
         * \brief Appends elements from a raw pointer.
         * \param elements Pointer to elements to append.
         * \param elementCount Number of elements to append.
         */
        void Append(const E* elements, SizeType elementCount)
        {
            E* availableElements = Allocate(elementCount);
            for (SizeType i = 0; i < elementCount; ++i)
            {
                AllocatorUtils::Construct(availableElements, elements[i]);
                ++availableElements;
            }

            mSize += elementCount;
        }

        /**
         * \brief Returns a reference to the element at the given index.
         * Throws if out of bounds.
         * \param index Index of the element.
         * \return Reference to the element.
         */
        E& At(SizeType index)
        {
            CheckIndex(index);

            E* element = GetData() + index;
            return *element;
        }

        /**
         * \brief Returns a const reference to the element at the given index.
         * Throws if out of bounds.
         * \param index Index of the element.
         * \return Const reference to the element.
         */
        const E& At(SizeType index) const
        {
            CheckIndex(index);

            const E* element = GetData() + index;
            return *element;
        }

        /**
         * \brief Returns an Optional reference to the element at the given index if valid.
         * \param index Index of the element.
         * \return Optional reference to the element or empty if out of bounds.
         */
        Optional<E&> TryAt(SizeType index)
        {
            if (index >= mSize)
                return Optional<E&>::Empty();

            E* element = GetData() + index;
            return Optional<E&>::Of(*element);
        }

        /**
         * \brief Returns an Optional const reference to the element at the given index if valid.
         * \param index Index of the element.
         * \return Optional const reference or empty if out of bounds.
         */
        Optional<const E&> TryAt(SizeType index) const
        {
            if (index >= mSize)
                return Optional<const E&>::Empty();

            const E* element = GetData() + index;
            return Optional<const E&>::Of(*element);
        }

        /**
         * \brief Returns a reference to the first element.
         * Throws if empty.
         * \return Reference to the first element.
         */
        E& First()
        {
            return TryFirst().template GetOrThrow<ArrayEmpty>();
        }

        /**
         * \brief Returns a const reference to the first element.
         * Throws if empty.
         * \return Const reference to the first element.
         */
        const E& First() const
        {
            return TryFirst().template GetOrThrow<ArrayEmpty>();
        }

        /**
         * \brief Returns an Optional reference to the first element if the array is not empty.
         * \return Optional reference or empty if empty.
         */
        Optional<E&> TryFirst()
        {
            if (mSize == 0)
                return Optional<E&>::Empty();

            return Optional<E&>::Of(At(0));
        }

        /**
         * \brief Returns an Optional const reference to the first element if the array is not empty.
         * \return Optional const reference or empty if empty.
         */
        Optional<const E&> TryFirst() const
        {
            if (mSize == 0)
                return Optional<const E&>::Empty();

            return Optional<const E&>::Of(At(0));
        }

        /**
         * \brief Returns a reference to the last element.
         * Throws if empty.
         * \return Reference to the last element.
         */
        E& Last()
        {
            return TryLast().template GetOrThrow<ArrayEmpty>();
        }

        /**
         * \brief Returns a const reference to the last element.
         * Throws if empty.
         * \return Const reference to the last element.
         */
        const E& Last() const
        {
            return TryLast().template GetOrThrow<ArrayEmpty>();
        }

        /**
         * \brief Returns an Optional reference to the last element if the array is not empty.
         * \return Optional reference or empty if empty.
         */
        Optional<E&> TryLast()
        {
            if (mSize == 0)
                return Optional<E&>::Empty();

            return Optional<E&>::Of(At(mSize - 1));
        }

        /**
         * \brief Returns an Optional const reference to the last element if the array is not empty.
         * \return Optional const reference or empty if empty.
         */
        Optional<const E&> TryLast() const
        {
            if (mSize == 0)
                return Optional<const E&>::Empty();

            return Optional<const E&>::Of(At(mSize - 1));
        }

        /**
         * \brief Swaps the contents of this array with another.
         * \param other Array to swap with.
         */
        void Swap(Array& other)
        {
            std::swap(mSize, other.mSize);
            std::swap(mCapacity, other.mCapacity);
            std::swap(mAllocator, other.mAllocator);
        }

        /**
         * \brief Swaps two elements in the array at the specified indices.
         * \param lhsIndex Index of the first element.
         * \param rhsIndex Index of the second element.
         */
        void Swap(SizeType lhsIndex, SizeType rhsIndex)
        {
            E* lhs = GetData() + lhsIndex;
            E* rhs = GetData() + rhsIndex;
            std::swap(*lhs, *rhs);
        }

        /**
         * \brief Checks if the array contains the specified element.
         * \param element Element to search for.
         * \return true if found, false otherwise.
         */
        bool Contains(const E& element) const
        {
            return !FindIndex(element).IsEmpty();
        }

        /**
         * \brief Checks if any element satisfies the predicate.
         * \tparam TPredicate Type of the predicate function.
         * \param predicate Predicate to apply to elements.
         * \return true if any element satisfies predicate, false otherwise.
         */
        template <typename TPredicate>
    	bool ContainsIf(TPredicate predicate) const
        {
            return !FindIndexIf(predicate).IsEmpty();
        }

        /**
         * \brief
         * \tparam TPredicate
         * \param predicate
         * \return
         */
        template <typename TPredicate>
        bool AnyOf(TPredicate predicate) const
        {
            for (SizeType i = 0; i < mSize; ++i)
            {
                const ElementType& element = At(i);
                if (predicate(element))
                    return true;
            }

            return false;
        }

        /**
         * \brief
         * \tparam TPredicate
         * \param predicate
         * \return
         */
        template <typename TPredicate>
        bool AllOf(TPredicate predicate) const
        {
            for (SizeType i = 0; i < mSize; ++i)
            {
                const ElementType& element = At(i);
                if (!predicate(element))
                    return false;
            }

            return true;
        }

        /**
         * \brief Finds the index of the first occurrence of the element.
         * \param element Element to find.
         * \return Optional index if found, empty otherwise.
         */
        Optional<SizeType> FindIndex(const E& element) const
        {
            for (SizeType i = 0; i < mSize; ++i)
            {
                if (At(i) == element)
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Finds the index of the last occurrence of the element.
         * \param element Element to find.
         * \return Optional index if found, empty otherwise.
         */
        Optional<SizeType> FindLastIndex(const E& element) const
        {
            for (SizeType i = mSize; i-- > 0;)
            {
                if (At(i) == element)
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Finds the index of the first occurrence of the element.
         * \tparam TPredicate Type of the predicate function.
         * \param predicate Predicate to apply.
         * \return Optional index if found, empty otherwise.
         */
        template <typename TPredicate>
        Optional<SizeType> FindIndexIf(TPredicate predicate) const
        {
            for (SizeType i = 0; i < mSize; ++i)
            {
                const E& element = At(i);
                if (predicate(element))
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Finds the index of the last occurrence of the element.
         * \tparam TPredicate Type of the predicate function.
         * \param predicate Predicate to apply.
         * \return Optional index if found, empty otherwise.
         */
        template <typename TPredicate>
        Optional<SizeType> FindLastIndexIf(TPredicate predicate) const
        {
            for (SizeType i = mSize; i-- > 0;)
            {
                const E& element = At(i);
                if (predicate(element))
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Finds the pointer to the first element satisfying a predicate.
         * \tparam TPredicate Type of the predicate function.
         * \param predicate Predicate to apply.
         * \return Pointer to element if found, nullptr otherwise.
         */
        template <typename TPredicate>
    	Optional<E&> FindIf(TPredicate predicate)
        {
            for (SizeType i = 0; i < mSize; ++i)
            {
                E& element = At(i);
                if (predicate(element))
                    return Optional<E&>::Of(element);
            }

            return Optional<E&>::Empty();
        }

        /**
         * \brief Finds the pointer to the first element satisfying a predicate.
         * \tparam TPredicate Type of the predicate function.
         * \param predicate Predicate to apply.
         * \return Pointer to element if found, nullptr otherwise.
         */
        template <typename TPredicate>
    	Optional<const E&> FindIf(TPredicate predicate) const
        {
            return const_cast<Array<E, TAllocator>*>(this)->FindIf(predicate);
        }

        /**
         * \brief Finds the pointer to the last element satisfying a predicate.
         * \tparam TPredicate Type of the predicate function.
         * \param predicate Predicate to apply.
         * \return Pointer to element if found, nullptr otherwise.
         */
        template <typename TPredicate>
    	E* FindLastIf(TPredicate predicate)
        {
            for (SizeType i = mSize; i-- > 0;)
            {
                E& element = At(i);
                if (predicate(element))
                    return &element;
            }

            return nullptr;
        }

        /**
         * \brief Finds the pointer to the last element satisfying a predicate.
         * \tparam TPredicate Type of the predicate function.
         * \param predicate Predicate to apply.
         * \return Pointer to element if found, nullptr otherwise.
         */
        template <typename TPredicate>
    	const E* FindLastIf(TPredicate predicate) const
        {
            return const_cast<Array<E, TAllocator>*>(this)->FindLastIf(predicate);
        }

        /**
         * \brief Removes and returns the first element.
         * \return The removed element.
         */
        E PopFirst()
        {
            CheckSize();

            ElementType* data = GetData();
            ElementType& first = data[0];

            ElementType firstElement = std::move(first);
            for (SizeType i = 1; i < mSize; ++i)
                data[i - 1] = std::move(data[i]);

            ElementType& last = data[mSize - 1];
            AllocatorUtils::Destroy(&last);

            --mSize;

            return firstElement;
        }

        /**
         * \brief Removes and returns the last element.
         * \return The removed element.
         */
        E PopLast()
        {
            CheckSize();

            ElementType* data = GetData();
            ElementType& last = data[mSize - 1];

            ElementType lastElement = std::move(last);
            AllocatorUtils::Destroy(&last);

            --mSize;

            return lastElement;
        }

        /**
         * \brief Removes the element at the specified index.
         * \param index Index of the element to remove.
         * \return The removed element.
         */
        E RemoveAt(SizeType index)
        {
            if (index == 0)
            {
                return PopFirst();
            }
            else if (index == mSize - 1)
            {
                return PopLast();
            }
            else
            {
                E removed = std::move(At(index));
                for (SizeType i = index + 1; i < mSize; ++i)
                    At(i - 1) = std::move(At(i));

                (mAllocator.GetAllocation() + mSize - 1)->~E();

                --mSize;

                return removed;
            }
        }

        /**
         * \brief Removes all occurrences of the specified element.
         * \param toRemove Element to remove.
         * \return Number of elements removed.
         */
        SizeType Remove(const E& toRemove)
        {
            SizeType removed = 0;
            FilterInPlace([&](const E& element) {
                const bool remove = element == toRemove;
                if (remove)
                    ++removed;

                return remove;
            });

            return removed;
        }

        /**
         * \brief Returns a new array containing elements satisfying the predicate.
         * \tparam TPredicate Type of the predicate function.
         * \param predicate Predicate to test elements.
         * \return Filtered array.
         */
        template <typename TPredicate>
    	Array<E, AllocatorType> Filter(TPredicate predicate) const
        {
            Array<E, AllocatorType> results(*this);
            results.FilterInPlace(predicate);

            return results;
        }

        /**
         * \brief Filters the array in place, keeping only elements satisfying the predicate.
         * \tparam TPredicate Type of the predicate function.
         * \param predicate Predicate to test elements.
         * \return Reference to *this.
         */
        template <typename TPredicate>
    	Array<E, AllocatorType>& FilterInPlace(TPredicate predicate)
        {
            SizeType writeIndex = 0;
            ElementType* data = GetData();

            for (SizeType readIndex = 0; readIndex < mSize; ++readIndex)
            {
                ElementType& readElement = data[readIndex];
                ElementType& writeElement = data[writeIndex];

                if (!predicate(readElement))
                {
                    if (writeIndex != readIndex) 
                    {
                        AllocatorUtils::Construct(&writeElement, std::move(readElement));
                        AllocatorUtils::Destroy(&readElement);
                    }
                    ++writeIndex;
                }
                else
                {
                    AllocatorUtils::Destroy(&readElement);
                }
            }

            mSize = writeIndex;

            return *this;
        }

        /**
         * \brief Applies a transformation function to each element and returns a new array.
         * \tparam TFunction Type of the transformer function.
         * \param transformer Function to apply to each element.
         * \return Transformed array.
         */
        template <typename TFunction>
    	Array<typename FunctionTraits<TFunction>::Return> Transform(TFunction transformer) const
        {
            using T = typename FunctionTraits<TFunction>::Return;
            static_assert(FunctionTraits<TFunction>::Arity <= 2, "Transform function takes only 0, 1 or 2 parameters");

            Array<T> transformed;
            transformed.Reserve(mSize);
            for (SizeType i = 0; i < mSize; ++i)
            {
                const E& element = At(i);

                if constexpr (FunctionTraits<TFunction>::Arity == 0)
                {
                    T transformedElement = transformer();
                    transformed.Add(std::move(transformedElement));
                }
                else if constexpr (FunctionTraits<TFunction>::Arity == 1)
                {
                    T transformedElement = transformer(element);
                    transformed.Add(std::move(transformedElement));
                }
                else if constexpr (FunctionTraits<TFunction>::Arity == 2)
                {
                    T transformedElement = transformer(element, i);
                    transformed.Add(std::move(transformedElement));
                }
            }

            return transformed;
        }

        /**
         * \brief Applies a transformation function to each element in place.
         * \tparam TFunction Type of the transformer function.
         * \param transformer Function to apply to each element.
         * \return Reference to *this.
         */
        template <typename TFunction>
    	Array<E, AllocatorType>& TransformInPlace(TFunction transformer)
        {
            for (SizeType i = 0; i < mSize; ++i)
            {
                E& element = At(i);
                element = transformer(element);
            }

            return *this;
        }

        /**
         * \brief Applies a function to all elements in the array
         *
         * \tparam TFunction Type of the function
         * \param function Function to apply to each element
         *
         * \note The iteration order is the same as the element order in the array.
         *
         * \warning The \p function should be read-only.
         *          Any insertion/deletion of element will cause an unpredictable iteration behaviour.
         */
        template <typename TFunction>
    	void ForEach(TFunction function) const
        {
            static_assert(FunctionTraits<TFunction>::Arity <= 2, "Transform function takes only 0, 1 or 2 parameters");

            for (SizeType i = 0; i < mSize; ++i)
            {
                if constexpr (FunctionTraits<TFunction>::Arity == 0)
                {
                    function();
                }
                else if constexpr (FunctionTraits<TFunction>::Arity == 1)
                {
                    const E& element = At(i);
                    function(element);
                }
                else if constexpr (FunctionTraits<TFunction>::Arity == 2)
                {
                    const E& element = At(i);
                    function(element, i);
                }
            }
        }

        /**
         * \brief Applies a reduction operation to all elements in the array
         *
         * This method accumulates a value by successively applying a binary function
         * to each element of the array, starting from an initial value.
         *
         * \tparam T The type of the accumulated value (the result).
         * \tparam TFunction The type of the reduction function.
         *         It must be callable with the signature:
         *         (T accumulated, const ElementType& current) -> T
         *
         * \param initialValue The starting value for the reduction process.
         * \param function A binary function that combines the accumulated value with each
         *        array element. It must return the updated accumulated value.
         *
         * \return The final accumulated result after applying the reduction function to all elements.
         *
         * \note The iteration order is the same as the element order in the array.
         *
         * \warning The \p function should ideally be associative and side-effect free to ensure predictable behavior.
         *
         * \example
         * Array<int> array = {1, 2, 3, 4};
         * int sum = array.Reduce(0, [](int acc, int value) {
         *     return acc + value;
         * });
         * // sum == 10
         */
        template <typename T, typename TFunction>
        T Reduce(T initialValue, TFunction function) const
        {
            ForEach([&](const ElementType& element) {
                initialValue = function(std::move(initialValue), element);
            });

            return initialValue;
        }

        /**
         * \brief Returns a reversed copy of the array.
         * \return New reversed array.
         */
        Array<E, AllocatorType> Reverse() const
        {
            Array<E, AllocatorType> copy = *this;
            copy.ReverseInPlace();
            return copy;
        }

        /**
         * \brief Reverses the array in place.
         * \return Reference to *this.
         */
        Array<E, AllocatorType>& ReverseInPlace()
        {
            const SizeType size = GetSize();
            if (size > 1)
            {
                const SizeType lastIndex = size - 1;
                for (SizeType i = 0; i < size / 2; ++i)
                    Swap(i, lastIndex - i);
            }

            return *this;
        }

        /**
         * \brief Returns a new array which is a slice from the first index to the end.
         * \param first Start index of the slice.
         * \return Sliced array.
         */
        Array<E, AllocatorType> Slice(SizeType first) const
        {
            Array<E, AllocatorType> sliced(*this);
            sliced.SliceInPlace(first);

            return sliced;
        }

        /**
         * \brief Removes elements before the first index in place.
         * \param first Start index of the slice.
         * \return Reference to *this.
         */
        Array<E, AllocatorType>& SliceInPlace(SizeType first)
        {
            return SliceInPlace(first, mSize - first);
        }

        /**
         * \brief Returns a new array which is a slice of specified count starting at first index.
         * \param first Start index of the slice.
         * \param count Number of elements in the slice.
         * \return Sliced array.
         */
        Array<E, AllocatorType> Slice(SizeType first, SizeType count) const
        {
            Array<E, AllocatorType> sliced(*this);
            sliced.SliceInPlace(first, count);

            return sliced;
        }

        /**
         * \brief Removes elements outside the slice in place.
         * \param first Start index of the slice.
         * \param count Number of elements in the slice.
         * \return Reference to *this.
         */
        Array<E, AllocatorType>& SliceInPlace(SizeType first, SizeType count)
        {
            CheckIndex(first + count - 1);
            
            const SizeType firstKept = first + count;

            AllocatorUtils::DestroyRange(count, GetData() + first);
            for (SizeType i = 0; i < mSize - first; ++i)
            {
                E* element = GetData() + firstKept + i;
                AllocatorUtils::Construct(GetData() + first + i, std::move(*element));
                AllocatorUtils::Destroy(element);
            }

            mSize -= count;

            return *this;
        }

        /**
         * \brief Resizes the array to the given size
         * New elements are default constructed.
         * If the new size is lower than the current size, right most elements will be destroyed.
         * \param size New size of the array
         */
        void Resize(SizeType size)
        {
            if (size > mSize)
            {
                SizeType allocateCount = size - mSize;
                E* availableElements = Allocate(allocateCount);
                for (SizeType i = 0; i < size; ++i)
                    AllocatorUtils::ConstructRange(allocateCount, availableElements);

                mSize += allocateCount;
            }
            else if (size < mSize)
            {
                SizeType destroyCount = mSize - size;
                AllocatorUtils::DestroyRange(destroyCount, GetData() + size);

                mSize -= destroyCount;
            }
        }

        /**
         * \brief Resizes the array to the given size
         * New elements are constructed by copying \p element.
         * \param size New size of the array
         * \param element Element to initialize new elements with
         */
        template <typename U, typename = std::enable_if_t<std::is_constructible_v<E, U>>>
        void Resize(SizeType size, const U& element)
        {
            if (size > mSize)
            {
                SizeType allocateCount = size - mSize;
                E* availableElements = Allocate(allocateCount);
                for (SizeType i = 0; i < size; ++i)
                    ConstructRange(allocateCount, availableElements, element);

                mSize += allocateCount;
            }
            else if (size < mSize)
            {
                SizeType destroyCount = mSize - size;
                DestroyRange(destroyCount, GetData() + size);

                mSize -= destroyCount;
            }
        }

        /**
         * \brief Reserves additional capacity for at least the specified size
         * \param size Capacity to reserve
         */
        void Reserve(SizeType size)
        {
            SizeType nextCapacity = NextPowerOfTwo(mCapacity + size);
            mAllocator.ResizeAllocation(mSize, nextCapacity, sizeof(E));

            mCapacity = nextCapacity;
        }

        /**
         * \brief Clears the array, removing all elements.
         */
        void Clear()
        {
            if (mSize > 0)
                AllocatorUtils::DestroyRange(mSize, mAllocator.GetAllocation());
                
            mSize = 0;
        }

        /**
         * \brief Shrinks capacity to fit the current size.
         */
        void Shrink()
        {
            mAllocator.ResizeAllocation(mSize, mSize, sizeof(E));
            mCapacity = mSize;
        }

        /**
         * \brief Returns the number of elements in the array.
         * \return Number of elements.
         */
        SizeType GetSize() const
        {
            return mSize;
        }

        /**
         * \brief Returns the current capacity of the array.
         * \return Capacity.
         */
        SizeType GetCapacity() const
        {
            return mCapacity;
        }

        /**
         * \brief Checks if the array is empty.
         * \return true if empty, false otherwise.
         */
        bool IsEmpty() const
        {
            return mSize == 0;
        }

        /**
         * \brief Returns a pointer to the underlying data.
         * \return Pointer to data.
         */
        E* GetData()
        {
            return mAllocator.GetAllocation();
        }

        /**
         * \brief Returns a const pointer to the underlying data.
         * \return Const pointer to data.
         */
        const E* GetData() const
        {
            return mAllocator.GetAllocation();
        }

        E* begin()
        {
            return GetData();
        }

        const E* begin() const
        {
            return GetData();
        }

        E* end()
        {
            return GetData() + GetSize();
        }

        const E* end() const
        {
            return GetData() + GetSize();
        }

        /**
         * \brief Equality comparison.
         * \param other Array to compare with.
         * \return true if equal, false otherwise.
         */
        bool operator==(const Array& other) const
        {
            if (this == &other)
                return true;

            if (mSize != other.GetSize())
                return false;

            for (SizeType i = 0; i < mSize; ++i)
            {
                if (At(i) != other.At(i))
                    return false;
            }

            return true;
        }

        /**
         * \brief Inequality comparison.
         * \param other Array to compare with.
         * \return true if different, false otherwise.
         */
        bool operator!=(const Array& other) const
        {
            return !(*this == other);
        }

        /**
         * \brief
         * \param
         * \return
         */
        E& operator[](SizeType index)
        {
            E* element = GetData() + index;
            return *element;
        }

        /**
         * \brief
         * \param
         * \return
         */
        const E& operator[](SizeType index) const
        {
            const E* element = GetData() + index;
            return *element;
        }

    private:

        void CheckSize() const
        {
            if (mSize == 0)
                throw ArrayEmpty();
        }

        /**
         * \brief 
         * \param index 
         */
        void CheckIndex(SizeType index) const
        {
            CheckSize();

            if (index >= mSize)
                throw ArrayOutOfRange("Index {} is out of array size {}", index, mSize);
        }

        /**
         * \brief Allocate memory to fit at least \p count elements
         * \param count The number of new elements that should fit in the Array
         * \return A pointer to the first element available
         */
        E* Allocate(SizeType count)
        {
            SizeType nextCapacity = NextPowerOfTwo(mSize + count);
            if (nextCapacity > mCapacity)
            {
                mAllocator.ResizeAllocation(mSize, nextCapacity, sizeof(E));
                mCapacity = nextCapacity;
            }

            E* allocation = mAllocator.GetAllocation();
            return allocation + mSize;
        }

        SizeType mSize;
        SizeType mCapacity;
        ElementAllocatorType mAllocator;
    };

    /**
     * \brief 
     * \tparam E 
     */
    template <typename E>
    using Array64 = Array<E, SizedHeapAllocator<64>>;
}

#endif // COCKTAIL_CORE_ARRAY_HPP
