#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Input/Reader/StringReader.hpp>
#include <Cocktail/Core/IO/Output/Writer/StringWriter.hpp>

using namespace Ck;

TEST_CASE("StringReader basic read", "[IO][Reader]")
{
    using Encoding = Encoders::Ascii;

    StringReader<Encoding> reader(BasicString<Encoding>("Hello"));

    SECTION("Read single character")
    {
        AnsiChar c = 0;
        bool ok = reader.Read(c);

        REQUIRE(ok);
        REQUIRE(c == 'H');
    }

    SECTION("Read multiple characters")
    {
        AnsiChar buffer[5] = {};
        auto read = reader.Read(buffer, 5);

        REQUIRE(read == 5);
        REQUIRE(buffer[0] == 'H');
        REQUIRE(buffer[1] == 'e');
        REQUIRE(buffer[4] == 'o');
    }

    SECTION("Read past end returns partial")
    {
        AnsiChar buffer[10] = {};
        auto read = reader.Read(buffer, 10);

        REQUIRE(read == 5);
    }

    SECTION("Read after EOF returns false")
    {
        AnsiChar buffer[5] = {};
        reader.Read(buffer, 5);

        AnsiChar c = 0;
        REQUIRE_FALSE(reader.Read(c));
    }
}

TEST_CASE("StringReader cursor operations", "[IO][Reader]")
{
    using Encoding = Encoders::Ascii;

    StringReader<Encoding> reader(BasicString<Encoding>("ABCDE"));

    SECTION("HasCursor returns true")
    {
        REQUIRE(reader.HasCursor());
    }

    SECTION("Tell starts at zero")
    {
        REQUIRE(reader.Tell() == 0);
    }

    SECTION("Tell advances after read")
    {
        AnsiChar buffer[3] = {};
        reader.Read(buffer, 3);
        REQUIRE(reader.Tell() == 3);
    }

    SECTION("Seek to position")
    {
        reader.Seek(2);
        REQUIRE(reader.Tell() == 2);

        AnsiChar c = 0;
        reader.Read(c);
        REQUIRE(c == 'C');
    }

    SECTION("Rewind resets position")
    {
        AnsiChar buffer[3] = {};
        reader.Read(buffer, 3);

        reader.Rewind();
        REQUIRE(reader.Tell() == 0);

        AnsiChar c = 0;
        reader.Read(c);
        REQUIRE(c == 'A');
    }

    SECTION("GetSize returns string length")
    {
        REQUIRE(reader.GetSize() == 5);
    }

    SECTION("IsEof after full read")
    {
        REQUIRE_FALSE(reader.IsEof());

        AnsiChar buffer[5] = {};
        reader.Read(buffer, 5);
        REQUIRE(reader.IsEof());
    }
}

TEST_CASE("StringReader TransferTo", "[IO][Reader]")
{
    using Encoding = Encoders::Ascii;

    StringReader<Encoding> reader(BasicString<Encoding>("Hello World"));
    StringWriter<Encoding> writer;

    auto transferred = reader.TransferTo(writer);

    REQUIRE(transferred == 11);

    auto result = writer.ToString();
    REQUIRE(result.GetLength() == 11);
    REQUIRE(result == BasicString<Encoding>("Hello World"));
}
