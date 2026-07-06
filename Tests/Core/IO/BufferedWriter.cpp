#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Output/Writer/BufferedWriter.hpp>
#include <Cocktail/Core/IO/Output/Writer/StringWriter.hpp>

using namespace Ck;

TEST_CASE("BufferedWriter buffers writes", "[IO][BufferedWriter]")
{
    using Encoding = Encoders::Ascii;

    StringWriter<Encoding> inner;
    BufferedWriter<Encoding> buffered(inner, 8);

    SECTION("Small writes are buffered until flush")
    {
        const AnsiChar text[] = "Hi";
        buffered.Write(text, 2);

        // inner may or may not have data yet (depends on buffer not being full)
        buffered.Flush();

        auto result = inner.ToString();
        REQUIRE(result == BasicString<Encoding>("Hi"));
    }

    SECTION("Buffer overflow triggers commit")
    {
        const AnsiChar text1[] = "ABCDE"; // 5 chars
        const AnsiChar text2[] = "FGHIJ"; // 5 chars, total 10 > buffer of 8

        buffered.Write(text1, 5);
        buffered.Write(text2, 5);
        buffered.Flush();

        auto result = inner.ToString();
        REQUIRE(result == BasicString<Encoding>("ABCDEFGHIJ"));
    }

    SECTION("Large write bypasses buffer")
    {
        const AnsiChar text[] = "ABCDEFGHIJKLMNOP"; // 16 chars > buffer of 8
        buffered.Write(text, 16);
        buffered.Flush();

        auto result = inner.ToString();
        REQUIRE(result.GetLength() == 16);
    }
}

TEST_CASE("BufferedWriter flush on empty is no-op", "[IO][BufferedWriter]")
{
    using Encoding = Encoders::Ascii;

    StringWriter<Encoding> inner;
    BufferedWriter<Encoding> buffered(inner, 8);

    buffered.Flush();
    auto result = inner.ToString();
    REQUIRE(result.IsEmpty());
}

TEST_CASE("BufferedWriter destructor flushes", "[IO][BufferedWriter]")
{
    using Encoding = Encoders::Ascii;

    StringWriter<Encoding> inner;

    {
        BufferedWriter<Encoding> buffered(inner, 8);
        const AnsiChar text[] = "Data";
        buffered.Write(text, 4);
        // destructor should flush
    }

    auto result = inner.ToString();
    REQUIRE(result == BasicString<Encoding>("Data"));
}
