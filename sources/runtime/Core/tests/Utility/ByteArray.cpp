#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Utility/ByteArray.hpp>
#include <CocktailEngine/Core/Utility/StringUtils.hpp>

TEST_CASE("Create a ByteArray", "[ByteArray]")
{
    SECTION("with default constructor")
    {
        Ck::ByteArray byteArray;

        REQUIRE(byteArray.GetSize() == 0);
    }

    SECTION("from a memory area")
    {
        const char* data = "Hello World";
        const unsigned int length = Ck::StringUtils<Ck::AnsiChar, unsigned int>::GetLength(data);

        Ck::ByteArray byteArray(data, length);

        REQUIRE(byteArray.GetSize() == length);
    }

    SECTION("by copying an existing one")
    {
        const char* data = "Hello World";
        const unsigned int length = Ck::StringUtils<Ck::AnsiChar, unsigned int>::GetLength(data);

        Ck::ByteArray byteArray1(data, length);
        Ck::ByteArray byteArray2 = byteArray1;

        REQUIRE(byteArray2.GetSize() == length);
    }

    SECTION("by copying an moving one")
    {
        const char* data = "Hello World";
        const unsigned int length = Ck::StringUtils<Ck::AnsiChar, unsigned int>::GetLength(data);

        Ck::ByteArray byteArray1(data, length);
        Ck::ByteArray byteArray2 = Move(byteArray1);

        REQUIRE(byteArray1.GetSize() == 0);
        REQUIRE(byteArray2.GetSize() == length);
    }
}

// The data pointer of an empty ByteArray is nullptr. Every operation below funnels into
// Memory::Copy / Memory::Move, which assert on a non-null source and destination. These
// cases make sure an empty array never reaches those functions with a null pointer, even
// when the number of bytes involved is zero.
namespace
{
    Ck::ByteArray MakeFilledByteArray()
    {
        const char* data = "Hello World";
        const unsigned int length = Ck::StringUtils<Ck::AnsiChar, unsigned int>::GetLength(data);

        return Ck::ByteArray(data, length);
    }

    // ByteArray::operator== currently fails to compile (CTAD on BasicByteArrayView),
    // so compare byte by byte here to keep these tests focused on the empty-array paths.
    bool HaveSameContent(const Ck::ByteArray& lhs, const Ck::ByteArray& rhs)
    {
        if (lhs.GetSize() != rhs.GetSize())
            return false;

        for (unsigned int i = 0; i < lhs.GetSize(); i++)
        {
            if (lhs.At(i) != rhs.At(i))
                return false;
        }

        return true;
    }
}

TEST_CASE("Copy an empty ByteArray", "[ByteArray]")
{
    Ck::ByteArray empty;
    REQUIRE(empty.IsEmpty());

    SECTION("by copy construction")
    {
        Ck::ByteArray copy(empty);

        REQUIRE(copy.IsEmpty());
        REQUIRE(copy.GetSize() == 0);
    }

    SECTION("by copy assignment")
    {
        Ck::ByteArray copy = MakeFilledByteArray();
        copy = empty;

        REQUIRE(copy.IsEmpty());
        REQUIRE(copy.GetSize() == 0);
    }

    SECTION("by move construction")
    {
        Ck::ByteArray moved(Move(empty));

        REQUIRE(moved.IsEmpty());
        REQUIRE(moved.GetSize() == 0);
    }

    SECTION("by move assignment")
    {
        Ck::ByteArray moved = MakeFilledByteArray();
        moved = Move(empty);

        REQUIRE(moved.IsEmpty());
        REQUIRE(moved.GetSize() == 0);
    }
}

TEST_CASE("Append with an empty ByteArray", "[ByteArray]")
{
    SECTION("appending an empty array to a filled one")
    {
        Ck::ByteArray byteArray = MakeFilledByteArray();
        const auto before = byteArray.GetSize();

        Ck::ByteArray empty;
        byteArray.Append(empty);

        REQUIRE(byteArray.GetSize() == before);
    }

    SECTION("appending a filled array to an empty one")
    {
        Ck::ByteArray byteArray;
        Ck::ByteArray other = MakeFilledByteArray();

        byteArray.Append(other);

        REQUIRE(byteArray.GetSize() == other.GetSize());
        REQUIRE(HaveSameContent(byteArray, other));
    }

    SECTION("appending an empty array to an empty one")
    {
        Ck::ByteArray byteArray;
        Ck::ByteArray empty;

        byteArray.Append(empty);

        REQUIRE(byteArray.IsEmpty());
    }
}

TEST_CASE("Prepend with an empty ByteArray", "[ByteArray]")
{
    SECTION("prepending an empty array to a filled one")
    {
        Ck::ByteArray byteArray = MakeFilledByteArray();
        const auto before = byteArray.GetSize();

        Ck::ByteArray empty;
        byteArray.Prepend(empty);

        REQUIRE(byteArray.GetSize() == before);
    }

    SECTION("prepending a filled array to an empty one")
    {
        Ck::ByteArray byteArray;
        Ck::ByteArray other = MakeFilledByteArray();

        byteArray.Prepend(other);

        REQUIRE(byteArray.GetSize() == other.GetSize());
        REQUIRE(HaveSameContent(byteArray, other));
    }

    SECTION("prepending an empty array to an empty one")
    {
        Ck::ByteArray byteArray;
        Ck::ByteArray empty;

        byteArray.Prepend(empty);

        REQUIRE(byteArray.IsEmpty());
    }
}

TEST_CASE("Insert with an empty ByteArray", "[ByteArray]")
{
    SECTION("inserting an empty array into a filled one")
    {
        Ck::ByteArray byteArray = MakeFilledByteArray();
        const auto before = byteArray.GetSize();

        Ck::ByteArray empty;
        byteArray.Insert(byteArray.GetSize() / 2, empty);

        REQUIRE(byteArray.GetSize() == before);
    }

    SECTION("inserting a filled array into an empty one")
    {
        Ck::ByteArray byteArray;
        Ck::ByteArray other = MakeFilledByteArray();

        byteArray.Insert(0, other);

        REQUIRE(byteArray.GetSize() == other.GetSize());
        REQUIRE(HaveSameContent(byteArray, other));
    }

    SECTION("inserting an empty array into an empty one")
    {
        Ck::ByteArray byteArray;
        Ck::ByteArray empty;

        byteArray.Insert(0, empty);

        REQUIRE(byteArray.IsEmpty());
    }
}

TEST_CASE("Slice a ByteArray down to empty", "[ByteArray]")
{
    Ck::ByteArray byteArray = MakeFilledByteArray();

    SECTION("a zero-length slice")
    {
        Ck::ByteArray slice = byteArray.Slice(0, 0);

        REQUIRE(slice.IsEmpty());
        REQUIRE(slice.GetSize() == 0);
    }

    SECTION("a slice from the end")
    {
        Ck::ByteArray slice = byteArray.Slice(byteArray.GetSize());

        REQUIRE(slice.IsEmpty());
        REQUIRE(slice.GetSize() == 0);
    }
}

TEST_CASE("Resize a ByteArray across the empty state", "[ByteArray]")
{
    SECTION("from empty to a non-empty size")
    {
        Ck::ByteArray byteArray;
        byteArray.Resize(8, 0xAB);

        REQUIRE(byteArray.GetSize() == 8);
        REQUIRE_FALSE(byteArray.IsEmpty());
    }

    SECTION("from a non-empty size down to empty")
    {
        Ck::ByteArray byteArray = MakeFilledByteArray();
        byteArray.Resize(0);

        REQUIRE(byteArray.IsEmpty());
        REQUIRE(byteArray.GetSize() == 0);
    }
}

TEST_CASE("View an empty ByteArray", "[ByteArray]")
{
    Ck::ByteArray empty;

    SECTION("building a view from the whole empty array")
    {
        Ck::ByteArrayView view(empty);

        REQUIRE(view.IsEmpty());
        REQUIRE(view.GetSize() == 0);
    }

    SECTION("comparing two empty views")
    {
        Ck::ByteArrayView view(empty);
        Ck::ByteArrayView other{};

        REQUIRE(view == other);
    }
}
