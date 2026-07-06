#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Output/Writer/OutputStreamWriter.hpp>
#include <Cocktail/Core/IO/Output/Stream/MemoryOutputStream.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

using namespace Ck;

TEST_CASE("OutputStreamWriter writes encoded text to byte stream", "[IO][OutputStreamWriter]")
{
    using Encoding = Encoders::Ascii;

    ByteArray output;
    MemoryOutputStream<> stream(output);
    OutputStreamWriter<Encoding> writer(stream);

    SECTION("Write text as bytes")
    {
        const AnsiChar text[] = "Hello";
        writer.Write(text, 5);

        REQUIRE(output.GetSize() == 5);
        REQUIRE(output.GetData()[0] == 'H');
        REQUIRE(output.GetData()[4] == 'o');
    }

    SECTION("Multiple writes")
    {
        const AnsiChar text1[] = "AB";
        const AnsiChar text2[] = "CD";

        writer.Write(text1, 2);
        writer.Write(text2, 2);

        REQUIRE(output.GetSize() == 4);
        REQUIRE(output.GetData()[0] == 'A');
        REQUIRE(output.GetData()[3] == 'D');
    }

    SECTION("Flush delegates to stream")
    {
        const AnsiChar text[] = "X";
        writer.Write(text, 1);
        writer.Flush(); // should not crash

        REQUIRE(output.GetSize() == 1);
    }
}
