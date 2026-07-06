#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>

using namespace Ck;

TEST_CASE("InputStreamReader reads ASCII from byte stream", "[IO][InputStreamReader]")
{
    using Encoding = Encoders::Ascii;

    const AnsiChar rawData[] = "Hello";
    MemoryInputStream<> stream(reinterpret_cast<const Byte*>(rawData), 5);
    InputStreamReader<Encoding> reader(stream);

    SECTION("Read single character")
    {
        AnsiChar c = 0;
        REQUIRE(reader.Read(c));
        REQUIRE(c == 'H');
    }

    SECTION("Read multiple characters")
    {
        AnsiChar buffer[5] = {};
        auto read = reader.Read(buffer, 5);

        REQUIRE(read == 5);
        REQUIRE(buffer[0] == 'H');
        REQUIRE(buffer[4] == 'o');
    }

    SECTION("IsEof after reading all")
    {
        AnsiChar buffer[5] = {};
        reader.Read(buffer, 5);
        REQUIRE(reader.IsEof());
    }
}

TEST_CASE("InputStreamReader cursor operations", "[IO][InputStreamReader]")
{
    using Encoding = Encoders::Ascii;

    const AnsiChar rawData[] = "ABCDE";
    MemoryInputStream<> stream(reinterpret_cast<const Byte*>(rawData), 5);
    InputStreamReader<Encoding> reader(stream);

    SECTION("HasCursor delegates to stream")
    {
        REQUIRE(reader.HasCursor());
    }

    SECTION("Seek in characters translates to bytes")
    {
        reader.Seek(3);

        AnsiChar c = 0;
        reader.Read(c);
        REQUIRE(c == 'D');
    }

    SECTION("GetSize returns character count")
    {
        REQUIRE(reader.GetSize() == 5);
    }

    SECTION("Rewind resets")
    {
        AnsiChar buffer[3] = {};
        reader.Read(buffer, 3);

        reader.Rewind();

        AnsiChar c = 0;
        reader.Read(c);
        REQUIRE(c == 'A');
    }
}
