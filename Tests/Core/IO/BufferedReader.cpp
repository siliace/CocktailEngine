#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/StringReader.hpp>

using namespace Ck;

TEST_CASE("BufferedReader basic read", "[IO][BufferedReader]")
{
    using Encoding = Encoders::Ascii;

    StringReader<Encoding> source(BasicString<Encoding>("ABCDEFGHIJ"));
    BufferedReader<Encoding> buffered(source, 4); // small buffer

    SECTION("Read all characters")
    {
        AnsiChar buffer[10] = {};
        auto read = buffered.Read(buffer, 10);

        REQUIRE(read == 10);
        REQUIRE(buffer[0] == 'A');
        REQUIRE(buffer[9] == 'J');
    }

    SECTION("Read in small chunks crosses buffer boundary")
    {
        AnsiChar buffer[3] = {};

        auto r1 = buffered.Read(buffer, 3);
        REQUIRE(r1 == 3);
        REQUIRE(buffer[0] == 'A');
        REQUIRE(buffer[2] == 'C');

        auto r2 = buffered.Read(buffer, 3);
        REQUIRE(r2 == 3);
        REQUIRE(buffer[0] == 'D');
        REQUIRE(buffer[2] == 'F');
    }

    SECTION("Single character reads work")
    {
        AnsiChar c = 0;
        for (int i = 0; i < 10; ++i)
        {
            REQUIRE(buffered.Read(c));
            REQUIRE(c == ('A' + i));
        }
        REQUIRE_FALSE(buffered.Read(c));
    }
}

TEST_CASE("BufferedReader cursor operations", "[IO][BufferedReader]")
{
    using Encoding = Encoders::Ascii;

    StringReader<Encoding> source(BasicString<Encoding>("ABCDEFGH"));
    BufferedReader<Encoding> buffered(source, 4);

    SECTION("Seek invalidates buffer")
    {
        AnsiChar buffer[3] = {};
        buffered.Read(buffer, 3); // fills buffer from source

        buffered.Seek(6);
        AnsiChar c = 0;
        buffered.Read(c);
        REQUIRE(c == 'G');
    }

    SECTION("Rewind goes back to start")
    {
        AnsiChar buffer[5] = {};
        buffered.Read(buffer, 5);

        buffered.Rewind();

        AnsiChar c = 0;
        buffered.Read(c);
        REQUIRE(c == 'A');
    }

    SECTION("IsEof after reading everything")
    {
        AnsiChar buffer[8] = {};
        buffered.Read(buffer, 8);
        REQUIRE(buffered.IsEof());
    }

    SECTION("GetSize delegates to source")
    {
        REQUIRE(buffered.GetSize() == 8);
    }

    SECTION("HasCursor delegates to source")
    {
        REQUIRE(buffered.HasCursor());
    }
}
