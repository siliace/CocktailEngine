#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Output/Writer/StringWriter.hpp>

using namespace Ck;

TEST_CASE("StringWriter basic write", "[IO][Writer]")
{
    using Encoding = Encoders::Ascii;

    StringWriter<Encoding> writer;

    SECTION("Write appends text")
    {
        const AnsiChar text[] = "Hello";
        writer.Write(text, 5);

        auto result = writer.ToString();
        REQUIRE(result.GetLength() == 5);
        REQUIRE(result == BasicString<Encoding>("Hello"));
    }

    SECTION("Multiple writes concatenate")
    {
        const AnsiChar hello[] = "Hello";
        const AnsiChar space[] = " ";
        const AnsiChar world[] = "World";

        writer.Write(hello, 5);
        writer.Write(space, 1);
        writer.Write(world, 5);

        auto result = writer.ToString();
        REQUIRE(result == BasicString<Encoding>("Hello World"));
    }

    SECTION("Flush is no-op")
    {
        const AnsiChar text[] = "Test";
        writer.Write(text, 4);
        writer.Flush();

        auto result = writer.ToString();
        REQUIRE(result.GetLength() == 4);
    }
}
