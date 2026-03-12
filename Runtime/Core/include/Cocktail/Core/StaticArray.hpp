#ifndef COCKTAIL_CORE_STATICARRAY_HPP
#define COCKTAIL_CORE_STATICARRAY_HPP

#include <Cocktail/Core/Utility/ExceptionUtils.hpp>
#include <Cocktail/Core/Utility/FunctionTraits.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>

namespace Ck
{
    template <typename E, unsigned int TSize>
    class StaticArray
    {
    public:

        using ElementType = E;
        using SizeType = unsigned int;

        using Iterator = ElementType*;
        using ConstIterator = const ElementType*;

        StaticArray() = default;

        template <typename... U>
        StaticArray(U&&... values) :
            mElements{ std::forward<U>(values)... }
        {
            static_assert(sizeof...(U) <= TSize, "Too many initializers for StaticArray");
        }

        void Fill(const ElementType& value)
        {
            for (SizeType i = 0; i < TSize; ++i)
                mElements[i] = value;
        }

        ElementType& At(SizeType index)
        {
            CheckIndex(index);
            return UncheckedAt(index);
        }

        const ElementType& At(SizeType index) const
        {
            CheckIndex(index);
            return UncheckedAt(index);
        }

        Optional<E&> TryAt(SizeType index)
        {
            if (index >= TSize)
                return Optional<E&>::Empty();

            return Optional<E&>::Of(UncheckedAt(index));
        }

        Optional<const E&> TryAt(SizeType index) const
        {
            if (index >= TSize)
                return Optional<const E&>::Empty();

            return Optional<const E&>::Of(UncheckedAt(index));
        }

        E& First()
        {
            if (Optional<E&> first = TryFirst(); !first.IsEmpty())
                return first.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        const E& First() const
        {
            if (Optional<const E&> first = TryFirst(); !first.IsEmpty())
                return first.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        Optional<E&> TryFirst()
        {
            if (IsEmpty())
                return Optional<E&>::Empty();

            return Optional<E&>::Of(At(0));
        }

        Optional<const E&> TryFirst() const
        {
            if (IsEmpty())
                return Optional<const E&>::Empty();

            return Optional<const E&>::Of(At(0));
        }

        E& Last()
        {
            if (Optional<E&> last = TryLast(); !last.IsEmpty())
                return last.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        const E& Last() const
        {
            if (Optional<const E&> last = TryLast(); !last.IsEmpty())
                return last.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        Optional<E&> TryLast()
        {
            if (IsEmpty())
                return Optional<E&>::Empty();

            return Optional<E&>::Of(At(TSize - 1));
        }

        Optional<const E&> TryLast() const
        {
            if (IsEmpty())
                return Optional<const E&>::Empty();

            return Optional<const E&>::Of(At(TSize - 1));
        }

        bool Contains(const E& element) const
        {
            return !FindIndex(element).IsEmpty();
        }

        template <typename TPredicate>
        bool ContainsIf(TPredicate predicate) const
        {
            return !FindIndexIf(predicate).IsEmpty();
        }

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

        Optional<SizeType> FindLastIndex(const E& element) const
        {
            for (SizeType i = TSize; i-- > 0;)
            {
                if (mElements[i] == element)
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

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

        template <typename TFunction>
        StaticArray<typename FunctionTraits<TFunction>::Return, TSize> Transform(TFunction transformer) const
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

        StaticArray Reverse() const
        {
            StaticArray result;
            for (SizeType i = 0; i < TSize; ++i)
                result[i] = mElements[TSize - 1 - i];

            return result;
        }

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

        constexpr bool IsEmpty() const noexcept
        {
            return false;
        }

        constexpr E* GetData() noexcept
        {
            return mElements;
        }

        constexpr const E* GetData() const noexcept
        {
            return mElements;
        }

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

        E& UncheckedAt(SizeType index)
        {
            return mElements[index];
        }

        const E& UncheckedAt(SizeType index) const
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
            return Optional<const E&>::Empty();
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
            return Optional<const E&>::Empty();
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
}

#endif // COCKTAIL_CORE_STATICARRAY_HPP
