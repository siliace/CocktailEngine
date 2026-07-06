#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Input/Reader/TranscodingReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/StringReader.hpp>

using namespace Ck;

TEST_CASE("TranscodingReader ASCII to ASCII (identity)", "[IO][TranscodingReader]")
{
    using SrcEncoding = Encoders::Ascii;
    using DstEncoding = Encoders::Ascii;

    StringReader<SrcEncoding> source(BasicString<SrcEncoding>("Hello"));
    TranscodingReader<SrcEncoding, DstEncoding> reader(source);

    SECTION("Read single character")
    {
        AnsiChar c = 0;
        REQUIRE(reader.Read(c));
        REQUIRE(c == 'H');
    }

    SECTION("Read all characters")
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

TEST_CASE("TranscodingReader ASCII to UTF-32", "[IO][TranscodingReader]")
{
    using SrcEncoding = Encoders::Ascii;
    using DstEncoding = Encoders::Utf32;

    StringReader<SrcEncoding> source(BasicString<SrcEncoding>("AB"));
    TranscodingReader<SrcEncoding, DstEncoding> reader(source);

    SECTION("Read transcodes each ASCII char to UTF-32 codepoint")
    {
        Utf32Char c = 0;
        REQUIRE(reader.Read(c));
        REQUIRE(c == U'A');

        REQUIRE(reader.Read(c));
        REQUIRE(c == U'B');

        REQUIRE_FALSE(reader.Read(c));
    }
}

TEST_CASE("TranscodingReader cursor operations for fixed-width encodings", "[IO][TranscodingReader]")
{
    using SrcEncoding = Encoders::Ascii;
    using DstEncoding = Encoders::Ascii;

    StringReader<SrcEncoding> source(BasicString<SrcEncoding>("ABCDE"));
    TranscodingReader<SrcEncoding, DstEncoding> reader(source);

    SECTION("HasCursor delegates to source")
    {
        REQUIRE(reader.HasCursor());
    }

    SECTION("GetSize returns correct count")
    {
        REQUIRE(reader.GetSize() == 5);
    }

    SECTION("Rewind works")
    {
        AnsiChar buffer[3] = {};
        reader.Read(buffer, 3);

        reader.Rewind();

        AnsiChar c = 0;
        reader.Read(c);
        REQUIRE(c == 'A');
    }
}
