#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Output/Writer/TranscodingWriter.hpp>
#include <Cocktail/Core/IO/Output/Writer/StringWriter.hpp>

using namespace Ck;

TEST_CASE("TranscodingWriter ASCII to ASCII (identity)", "[IO][TranscodingWriter]")
{
    using SrcEncoding = Encoders::Ascii;
    using DstEncoding = Encoders::Ascii;

    StringWriter<DstEncoding> inner;
    TranscodingWriter<SrcEncoding, DstEncoding> writer(inner);

    SECTION("Write text passes through")
    {
        const AnsiChar text[] = "Hello";
        writer.Write(text, 5);

        auto result = inner.ToString();
        REQUIRE(result == BasicString<DstEncoding>("Hello"));
    }
}

TEST_CASE("TranscodingWriter ASCII to UTF-32", "[IO][TranscodingWriter]")
{
    using SrcEncoding = Encoders::Ascii;
    using DstEncoding = Encoders::Utf32;

    StringWriter<DstEncoding> inner;
    TranscodingWriter<SrcEncoding, DstEncoding> writer(inner);

    SECTION("ASCII chars are transcoded to UTF-32")
    {
        const AnsiChar text[] = "AB";
        writer.Write(text, 2);

        auto result = inner.ToString();
        REQUIRE(result.GetLength() == 2);
        REQUIRE(result.At(0) == U'A');
        REQUIRE(result.At(1) == U'B');
    }
}

TEST_CASE("TranscodingWriter flush delegates", "[IO][TranscodingWriter]")
{
    using SrcEncoding = Encoders::Ascii;
    using DstEncoding = Encoders::Ascii;

    StringWriter<DstEncoding> inner;
    TranscodingWriter<SrcEncoding, DstEncoding> writer(inner);

    const AnsiChar text[] = "Test";
    writer.Write(text, 4);
    writer.Flush(); // should not crash

    auto result = inner.ToString();
    REQUIRE(result == BasicString<DstEncoding>("Test"));
}
