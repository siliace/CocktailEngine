#include <stdexcept>
#include <type_traits>

#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Utility/Variant.hpp>

namespace
{
    struct LifetimeValue
    {
        inline static int LiveCount = 0;

        explicit LifetimeValue(int value) :
            Value(value)
        {
            ++LiveCount;
        }

        LifetimeValue(const LifetimeValue& other) noexcept :
            Value(other.Value)
        {
            ++LiveCount;
        }

        LifetimeValue(LifetimeValue&& other) noexcept :
            Value(other.Value)
        {
            ++LiveCount;
        }

        ~LifetimeValue()
        {
            --LiveCount;
        }

        static void ResetCounts()
        {
            LiveCount = 0;
        }

        int Value;
    };

    struct CopyMoveValue
    {
        inline static int CopyConstructionCount = 0;
        inline static int MoveConstructionCount = 0;

        explicit CopyMoveValue(int value) :
            Value(value)
        {
        }

        CopyMoveValue(const CopyMoveValue& other) noexcept :
            Value(other.Value)
        {
            ++CopyConstructionCount;
        }

        CopyMoveValue(CopyMoveValue&& other) noexcept :
            Value(other.Value)
        {
            ++MoveConstructionCount;
            other.Value = -1;
        }

        ~CopyMoveValue() = default;

        static void ResetCounts()
        {
            CopyConstructionCount = 0;
            MoveConstructionCount = 0;
        }

        int Value;
    };

    struct MoveOnlyValue
    {
        MoveOnlyValue() = default;
        MoveOnlyValue(const MoveOnlyValue&) = delete;
        MoveOnlyValue(MoveOnlyValue&&) noexcept = default;
        ~MoveOnlyValue() = default;
    };

    struct ThrowingValue
    {
        ThrowingValue()
        {
            throw std::runtime_error("Construction failed");
        }
    };

    struct ThrowingTransferValue
    {
        explicit ThrowingTransferValue(bool throwOnConstruction)
        {
            if (throwOnConstruction)
                throw std::runtime_error("Construction failed");
        }

        ThrowingTransferValue(const ThrowingTransferValue&) = delete;
        ThrowingTransferValue(ThrowingTransferValue&&) noexcept(false)
        {
        }
    };
}

TEST_CASE("Create a Variant", "[Variant]")
{
    SECTION("with the default constructor")
    {
        Ck::Variant<int, float> variant;

        REQUIRE(variant.IsEmpty());
        REQUIRE_FALSE(variant.HasValue());
        REQUIRE_FALSE(static_cast<bool>(variant));
        REQUIRE(variant.GetIndex() == Ck::Variant<int, float>::InvalidIndex);
        REQUIRE_THROWS_AS(variant.Get<int>(), Ck::BadVariantCastException);
    }

    SECTION("by constructing an alternative in place")
    {
        Ck::Variant<int, float> variant(Ck::InPlaceType<float>, 4.2f);

        REQUIRE(variant.HasValue());
        REQUIRE(static_cast<bool>(variant));
        REQUIRE(variant.GetIndex() == Ck::Variant<int, float>::IndexFor<float>());
        REQUIRE(variant.Get<float>() == 4.2f);
    }
}

TEST_CASE("Inspect an active Variant alternative", "[Variant]")
{
    Ck::Variant<int, float> variant(Ck::InPlaceType<int>, 42);

    SECTION("as its active type")
    {
        variant.Get<int>() = 24;

        REQUIRE(variant.Is<int>());
        REQUIRE_FALSE(variant.Is<float>());
        REQUIRE(variant.Get<int>() == 24);
        REQUIRE(variant.TryGet<int>() == &variant.Get<int>());
        REQUIRE(variant.TryGet<float>() == nullptr);
    }

    SECTION("from a const Variant")
    {
        const Ck::Variant<int, float>& constVariant = variant;

        REQUIRE(constVariant.Get<int>() == 42);
        REQUIRE(constVariant.TryGet<int>() == &constVariant.Get<int>());
        REQUIRE(constVariant.TryGet<float>() == nullptr);
    }

    SECTION("as an inactive type")
    {
        bool threw = false;

        try
        {
            variant.Get<float>();
        }
        catch (const Ck::BadVariantCastException& exception)
        {
            threw = true;
            REQUIRE_FALSE(exception.GetMessage().IsEmpty());
        }

        REQUIRE(threw);
    }
}

TEST_CASE("Copy and move a Variant", "[Variant]")
{
    CopyMoveValue::ResetCounts();
    Ck::Variant<CopyMoveValue, int> source(Ck::InPlaceType<CopyMoveValue>, 42);

    SECTION("with the copy constructor")
    {
        Ck::Variant<CopyMoveValue, int> copy(source);

        REQUIRE(CopyMoveValue::CopyConstructionCount == 1);
        REQUIRE(source.Get<CopyMoveValue>().Value == 42);
        REQUIRE(copy.Get<CopyMoveValue>().Value == 42);
    }

    SECTION("with copy assignment")
    {
        Ck::Variant<CopyMoveValue, int> destination(Ck::InPlaceType<int>, 24);

        destination = source;

        REQUIRE(CopyMoveValue::CopyConstructionCount == 1);
        REQUIRE(destination.Is<CopyMoveValue>());
        REQUIRE(destination.Get<CopyMoveValue>().Value == 42);
    }

    SECTION("with the move constructor")
    {
        Ck::Variant<CopyMoveValue, int> destination(Move(source));

        REQUIRE(CopyMoveValue::MoveConstructionCount == 2);
        REQUIRE(source.IsEmpty());
        REQUIRE(destination.Get<CopyMoveValue>().Value == 42);
    }

    SECTION("with move assignment")
    {
        Ck::Variant<CopyMoveValue, int> destination(Ck::InPlaceType<int>, 24);

        destination = Move(source);

        REQUIRE(CopyMoveValue::MoveConstructionCount == 2);
        REQUIRE(source.IsEmpty());
        REQUIRE(destination.Get<CopyMoveValue>().Value == 42);
    }
}

TEST_CASE("Replace and reset a Variant value", "[Variant]")
{
    SECTION("by emplacing another alternative")
    {
        Ck::Variant<int, float> variant(Ck::InPlaceType<int>, 42);
        float& value = variant.Emplace<float>(4.2f);

        REQUIRE(&value == &variant.Get<float>());
        REQUIRE(variant.GetIndex() == Ck::Variant<int, float>::IndexFor<float>());
        REQUIRE(variant.Get<float>() == 4.2f);
        REQUIRE_THROWS_AS(variant.Get<int>(), Ck::BadVariantCastException);
    }

    SECTION("by resetting its active value")
    {
        Ck::Variant<int, float> variant(Ck::InPlaceType<int>, 42);

        variant.Reset();

        REQUIRE(variant.IsEmpty());
        REQUIRE(variant.GetIndex() == Ck::Variant<int, float>::InvalidIndex);
        REQUIRE_THROWS_AS(variant.Get<int>(), Ck::BadVariantCastException);
    }
}

TEST_CASE("Variant manages the lifetime of its active value", "[Variant]")
{
    LifetimeValue::ResetCounts();

    {
        Ck::Variant<LifetimeValue, int> variant(Ck::InPlaceType<LifetimeValue>, 42);

        REQUIRE(LifetimeValue::LiveCount == 1);

        variant.Emplace<int>(24);

        REQUIRE(LifetimeValue::LiveCount == 0);
    }

    REQUIRE(LifetimeValue::LiveCount == 0);
}

TEST_CASE("Variant preserves its value when safe replacement construction fails", "[Variant]")
{
    LifetimeValue::ResetCounts();
    Ck::Variant<LifetimeValue, ThrowingValue, int> variant(Ck::InPlaceType<LifetimeValue>, 42);

    REQUIRE_THROWS_AS(variant.Emplace<ThrowingValue>(), std::runtime_error);
    REQUIRE(LifetimeValue::LiveCount == 1);
    REQUIRE(variant.Is<LifetimeValue>());
    REQUIRE(variant.Get<LifetimeValue>().Value == 42);

    variant.Reset();

    REQUIRE(LifetimeValue::LiveCount == 0);
}

TEST_CASE("Variant is empty when unsafe replacement construction fails", "[Variant]")
{
    LifetimeValue::ResetCounts();
    Ck::Variant<LifetimeValue, ThrowingTransferValue, int> variant(Ck::InPlaceType<LifetimeValue>, 42);

    REQUIRE_THROWS_AS(variant.Emplace<ThrowingTransferValue>(true), std::runtime_error);
    REQUIRE(LifetimeValue::LiveCount == 0);
    REQUIRE(variant.IsEmpty());
}

TEST_CASE("Swap two Variants", "[Variant]")
{
    Ck::Variant<CopyMoveValue, int> lhs(Ck::InPlaceType<CopyMoveValue>, 42);
    Ck::Variant<CopyMoveValue, int> rhs(Ck::InPlaceType<int>, 24);

    lhs.Swap(rhs);

    REQUIRE(lhs.Get<int>() == 24);
    REQUIRE(rhs.Get<CopyMoveValue>().Value == 42);
}

TEST_CASE("Visit the active Variant alternative", "[Variant]")
{
    SECTION("with a mutable visitor")
    {
        Ck::Variant<int, float> variant(Ck::InPlaceType<int>, 42);

        variant.Visit([](auto& value) {
            value += 1;
        });

        REQUIRE(variant.Get<int>() == 43);
    }

    SECTION("with a const visitor")
    {
        const Ck::Variant<int, float> variant(Ck::InPlaceType<float>, 4.2f);
        int value = variant.Visit([](const auto& activeValue) -> int {
            return static_cast<int>(activeValue);
        });

        REQUIRE(value == 4);
    }

    SECTION("on an empty Variant")
    {
        Ck::Variant<int, float> variant;

        REQUIRE_THROWS_AS(variant.Visit([](const auto&) {}), Ck::BadVariantCastException);
    }
}

TEST_CASE("Variant special member functions follow its alternatives", "[Variant]")
{
    using CopyableVariant = Ck::Variant<int, CopyMoveValue>;
    using MoveOnlyVariant = Ck::Variant<int, MoveOnlyValue>;

    STATIC_REQUIRE(std::is_copy_constructible_v<CopyableVariant>);
    STATIC_REQUIRE(std::is_copy_assignable_v<CopyableVariant>);
    STATIC_REQUIRE(std::is_move_constructible_v<CopyableVariant>);
    STATIC_REQUIRE(std::is_move_assignable_v<CopyableVariant>);

    STATIC_REQUIRE_FALSE(std::is_copy_constructible_v<MoveOnlyVariant>);
    STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<MoveOnlyVariant>);
    STATIC_REQUIRE(std::is_move_constructible_v<MoveOnlyVariant>);
    STATIC_REQUIRE(std::is_move_assignable_v<MoveOnlyVariant>);
}
