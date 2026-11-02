#include <type_traits>

#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Utility/Result.hpp>

namespace
{
    struct MoveOnlyError
    {
        MoveOnlyError() = default;
        MoveOnlyError(const MoveOnlyError&) = delete;
        MoveOnlyError(MoveOnlyError&&) noexcept = default;
        ~MoveOnlyError() = default;
    };
}

TEST_CASE("Create a Result", "[Result]")
{
    SECTION("with a successful value")
    {
        Ck::Result<int, Ck::String> result = Ck::Result<int, Ck::String>::Success(42);

        REQUIRE(result.IsSuccess());
        REQUIRE_FALSE(result.IsFailure());
        REQUIRE(static_cast<bool>(result));
        REQUIRE(result.GetValue() == 42);
    }

    SECTION("with an error")
    {
        Ck::Result<int, Ck::String> result = Ck::Result<int, Ck::String>::Failure(CK_TEXT("Failure"));

        REQUIRE_FALSE(result.IsSuccess());
        REQUIRE(result.IsFailure());
        REQUIRE_FALSE(static_cast<bool>(result));
        REQUIRE(result.GetError() == CK_TEXT("Failure"));
    }

    SECTION("when the value and error have the same type")
    {
        Ck::Result<int, int> success = Ck::Result<int, int>::Success(42);
        Ck::Result<int, int> failure = Ck::Result<int, int>::Failure(24);

        REQUIRE(success.GetValue() == 42);
        REQUIRE(failure.GetError() == 24);
    }
}

TEST_CASE("Access a Result", "[Result]")
{
    SECTION("through non-throwing accessors")
    {
        Ck::Result<int, Ck::String> result = Ck::Result<int, Ck::String>::Success(42);
        const Ck::Result<int, Ck::String>& constResult = result;

        auto value = result.TryGetValue();
        auto error = result.TryGetError();
        auto constValue = constResult.TryGetValue();
        auto constError = constResult.TryGetError();

        REQUIRE(value.HasValue());
        REQUIRE(&value.Get() == &result.GetValue());
        REQUIRE_FALSE(error.HasValue());
        REQUIRE(constValue.HasValue());
        REQUIRE(&constValue.Get() == &constResult.GetValue());
        REQUIRE_FALSE(constError.HasValue());
    }

    SECTION("with the wrong accessor")
    {
        Ck::Result<int, Ck::String> result = Ck::Result<int, Ck::String>::Failure(CK_TEXT("Failure"));
        bool threw = false;

        try
        {
            result.GetValue();
        }
        catch (const Ck::BadResultAccessException& exception)
        {
            threw = true;
            REQUIRE_FALSE(exception.GetMessage().IsEmpty());
        }

        REQUIRE(threw);
        REQUIRE_THROWS_AS(result.GetValue(), Ck::BadResultAccessException);
    }
}

TEST_CASE("Copy and move a Result", "[Result]")
{
    SECTION("by copying")
    {
        Ck::Result<Ck::String, int> source = Ck::Result<Ck::String, int>::Success(CK_TEXT("Success"));
        Ck::Result<Ck::String, int> copy(source);
        Ck::Result<Ck::String, int> assigned = Ck::Result<Ck::String, int>::Failure(42);

        assigned = source;

        REQUIRE(source.GetValue() == CK_TEXT("Success"));
        REQUIRE(copy.GetValue() == CK_TEXT("Success"));
        REQUIRE(assigned.GetValue() == CK_TEXT("Success"));
    }

    SECTION("by moving")
    {
        Ck::Result<Ck::String, int> source = Ck::Result<Ck::String, int>::Success(CK_TEXT("Success"));
        Ck::Result<Ck::String, int> destination(Move(source));
        Ck::Result<Ck::String, int> assigned = Ck::Result<Ck::String, int>::Failure(42);

        assigned = Move(destination);

        REQUIRE(source.IsSuccess());
        REQUIRE(destination.IsSuccess());
        REQUIRE(assigned.IsSuccess());
        REQUIRE(assigned.GetValue() == CK_TEXT("Success"));
    }
}

TEST_CASE("Result special member functions follow its alternatives", "[Result]")
{
    using CopyableResult = Ck::Result<int, Ck::String>;
    using MoveOnlyResult = Ck::Result<int, MoveOnlyError>;

    STATIC_REQUIRE_FALSE(std::is_default_constructible_v<CopyableResult>);
    STATIC_REQUIRE(std::is_copy_constructible_v<CopyableResult>);
    STATIC_REQUIRE(std::is_copy_assignable_v<CopyableResult>);
    STATIC_REQUIRE(std::is_move_constructible_v<CopyableResult>);
    STATIC_REQUIRE(std::is_move_assignable_v<CopyableResult>);

    STATIC_REQUIRE_FALSE(std::is_copy_constructible_v<MoveOnlyResult>);
    STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<MoveOnlyResult>);
    STATIC_REQUIRE(std::is_move_constructible_v<MoveOnlyResult>);
    STATIC_REQUIRE(std::is_move_assignable_v<MoveOnlyResult>);
}

TEST_CASE("Create a void Result", "[Result]")
{
    SECTION("with a successful operation")
    {
        Ck::Result<void, Ck::String> result = Ck::Result<void, Ck::String>::Success();

        REQUIRE(result.IsSuccess());
        REQUIRE_FALSE(result.IsFailure());
        REQUIRE(static_cast<bool>(result));
        REQUIRE_FALSE(result.TryGetError().HasValue());
        REQUIRE_THROWS_AS(result.GetError(), Ck::BadResultAccessException);
    }

    SECTION("with an error")
    {
        Ck::Result<void, Ck::String> result = Ck::Result<void, Ck::String>::Failure(CK_TEXT("Failure"));
        const Ck::Result<void, Ck::String>& constResult = result;
        auto error = result.TryGetError();
        auto constError = constResult.TryGetError();

        REQUIRE_FALSE(result.IsSuccess());
        REQUIRE(result.IsFailure());
        REQUIRE_FALSE(static_cast<bool>(result));
        REQUIRE(error.HasValue());
        REQUIRE(&error.Get() == &result.GetError());
        REQUIRE(constError.HasValue());
        REQUIRE(&constError.Get() == &constResult.GetError());
        REQUIRE(result.GetError() == CK_TEXT("Failure"));
    }
}

TEST_CASE("Copy and move a void Result", "[Result]")
{
    SECTION("when it represents a success")
    {
        Ck::Result<void, Ck::String> source = Ck::Result<void, Ck::String>::Success();
        Ck::Result<void, Ck::String> copy(source);
        Ck::Result<void, Ck::String> destination(Move(source));

        REQUIRE(source.IsSuccess());
        REQUIRE(copy.IsSuccess());
        REQUIRE(destination.IsSuccess());
    }

    SECTION("when it represents an error")
    {
        Ck::Result<void, Ck::String> source = Ck::Result<void, Ck::String>::Failure(CK_TEXT("Failure"));
        Ck::Result<void, Ck::String> copy(source);
        Ck::Result<void, Ck::String> destination = Ck::Result<void, Ck::String>::Success();

        destination = source;

        REQUIRE(copy.GetError() == CK_TEXT("Failure"));
        REQUIRE(destination.GetError() == CK_TEXT("Failure"));

        destination = Move(source);

        REQUIRE(source.IsFailure());
        REQUIRE(destination.IsFailure());
        REQUIRE(destination.GetError() == CK_TEXT("Failure"));
    }
}

TEST_CASE("Void Result special member functions follow its error type", "[Result]")
{
    using CopyableResult = Ck::Result<void, Ck::String>;
    using MoveOnlyResult = Ck::Result<void, MoveOnlyError>;

    STATIC_REQUIRE_FALSE(std::is_default_constructible_v<CopyableResult>);
    STATIC_REQUIRE(std::is_copy_constructible_v<CopyableResult>);
    STATIC_REQUIRE(std::is_copy_assignable_v<CopyableResult>);
    STATIC_REQUIRE(std::is_move_constructible_v<CopyableResult>);
    STATIC_REQUIRE(std::is_move_assignable_v<CopyableResult>);

    STATIC_REQUIRE_FALSE(std::is_copy_constructible_v<MoveOnlyResult>);
    STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<MoveOnlyResult>);
    STATIC_REQUIRE(std::is_move_constructible_v<MoveOnlyResult>);
    STATIC_REQUIRE(std::is_move_assignable_v<MoveOnlyResult>);
}
