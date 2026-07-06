#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Input/Reader/LineReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/StringReader.hpp>

using namespace Ck;

TEST_CASE("LineReader reads lines from source", "[IO][LineReader]")
{
    using Encoding = Encoders::Ascii;

    SECTION("Read lines separated by newline")
    {
        StringReader<Encoding> source(BasicString<Encoding>("Line1\nLine2\nLine3\n"));
        LineReader<Encoding> lineReader(source);

        BasicString<Encoding> line;

        REQUIRE(lineReader.ReadLine(line, false));
        REQUIRE(line == BasicString<Encoding>("Line1"));

        REQUIRE(lineReader.ReadLine(line, false));
        REQUIRE(line == BasicString<Encoding>("Line2"));

        REQUIRE(lineReader.ReadLine(line, false));
        REQUIRE(line == BasicString<Encoding>("Line3"));

        REQUIRE_FALSE(lineReader.ReadLine(line, false));
    }

    SECTION("Last line without newline is still returned")
    {
        StringReader<Encoding> source(BasicString<Encoding>("Line1\nLine2"));
        LineReader<Encoding> lineReader(source);

        BasicString<Encoding> line;

        REQUIRE(lineReader.ReadLine(line, false));
        REQUIRE(line == BasicString<Encoding>("Line1"));

        REQUIRE(lineReader.ReadLine(line, false));
        REQUIRE(line == BasicString<Encoding>("Line2"));

        REQUIRE_FALSE(lineReader.ReadLine(line, false));
    }

    SECTION("Empty lines are returned when ignoreEmptyLines is false")
    {
        StringReader<Encoding> source(BasicString<Encoding>("A\n\nB\n"));
        LineReader<Encoding> lineReader(source);

        BasicString<Encoding> line;

        REQUIRE(lineReader.ReadLine(line, false));
        REQUIRE(line == BasicString<Encoding>("A"));

        REQUIRE(lineReader.ReadLine(line, false));
        REQUIRE(line.IsEmpty());

        REQUIRE(lineReader.ReadLine(line, false));
        REQUIRE(line == BasicString<Encoding>("B"));
    }

    SECTION("Empty lines are skipped when ignoreEmptyLines is true")
    {
        StringReader<Encoding> source(BasicString<Encoding>("A\n\n\nB\n"));
        LineReader<Encoding> lineReader(source);

        BasicString<Encoding> line;

        REQUIRE(lineReader.ReadLine(line, true));
        REQUIRE(line == BasicString<Encoding>("A"));

        REQUIRE(lineReader.ReadLine(line, true));
        REQUIRE(line == BasicString<Encoding>("B"));
    }
}

TEST_CASE("LineReader ReadLine returning Optional", "[IO][LineReader]")
{
    using Encoding = Encoders::Ascii;

    StringReader<Encoding> source(BasicString<Encoding>("Hello\nWorld\n"));
    LineReader<Encoding> lineReader(source);

    auto line1 = lineReader.ReadLine(false);
    REQUIRE_FALSE(line1.IsEmpty());
    REQUIRE(line1.Get() == BasicString<Encoding>("Hello"));

    auto line2 = lineReader.ReadLine(false);
    REQUIRE_FALSE(line2.IsEmpty());
    REQUIRE(line2.Get() == BasicString<Encoding>("World"));

    auto line3 = lineReader.ReadLine(false);
    REQUIRE(line3.IsEmpty());
}
