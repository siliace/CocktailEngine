#ifndef COCKTAILENGINE_CORE_UTILITY_VARIANT_HPP
#define COCKTAILENGINE_CORE_UTILITY_VARIANT_HPP

#include <cstddef>
#include <functional>
#include <type_traits>

#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/Memory/ObjectMemoryUtils.hpp>
#include <CocktailEngine/Core/Utility/StorageFor.hpp>

namespace Ck
{
    /**
     * \brief Exception thrown when accessing an inactive Variant alternative.
     *
     * The exception message identifies the requested alternative index and, when
     * applicable, the index of the active alternative.
     */
    COCKTAIL_DECLARE_EXCEPTION_FROM(BadVariantCastException, RuntimeException);

    namespace Detail
    {
        template <typename... TTypes>
        struct MaxSize;

        template <typename T, typename... TOthers>
        struct MaxSize<T, TOthers...>
        {
            static constexpr std::size_t Value = sizeof(T) > MaxSize<TOthers...>::Value ? sizeof(T) : MaxSize<TOthers...>::Value;
        };

        template <>
        struct MaxSize<>
        {
            static constexpr std::size_t Value = 1;
        };

        template <typename... TTypes>
        struct MaxAlignment;

        template <typename T, typename... TOthers>
        struct MaxAlignment<T, TOthers...>
        {
            static constexpr std::size_t Value = alignof(T) > MaxAlignment<TOthers...>::Value ? alignof(T) : MaxAlignment<TOthers...>::Value;
        };

        template <>
        struct MaxAlignment<>
        {
            static constexpr std::size_t Value = 1;
        };

        template <typename T, typename... TOthers>
        struct TypeIndex;

        template <typename T, typename... TOthers>
        struct TypeIndex<T, T, TOthers...>
        {
            static constexpr std::size_t Value = 0;
        };

        template <typename T, typename U, typename... TOthers>
        struct TypeIndex<T, U, TOthers...>
        {
            static constexpr std::size_t Value = 1 + TypeIndex<T, TOthers...>::Value;
        };

        template <std::size_t TIndex, typename... TTypes>
        struct TypeAt;

        template <typename T, typename... TOthers>
        struct TypeAt<0, T, TOthers...>
        {
            using Type = T;
        };

        template <std::size_t TIndex, typename T, typename... TOthers>
        struct TypeAt<TIndex, T, TOthers...>
        {
            using Type = typename TypeAt<TIndex - 1, TOthers...>::Type;
        };

        template <typename T, typename... TTypes>
        struct Contains;

        template <typename T>
        struct Contains<T>
        {
            static constexpr bool Value = false;
        };

        template <typename T, typename U, typename... TOthers>
        struct Contains<T, U, TOthers...>
        {
            static constexpr bool Value = std::is_same_v<T, U> || Contains<T, TOthers...>::Value;
        };

        template <typename... TTypes>
        struct AreUnique;

        template <>
        struct AreUnique<>
        {
            static constexpr bool Value = true;
        };

        template <typename T, typename... TOthers>
        struct AreUnique<T, TOthers...>
        {
            static constexpr bool Value = !Contains<T, TOthers...>::Value && AreUnique<TOthers...>::Value;
        };

        using CopyConstructFunction = void (*)(void*, const void*);
        using MoveConstructFunction = void (*)(void*, void*);

        template <typename T>
        void CopyConstruct(void* destination, const void* source)
        {
            ObjectMemoryUtils::Construct(static_cast<T*>(destination), *static_cast<const T*>(source));
        }

        template <typename T>
        void MoveConstruct(void* destination, void* source)
        {
            ObjectMemoryUtils::Construct(static_cast<T*>(destination), Move(*static_cast<T*>(source)));
        }

        template <bool TEnabled, typename... TTypes>
        struct CopyConstructFunctions
        {
            inline static constexpr CopyConstructFunction Values[] = { nullptr };
        };

        template <typename... TTypes>
        struct CopyConstructFunctions<true, TTypes...>
        {
            inline static constexpr CopyConstructFunction Values[] = { &CopyConstruct<TTypes>... };
        };

        template <bool TEnabled, typename... TTypes>
        struct MoveConstructFunctions
        {
            inline static constexpr MoveConstructFunction Values[] = { nullptr };
        };

        template <typename... TTypes>
        struct MoveConstructFunctions<true, TTypes...>
        {
            inline static constexpr MoveConstructFunction Values[] = { &MoveConstruct<TTypes>... };
        };
    }

    /**
     * \brief Stores at most one value from a fixed, non-empty set of types.
     *
     * A Variant is empty after default construction or Reset. Construct a value
     * with an InPlaceTypeTag or replace it with Emplace. The active alternative
     * can be accessed with Get, TryGet, Is or Visit.
     *
     * Variant stores its value in local, suitably aligned memory and does not
     * allocate dynamically. References and pointers returned by Get, TryGet and
     * Emplace remain valid until the Variant is reset, another value is emplaced,
     * the Variant is assigned to, or the Variant is destroyed.
     *
     * Every alternative must be an object type with a non-throwing destructor and
     * must occur exactly once. Variant copy operations are available only when all
     * alternatives are copy-constructible. Move and Swap operations are available
     * only when all alternatives are move-constructible.
     *
     * \tparam TTypes Types that may be stored in the Variant.
     */
    template <typename... TTypes>
    class Variant
    {
        static_assert(sizeof...(TTypes) > 0, "Variant requires at least one alternative type");
        static_assert((std::is_object_v<TTypes> && ...), "Variant alternatives must be object types");
        static_assert((std::is_nothrow_destructible_v<TTypes> && ...), "Variant alternatives must have non-throwing destructors");
        static_assert(Detail::AreUnique<TTypes...>::Value, "Variant alternative types must be unique");

        template <typename U>
        using TypeIndexImpl = Detail::TypeIndex<U, TTypes...>;

        template <std::size_t TIndex>
        using TypeAtImpl = typename Detail::TypeAt<TIndex, TTypes...>::Type;

        static constexpr bool CanCopy = (std::is_copy_constructible_v<TTypes> && ...);
        static constexpr bool CanMove = (std::is_move_constructible_v<TTypes> && ...);
        static constexpr bool HasNoThrowMove = (std::is_nothrow_move_constructible_v<TTypes> && ...);

    public:

        /**
         * \brief Index that represents the empty Variant state.
         */
        static constexpr std::size_t InvalidIndex = static_cast<std::size_t>(-1);

        /**
         * \brief Returns the index assigned to an alternative type.
         *
         * \tparam T Alternative type whose index is requested.
         *
         * \return Zero-based index of \p T in \p TTypes.
         */
        template <typename T, std::enable_if_t<Detail::Contains<T, TTypes...>::Value, int> = 0>
        static constexpr std::size_t IndexFor()
        {
            return TypeIndexImpl<T>::Value;
        }

        /**
         * \brief Constructs an empty Variant.
         */
        Variant() :
            mIndex(InvalidIndex)
        {
            /// Nothing
        }

        /**
         * \brief Copy-constructs a Variant.
         *
         * The new Variant is empty when \p other is empty. Otherwise, it contains
         * a copy of the active value of \p other.
         *
         * \param other Variant to copy.
         */
        Variant(const Variant& other) requires(CanCopy) :
            mIndex(InvalidIndex)
        {
            CopyFrom(other);
        }

        Variant(const Variant&) requires(!CanCopy) = delete;

        /**
         * \brief Move-constructs a Variant.
         *
         * The new Variant is empty when \p other is empty. Otherwise, it receives
         * the active value of \p other and \p other becomes empty after a successful
         * move.
         *
         * \param other Variant to move from.
         */
        Variant(Variant&& other) noexcept(HasNoThrowMove) requires(CanMove) :
            mIndex(InvalidIndex)
        {
            MoveFrom(other);
        }

        Variant(Variant&&) requires(!CanMove) = delete;

        /**
         * \brief Destroys the active value when the Variant is not empty.
         */
        ~Variant()
        {
            Reset();
        }

        /**
         * \brief Replaces this Variant with a copy of another Variant.
         *
         * Self-assignment has no effect. If copying the active value throws, this
         * Variant is left empty.
         *
         * \param other Variant to copy.
         *
         * \return This Variant.
         */
        Variant& operator=(const Variant& other) requires(CanCopy)
        {
            if (this != &other)
            {
                Reset();
                CopyFrom(other);
            }

            return *this;
        }

        Variant& operator=(const Variant&) requires(!CanCopy) = delete;

        /**
         * \brief Replaces this Variant by moving another Variant into it.
         *
         * Self-move assignment has no effect. The source Variant becomes empty after
         * a successful move. If moving the active value throws, this Variant is empty
         * and the source remains valid but may contain a moved-from value.
         *
         * \param other Variant to move from.
         *
         * \return This Variant.
         */
        Variant& operator=(Variant&& other) noexcept(HasNoThrowMove) requires(CanMove)
        {
            if (this != &other)
            {
                Reset();
                MoveFrom(other);
            }

            return *this;
        }

        Variant& operator=(Variant&&) requires(!CanMove) = delete;

        /**
         * \brief Constructs an alternative in place.
         *
         * \tparam T Alternative type to construct.
         * \tparam TArgs Constructor argument types.
         *
         * \param args Arguments forwarded to the constructor of \p T.
         */
        template <typename T, typename... TArgs, std::enable_if_t<Detail::Contains<T, TTypes...>::Value, int> = 0>
        explicit Variant(InPlaceTypeTag<T>, TArgs&&... args) :
            mIndex(InvalidIndex)
        {
            Emplace<T>(Forward<TArgs>(args)...);
        }

        /**
         * \brief Replaces the active value with an alternative constructed in place.
         *
         * When \p T can be moved or copied without throwing, the replacement is first
         * constructed in temporary storage. In that case, an exception thrown by its
         * constructor leaves this Variant unchanged. For other types, the active value
         * is destroyed before construction and a constructor exception leaves this
         * Variant empty.
         *
         * \tparam T Alternative type to construct.
         * \tparam TArgs Constructor argument types.
         *
         * \param args Arguments forwarded to the constructor of \p T.
         *
         * \return Reference to the newly constructed value.
         */
        template <typename T, typename... TArgs, std::enable_if_t<Detail::Contains<T, TTypes...>::Value, int> = 0>
        T& Emplace(TArgs&&... args)
        {
            if constexpr (std::is_nothrow_move_constructible_v<T> || std::is_nothrow_copy_constructible_v<T>)
            {
                StorageForSize<MaxAlignment, MaxSize> temporaryStorage;
                T* temporary = static_cast<T*>(temporaryStorage.GetAddress());
                ObjectMemoryUtils::Construct(temporary, Forward<TArgs>(args)...);

                Reset();

                T* value = static_cast<T*>(mStorage.GetAddress());
                if constexpr (std::is_nothrow_move_constructible_v<T>)
                {
                    ObjectMemoryUtils::Construct(value, Move(*temporary));
                }
                else
                {
                    ObjectMemoryUtils::Construct(value, *temporary);
                }

                ObjectMemoryUtils::Destroy(temporary);
                mIndex = TypeIndexImpl<T>::Value;

                return *value;
            }
            else
            {
                Reset();

                T* value = static_cast<T*>(mStorage.GetAddress());
                ObjectMemoryUtils::Construct(value, Forward<TArgs>(args)...);
                mIndex = TypeIndexImpl<T>::Value;

                return *value;
            }
        }

        /**
         * \brief Retrieves the active value as a mutable reference.
         *
         * \tparam T Expected active alternative type.
         *
         * \return Reference to the active value.
         *
         * \throws BadVariantCastException If the Variant is empty or its active
         *         alternative is not \p T.
         */
        template <typename T, std::enable_if_t<Detail::Contains<T, TTypes...>::Value, int> = 0>
        T& Get()
        {
            if (T* value = TryGet<T>())
                return *value;

            ThrowBadVariantCast(IndexFor<T>());
        }

        /**
         * \brief Retrieves the active value as a const reference.
         *
         * \tparam T Expected active alternative type.
         *
         * \return Const reference to the active value.
         *
         * \throws BadVariantCastException If the Variant is empty or its active
         *         alternative is not \p T.
         */
        template <typename T, std::enable_if_t<Detail::Contains<T, TTypes...>::Value, int> = 0>
        const T& Get() const
        {
            if (const T* value = TryGet<T>())
                return *value;

            ThrowBadVariantCast(IndexFor<T>());
        }

        /**
         * \brief Attempts to retrieve the active value as a mutable pointer.
         *
         * \tparam T Expected active alternative type.
         *
         * \return Pointer to the active value when it is \p T, otherwise \c nullptr.
         */
        template <typename T, std::enable_if_t<Detail::Contains<T, TTypes...>::Value, int> = 0>
        T* TryGet()
        {
            return Is<T>() ? static_cast<T*>(mStorage.GetAddress()) : nullptr;
        }

        /**
         * \brief Attempts to retrieve the active value as a const pointer.
         *
         * \tparam T Expected active alternative type.
         *
         * \return Const pointer to the active value when it is \p T, otherwise
         *         \c nullptr.
         */
        template <typename T, std::enable_if_t<Detail::Contains<T, TTypes...>::Value, int> = 0>
        const T* TryGet() const
        {
            return Is<T>() ? static_cast<const T*>(mStorage.GetAddress()) : nullptr;
        }

        /**
         * \brief Checks whether a specified type is active.
         *
         * \tparam T Alternative type to test.
         *
         * \return \c true when \p T is active, \c false otherwise.
         */
        template <typename T, std::enable_if_t<Detail::Contains<T, TTypes...>::Value, int> = 0>
        bool Is() const
        {
            return mIndex == IndexFor<T>();
        }

        /**
         * \brief Invokes a visitor with the mutable active value.
         *
         * The visitor must accept every alternative as a mutable reference and return
         * the same type for every alternative.
         *
         * \tparam TVisitor Visitor type.
         *
         * \param visitor Visitor to invoke.
         *
         * \return The result returned by \p visitor.
         *
         * \throws BadVariantCastException If the Variant is empty.
         */
        template <typename TVisitor>
        decltype(auto) Visit(TVisitor&& visitor)
        {
            if (IsEmpty())
                ThrowBadVariantVisit();

            return VisitImpl<0>(Forward<TVisitor>(visitor));
        }

        /**
         * \brief Invokes a visitor with the const active value.
         *
         * The visitor must accept every alternative as a const reference and return
         * the same type for every alternative.
         *
         * \tparam TVisitor Visitor type.
         *
         * \param visitor Visitor to invoke.
         *
         * \return The result returned by \p visitor.
         *
         * \throws BadVariantCastException If the Variant is empty.
         */
        template <typename TVisitor>
        decltype(auto) Visit(TVisitor&& visitor) const
        {
            if (IsEmpty())
                ThrowBadVariantVisit();

            return VisitImpl<0>(Forward<TVisitor>(visitor));
        }

        /**
         * \brief Destroys the active value and makes the Variant empty.
         *
         * Calling Reset on an empty Variant has no effect.
         */
        void Reset() noexcept
        {
            if (mIndex != InvalidIndex)
            {
                DestroyFunctions[mIndex](mStorage.GetAddress());
                mIndex = InvalidIndex;
            }
        }

        /**
         * \brief Exchanges the contents of two Variants.
         *
         * \param other Variant whose contents are exchanged with this Variant.
         */
        void Swap(Variant& other) noexcept(HasNoThrowMove) requires(CanMove)
        {
            if (this == &other)
                return;

            Variant temporary(Move(*this));
            *this = Move(other);
            other = Move(temporary);
        }

        void Swap(Variant&) requires(!CanMove) = delete;

        /**
         * \brief Checks whether this Variant is empty.
         *
         * \return \c true when no alternative is active, \c false otherwise.
         */
        bool IsEmpty() const
        {
            return mIndex == InvalidIndex;
        }

        /**
         * \brief Checks whether this Variant contains an active alternative.
         *
         * \return \c true when an alternative is active, \c false otherwise.
         */
        bool HasValue() const
        {
            return !IsEmpty();
        }

        /**
         * \brief Tests whether this Variant contains an active alternative.
         *
         * This enables direct use in boolean contexts such as \c if (variant).
         *
         * \return \c true when an alternative is active, \c false otherwise.
         */
        explicit operator bool() const
        {
            return HasValue();
        }

        /**
         * \brief Returns the index of the active alternative.
         *
         * \return The active alternative index, or InvalidIndex when empty.
         */
        std::size_t GetIndex() const
        {
            return mIndex;
        }

    private:

        using DestroyFunction = void (*)(void*);

        template <typename T>
        static void Destroy(void* storage) noexcept
        {
            ObjectMemoryUtils::Destroy(static_cast<T*>(storage));
        }

        template <typename T>
        static void CopyConstruct(void* destination, const void* source)
        {
            ObjectMemoryUtils::Construct(static_cast<T*>(destination), *static_cast<const T*>(source));
        }

        template <typename T>
        static void MoveConstruct(void* destination, void* source)
        {
            ObjectMemoryUtils::Construct(static_cast<T*>(destination), Move(*static_cast<T*>(source)));
        }

        void CopyFrom(const Variant& other) requires(CanCopy)
        {
            if (!other.HasValue())
                return;

            Detail::CopyConstructFunctions<CanCopy, TTypes...>::Values[other.mIndex](mStorage.GetAddress(), other.mStorage.GetAddress());
            mIndex = other.mIndex;
        }

        void MoveFrom(Variant& other) requires(CanMove)
        {
            if (!other.HasValue())
                return;

            Detail::MoveConstructFunctions<CanMove, TTypes...>::Values[other.mIndex](mStorage.GetAddress(), other.mStorage.GetAddress());
            mIndex = other.mIndex;
            other.Reset();
        }

        [[noreturn]] void ThrowBadVariantCast(std::size_t requestedIndex) const
        {
            if (IsEmpty())
                throw BadVariantCastException(CK_TEXT("Cannot retrieve Variant alternative at index %u from an empty Variant."), static_cast<unsigned int>(requestedIndex));

            throw BadVariantCastException(CK_TEXT("Cannot retrieve Variant alternative at index %u while alternative at index %u is active."),
                                           static_cast<unsigned int>(requestedIndex), static_cast<unsigned int>(mIndex));
        }

        [[noreturn]] void ThrowBadVariantVisit() const
        {
            throw BadVariantCastException(CK_TEXT("Cannot visit an empty Variant."));
        }

        template <std::size_t TIndex, typename TVisitor>
        decltype(auto) VisitImpl(TVisitor&& visitor)
        {
            if (mIndex == TIndex)
            {
                using T = TypeAtImpl<TIndex>;
                return std::invoke(Forward<TVisitor>(visitor), *static_cast<T*>(mStorage.GetAddress()));
            }

            if constexpr (TIndex + 1 < sizeof...(TTypes))
            {
                return VisitImpl<TIndex + 1>(Forward<TVisitor>(visitor));
            }
            else
            {
                ThrowBadVariantVisit();
            }
        }

        template <std::size_t TIndex, typename TVisitor>
        decltype(auto) VisitImpl(TVisitor&& visitor) const
        {
            if (mIndex == TIndex)
            {
                using T = TypeAtImpl<TIndex>;
                return std::invoke(Forward<TVisitor>(visitor), *static_cast<const T*>(mStorage.GetAddress()));
            }

            if constexpr (TIndex + 1 < sizeof...(TTypes))
            {
                return VisitImpl<TIndex + 1>(Forward<TVisitor>(visitor));
            }
            else
            {
                ThrowBadVariantVisit();
            }
        }

        static constexpr DestroyFunction DestroyFunctions[] = { &Destroy<TTypes>... };

        static constexpr std::size_t MaxAlignment = Detail::MaxAlignment<TTypes...>::Value;
        static constexpr std::size_t MaxSize = Detail::MaxSize<TTypes...>::Value;

        std::size_t mIndex;
        StorageForSize<MaxAlignment, MaxSize> mStorage{};
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_VARIANT_HPP
