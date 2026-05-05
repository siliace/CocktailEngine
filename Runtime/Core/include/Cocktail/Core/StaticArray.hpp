#ifndef COCKTAIL_CORE_STATICARRAY_HPP
#define COCKTAIL_CORE_STATICARRAY_HPP

#include <Cocktail/Core/Utility/ExceptionUtils.hpp>
#include <Cocktail/Core/Utility/FunctionTraits.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>

namespace Ck
{
    /**
     * \brief Fixed-size array container with utility algorithms
     *
     * StaticArray is a lightweight container storing a fixed number of elements
     * known at compile time. It provides direct access to its underlying storage
     * along with a set of utility methods for querying, searching, and transforming
     * elements.
     *
     * Unlike dynamic containers, its size cannot change at runtime.
     *
     * \tparam E Element type
     * \tparam TSize Number of elements in the array
     */
    template <typename E, unsigned int TSize>
    class StaticArray
    {
    public:

        using ElementType = E; /*!< Element type stored in the array */
        using SizeType = unsigned int; /*!< Type used for indexing */

        using Iterator = ElementType*; /*!< Mutable iterator type */
        using ConstIterator = const ElementType*; /*!< Const iterator type */

        /**
         * \brief Default constructor
         *
         * Elements are default-initialized.
         */
        constexpr StaticArray() = default;

        /**
         * \brief Constructs the array with an initializer list
         *
         * Initializes elements in order using the provided values.
         * Remaining elements (if any) are default-initialized.
         *
         * \tparam U Types of initializer values
         *
         * \param values Values used to initialize the array
         */
        template <typename... U>
        explicit constexpr StaticArray(U&&... values) :
            mElements{ std::forward<U>(values)... }
        {
            static_assert(sizeof...(U) <= TSize, "Too many initializers for StaticArray");
        }

        /**
         * \brief Fills the array with a given value
         *
         * \param value Value assigned to all elements
         */
        void Fill(const ElementType& value)
        {
            for (SizeType i = 0; i < TSize; ++i)
                mElements[i] = value;
        }

        /**
         * \brief Accesses an element with bounds checking
         *
         * \param index Index of the element
         *
         * \return Reference to the element
         *
         * \throw OutOfRangeException if index is invalid
         */
        ElementType& At(SizeType index)
        {
            CheckIndex(index);
            return UncheckedAt(index);
        }

        /**
         * \brief Accesses an element with bounds checking
         *
         * \param index Index of the element
         *
         * \return Reference to the element
         *
         * \throw OutOfRangeException if index is invalid
         */
        const ElementType& At(SizeType index) const
        {
            CheckIndex(index);
            return UncheckedAt(index);
        }

        /**
         * \brief Attempts to access an element safely with bounds checking
         *
         * \param index Index of the element
         *
         * \return Optional containing the element if valid, otherwise empty
         */
        Optional<E&> TryAt(SizeType index)
        {
            if (index >= TSize)
                return Optional<E&>::Empty();

            return Optional<E&>::Of(UncheckedAt(index));
        }

        /**
         * \brief Attempts to access an element safely with bounds checking
         *
         * \param index Index of the element
         *
         * \return Optional containing the element if valid, otherwise empty
         */
        Optional<const E&> TryAt(SizeType index) const
        {
            if (index >= TSize)
                return Optional<const E&>::Empty();

            return Optional<const E&>::Of(UncheckedAt(index));
        }

        /**
         * \brief Retrieves the first element
         *
         * \return Reference to the first element
         */
        constexpr E& First()
        {
            return mElements[0];
        }

        /**
         * \brief Retrieves the first element
         *
         * \return Reference to the first element
         */
        constexpr const E& First() const
        {
            return mElements[0];
        }

        /**
         * \brief Safely retrieves the first element
         *
         * \return Optional containing the first element
         */
        Optional<E&> TryFirst()
        {
            return Optional<E&>::Of(mElements[0]);
        }

        /**
         * \brief Safely retrieves the first element
         *
         * \return Optional containing the first element
         */
        Optional<const E&> TryFirst() const
        {
            return Optional<const E&>::Of(mElements[0]);
        }

        /**
         * \brief Retrieves the last element
         *
         * \return Reference to the last element
         */
        constexpr E& Last()
        {
            return mElements[TSize - 1];
        }

        /**
         * \brief Retrieves the last element
         *
         * \return Reference to the last element
         */
        constexpr const E& Last() const
        {
            return mElements[TSize - 1];
        }

        /**
         * \brief Safely retrieves the last element
         *
         * \return Optional containing the last element if available
         */
        Optional<E&> TryLast()
        {
            if (IsEmpty())
                return Optional<E&>::Empty();

            return Optional<E&>::Of(At(TSize - 1));
        }

        /**
         * \brief Safely retrieves the last element
         *
         * \return Optional containing the last element if available
         */
        Optional<const E&> TryLast() const
        {
            if (IsEmpty())
                return Optional<const E&>::Empty();

            return Optional<const E&>::Of(At(TSize - 1));
        }

        /**
         * \brief Checks whether the array contains a given element
         *
         * \param element Element to search for
         *
         * \return True if found, false otherwise
         */
        bool Contains(const E& element) const
        {
            return !FindIndex(element).IsEmpty();
        }

        /**
         * \brief Checks whether any element satisfies a predicate
         *
         * \param predicate Predicate evaluating elements
         *
         * \return True if found, false otherwise
         */
        template <typename TPredicate>
        bool ContainsIf(TPredicate predicate) const
        {
            return !FindIndexIf(predicate).IsEmpty();
        }

        /**
         * \brief Checks whether at least one element satisfies a predicate
         *
         * \param predicate Predicate evaluating elements
         *
         * \return True if an element that satisfies the predicate has been found, false otherwise
         */
        template <typename TPredicate>
        bool AnyOf(TPredicate predicate) const
        {
            for (SizeType i = 0; i < TSize; ++i)
            {
                const ElementType& element = mElements[i];
                if (predicate(element))
                    return true;
            }

            return false;
        }

        /**
         * \brief Checks whether at all elements satisfies a predicate
         *
         * \param predicate Predicate evaluating elements
         *
         * \return True if all elements satisfies the predicate, false otherwise
         */
        template <typename TPredicate>
        bool AllOf(TPredicate predicate) const
        {
            for (SizeType i = 0; i < TSize; ++i)
            {
                const ElementType& element = mElements[i];
                if (!predicate(element))
                    return false;
            }

            return true;
        }

        /**
         * \brief Finds the index of the first occurrence of an element
         *
         * \param element Element to search for
         * \param start Starting index
         *
         * \return Optional containing the index if found
         */
        Optional<SizeType> FindIndex(const E& element, SizeType start = 0) const
        {
            assert(start <= TSize);

            for (SizeType i = start; i < TSize; ++i)
            {
                if (mElements[i] == element)
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Finds the index of the last occurrence of an element
         *
         * \param element Element to search for
         *
         * \return Optional containing the index if found
         */
        Optional<SizeType> FindLastIndex(const E& element) const
        {
            for (SizeType i = TSize; i-- > 0;)
            {
                if (mElements[i] == element)
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }


        /**
         * \brief Finds the first index matching a predicate
         *
         * \tparam TPredicate Predicate type
         * \param predicate Function used to test elements
         * \param start Starting index for the search
         *
         * \return Optional containing the index if found, otherwise empty
         */
        template <typename TPredicate>
        Optional<SizeType> FindIndexIf(TPredicate predicate, SizeType start = 0) const
        {
            assert(start <= TSize);

            for (SizeType i = start; i < TSize; ++i)
            {
                const E& element = mElements[i];
                if (predicate(element))
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Finds the last index matching a predicate
         *
         * \tparam TPredicate Predicate type
         * \param predicate Function used to test elements
         *
         * \return Optional containing the last index if found, otherwise empty
         */
        template <typename TPredicate>
        Optional<SizeType> FindLastIndexIf(TPredicate predicate) const
        {
            for (SizeType i = TSize; i-- > 0;)
            {
                const E& element = mElements[i];
                if (predicate(element))
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Finds the first element matching a predicate
         *
         * \tparam TPredicate Predicate type
         * \param predicate Function used to test elements
         * \param start Starting index for the search
         *
         * \return Optional containing a reference to the element if found
         */
        template <typename TPredicate>
        Optional<E&> FindIf(TPredicate predicate, SizeType start = 0)
        {
            assert(start <= TSize);

            for (SizeType i = start; i < TSize; ++i)
            {
                E& element = mElements[i];
                if (predicate(element))
                    return Optional<E&>::Of(element);
            }

            return Optional<E&>::Empty();
        }

        /**
         * \brief Finds the first element matching a predicate
         *
         * \tparam TPredicate Predicate type
         * \param predicate Function used to test elements
         * \param start Starting index for the search
         *
         * \return Optional containing a reference to the element if found
         */
        template <typename TPredicate>
        Optional<const E&> FindIf(TPredicate predicate, SizeType start = 0) const
        {
            assert(start <= TSize);

            for (SizeType i = start; i < TSize; ++i)
            {
                const E& element = mElements[i];
                if (predicate(element))
                    return Optional<const E&>::Of(element);
            }

            return Optional<const E&>::Empty();
        }

        /**
         * \brief Finds the last element matching a predicate
         *
         * \tparam TPredicate Predicate type
         * \param predicate Function used to test elements
         *
         * \return Optional containing a reference to the element if found
         */
        template <typename TPredicate>
        Optional<E&> FindLastIf(TPredicate predicate)
        {
            for (SizeType i = TSize; i-- > 0;)
            {
                E& element = mElements[i];
                if (predicate(element))
                    return Optional<E&>::Of(element);
            }

            return Optional<E&>::Empty();
        }

        /**
         * \brief Finds the last element matching a predicate
         *
         * \tparam TPredicate Predicate type
         * \param predicate Function used to test elements
         *
         * \return Optional containing a reference to the element if found
         */
        template <typename TPredicate>
        Optional<const E&> FindLastIf(TPredicate predicate) const
        {
            for (SizeType i = TSize; i-- > 0;)
            {
                const E& element = mElements[i];
                if (predicate(element))
                    return Optional<const E&>::Of(element);
            }

            return Optional<const E&>::Empty();
        }

        /**
         * \brief Transforms the array using a mapping function
         *
         * The function can take:
         * - no parameters
         * - the element
         * - the element and its index
         *
         * \param transformer Transformation function
         *
         * \return A new StaticArray containing transformed elements
         */
        template <typename TFunction>
        StaticArray<typename FunctionTraits<TFunction>::Return, TSize> Map(TFunction transformer) const
        {
            using T = typename FunctionTraits<TFunction>::Return;
            static_assert(FunctionTraits<TFunction>::Arity <= 2, "Transform function takes only 0, 1 or 2 parameters");

            StaticArray<T, TSize> transformed;
            for (SizeType i = 0; i < TSize; ++i)
            {
                if constexpr (FunctionTraits<TFunction>::Arity == 0)
                {
                    transformed[i] = transformer();
                }
                else if constexpr (FunctionTraits<TFunction>::Arity == 1)
                {
                    transformed[i] = transformer(mElements[i]);
                }
                else if constexpr (FunctionTraits<TFunction>::Arity == 2)
                {
                    transformed[i] = transformer(mElements[i], i);
                }
            }

            return transformed;
        }

        /**
         * \brief Applies a function to each element
         *
         * The function can take:
         * - no parameters
         * - the element
         * - the element and its index
         *
         * \param function Function applied to each element
         */
        template <typename TFunction>
        void ForEach(TFunction function) const
        {
            static_assert(FunctionTraits<TFunction>::Arity <= 2, "Transform function takes only 0, 1 or 2 parameters");

            for (SizeType i = 0; i < TSize; ++i)
            {
                if constexpr (FunctionTraits<TFunction>::Arity == 0)
                {
                    function();
                }
                else if constexpr (FunctionTraits<TFunction>::Arity == 1)
                {
                    function(mElements[i]);
                }
                else if constexpr (FunctionTraits<TFunction>::Arity == 2)
                {
                    function(mElements[i], i);
                }
            }
        }

        /**
         * \brief Reverse the array
         *
         * \return The created reversed array
         */
        StaticArray Reverse() const
        {
            StaticArray result;
            for (SizeType i = 0; i < TSize; ++i)
                result[i] = mElements[TSize - 1 - i];

            return result;
        }

        /**
         * \brief Returns the number of elements
         */
        constexpr SizeType GetSize() const noexcept
        {
            return TSize;
        }

        Iterator GetIterator()
        {
            return mElements;
        }

        ConstIterator GetIterator() const
        {
            return mElements;
        }

        Iterator GetLastIterator()
        {
            return &mElements[TSize - 1];
        }

        ConstIterator GetLastIterator() const
        {
            return &mElements[TSize - 1];
        }

        /**
         * \brief Indicates whether the array is empty
         *
         * \return Always false since the size is fixed at compile time
         */
        constexpr bool IsEmpty() const noexcept
        {
            return false;
        }

        /**
         * \brief Returns a pointer to the underlying data
         */
        constexpr E* GetData() noexcept
        {
            return mElements;
        }

        /**
         * \brief Returns a pointer to the underlying data
         */
        constexpr const E* GetData() const noexcept
        {
            return mElements;
        }


        /**
         * \brief Equality comparison operator
         */
        bool operator==(const StaticArray& other) const
        {
            if (this == &other)
                return true;

            for (SizeType i = 0; i < TSize; ++i)
            {
                if (mElements[i] != other.mElements[i])
                    return false;
            }

            return true;
        }

        bool operator!=(const StaticArray& other) const
        {
            return !(*this == other);
        }

        ElementType& operator[](SizeType index)
        {
            return UncheckedAt(index);
        }

        const ElementType& operator[](SizeType index) const
        {
            return UncheckedAt(index);
        }

    private:

        static void CheckIndex(SizeType index)
        {
            if (index >= TSize)
                ExceptionUtils::ThrowOutOfRange(index, TSize);
        }

        constexpr E& UncheckedAt(SizeType index)
        {
            return mElements[index];
        }

        constexpr const E& UncheckedAt(SizeType index) const
        {
            return mElements[index];
        }

        ElementType mElements[TSize];
    };

    template <typename E>
    class StaticArray<E, 0>
    {
    public:

        using ElementType = E;
        using SizeType = unsigned int;

        using Iterator = ElementType*;
        using ConstIterator = const ElementType*;

        void Fill(const ElementType&)
        {
        }

        ElementType& At(SizeType)
        {
            ExceptionUtils::ThrowEmptyContainer();
        }

        const ElementType& At(SizeType) const
        {
            ExceptionUtils::ThrowEmptyContainer();
        }

        Optional<E&> TryAt(SizeType)
        {
            return Optional<E&>::Empty();
        }

        Optional<const E&> TryAt(SizeType) const
        {
            return Optional<const E&>::Empty();
        }

        constexpr E& First()
        {
            ExceptionUtils::ThrowEmptyContainer();
        }

        constexpr const E& First() const
        {
            ExceptionUtils::ThrowEmptyContainer();
        }

        Optional<E&> TryFirst()
        {
            return Optional<E&>::Empty();
        }

        Optional<const E&> TryFirst() const
        {
            return Optional<const E&>::Empty();
        }

        constexpr E& Last()
        {
            ExceptionUtils::ThrowEmptyContainer();
        }

        constexpr const E& Last() const
        {
            ExceptionUtils::ThrowEmptyContainer();
        }

        constexpr Optional<E&> TryLast()
        {
            return Optional<E&>::Empty();
        }

        constexpr Optional<const E&> TryLast() const
        {
            return Optional<const E&>::Empty();
        }

        constexpr bool Contains(const E&) const
        {
            return false;
        }

        template <typename TPredicate>
        constexpr bool ContainsIf(TPredicate) const
        {
            return false;
        }

        template <typename TPredicate>
        constexpr bool AnyOf(TPredicate) const
        {
            return false;
        }

        template <typename TPredicate>
        constexpr bool AllOf(TPredicate) const
        {
            return true;
        }

        Optional<SizeType> FindIndex(const E&, SizeType = 0) const
        {
            return Optional<SizeType>::Empty();
        }

        Optional<SizeType> FindLastIndex(const E&) const
        {
            return Optional<SizeType>::Empty();
        }

        template <typename TPredicate>
        Optional<SizeType> FindIndexIf(TPredicate, SizeType = 0) const
        {
            return Optional<SizeType>::Empty();
        }

        template <typename TPredicate>
        Optional<SizeType> FindLastIndexIf(TPredicate) const
        {
            return Optional<SizeType>::Empty();
        }

        template <typename TPredicate>
        Optional<E&> FindIf(TPredicate, SizeType = 0)
        {
            return Optional<E&>::Empty();
        }

        template <typename TPredicate>
        Optional<const E&> FindIf(TPredicate, SizeType = 0) const
        {
            return Optional<E&>::Empty();
        }

        template <typename TPredicate>
        Optional<E&> FindLastIf(TPredicate)
        {
            return Optional<E&>::Empty();
        }

        template <typename TPredicate>
        Optional<const E&> FindLastIf(TPredicate) const
        {
            return Optional<const E&>::Empty();
        }

        template <typename TFunction>
        StaticArray<typename FunctionTraits<TFunction>::Return, 0> Transform(TFunction) const
        {
            static_assert(FunctionTraits<TFunction>::Arity <= 2, "Transform function takes only 0, 1 or 2 parameters");

            return *this;
        }

        template <typename TFunction>
        constexpr void ForEach(TFunction) const
        {
            static_assert(FunctionTraits<TFunction>::Arity <= 2, "Transform function takes only 0, 1 or 2 parameters");
        }

        constexpr StaticArray Reverse() const
        {
            return *this;
        }

        constexpr SizeType GetSize() const noexcept
        {
            return 0;
        }

        Iterator GetIterator()
        {
            return nullptr;
        }

        ConstIterator GetIterator() const
        {
            return nullptr;
        }

        Iterator GetLastIterator()
        {
            return nullptr;
        }

        ConstIterator GetLastIterator() const
        {
            return nullptr;
        }

        constexpr bool IsEmpty() const noexcept
        {
            return true;
        }

        E* GetData() noexcept
        {
            return nullptr;
        }

        const E* GetData() const noexcept
        {
            return nullptr;
        }

        bool operator==(const StaticArray&) const
        {
            return true;
        }

        bool operator!=(const StaticArray& other) const
        {
            return !(*this == other);
        }

        ElementType& operator[](SizeType)
        {
            return *static_cast<ElementType*>(nullptr);
        }

        const ElementType& operator[](SizeType) const
        {
            return *static_cast<ElementType*>(nullptr);
        }
    };

    template <typename E, unsigned int TSize>
    typename StaticArray<E, TSize>::Iterator begin(StaticArray<E, TSize>& array)
    {
        return array.GetIterator();
    }

    template <typename E, unsigned int TSize>
    typename StaticArray<E, TSize>::ConstIterator begin(const StaticArray<E, TSize>& array)
    {
        return array.GetIterator();
    }

    template <typename E, unsigned int TSize>
    typename StaticArray<E, TSize>::Iterator end(StaticArray<E, TSize>& array)
    {
        return array.GetIterator() + TSize;
    }

    template <typename E, unsigned int TSize>
    typename StaticArray<E, TSize>::ConstIterator end(const StaticArray<E, TSize>& array)
    {
        return array.GetIterator() + TSize;
    }

    /**
     * \brief Creates a StaticArray from a variadic list of values
     *
     * This helper function constructs a StaticArray whose size is automatically
     * deduced from the number of provided arguments.
     *
     * \tparam E Element type of the array
     * \tparam TArgs Types of the provided arguments
     *
     * \param args Values used to initialize the array
     *
     * \return A StaticArray containing the provided values
     *
     * \note The number of arguments determines the size of the resulting array
     * \note All arguments must be convertible to E
     */
    template <typename E, typename... TArgs>
    constexpr StaticArray<E, sizeof...(TArgs)> MakeStaticArray(TArgs&&... args)
    {
        return StaticArray<E, sizeof...(TArgs)>(std::forward<TArgs>(args)...);
    }
}

#endif // COCKTAIL_CORE_STATICARRAY_HPP
