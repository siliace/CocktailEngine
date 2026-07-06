#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Output/Writer/LineWriter.hpp>
#include <Cocktail/Core/IO/Output/Writer/StringWriter.hpp>

using namespace Ck;

TEST_CASE("LineWriter write operations", "[IO][LineWriter]")
{
    using Encoding = Encoders::Ascii;

    StringWriter<Encoding> inner;
    LineWriter<Encoding> writer(inner);

    SECTION("Write text without line break")
    {
        const AnsiChar text[] = "Hello";
        writer.Write(text, 5);

        auto result = inner.ToString();
        REQUIRE(result == BasicString<Encoding>("Hello"));
    }

    SECTION("WriteLine appends text and line break")
    {
        const AnsiChar text[] = "Hello";
        writer.WriteLine(text, 5);

        auto result = inner.ToString();
        // On Linux: "Hello\n"
        REQUIRE(result.GetLength() == 6);
        REQUIRE(result.At(5) == '\n');
    }

    SECTION("NextLine writes line break only")
    {
        writer.NextLine();

        auto result = inner.ToString();
        REQUIRE(result.GetLength() == 1);
        REQUIRE(result.At(0) == '\n');
    }

    SECTION("Write boolean")
    {
        writer.Write(true);
        writer.Write(false);

        auto result = inner.ToString();
        REQUIRE(result == BasicString<Encoding>("truefalse"));
    }

    SECTION("Write BasicStringView")
    {
        BasicStringView<Encoding> sv("Test");
        writer.Write(sv);

        auto result = inner.ToString();
        REQUIRE(result == BasicString<Encoding>("Test"));
    }

    SECTION("WriteLine BasicStringView")
    {
        BasicStringView<Encoding> sv("Line");
        writer.WriteLine(sv);

        auto result = inner.ToString();
        REQUIRE(result == BasicString<Encoding>("Line\n"));
    }
}

TEST_CASE("LineWriter flush delegates", "[IO][LineWriter]")
{
    using Encoding = Encoders::Ascii;

    StringWriter<Encoding> inner;
    LineWriter<Encoding> writer(inner);

    const AnsiChar text[] = "Data";
    writer.Write(text, 4);
    writer.Flush(); // should not crash

    auto result = inner.ToString();
    REQUIRE(result == BasicString<Encoding>("Data"));
}
