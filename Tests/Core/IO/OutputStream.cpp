#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Output/Stream/MemoryOutputStream.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

using namespace Ck;

TEST_CASE("MemoryOutputStream basic write", "[IO][OutputStream]")
{
    ByteArray output;
    MemoryOutputStream<> stream(output);

    SECTION("Write appends bytes")
    {
        Byte data[] = { 0x01, 0x02, 0x03 };
        auto written = stream.Write(data, 3);

        REQUIRE(written == 3);
        REQUIRE(output.GetSize() == 3);
        REQUIRE(output.GetData()[0] == 0x01);
        REQUIRE(output.GetData()[2] == 0x03);
    }

    SECTION("Multiple writes append sequentially")
    {
        Byte data1[] = { 0xAA, 0xBB };
        Byte data2[] = { 0xCC, 0xDD };

        stream.Write(data1, 2);
        stream.Write(data2, 2);

        REQUIRE(output.GetSize() == 4);
        REQUIRE(output.GetData()[0] == 0xAA);
        REQUIRE(output.GetData()[1] == 0xBB);
        REQUIRE(output.GetData()[2] == 0xCC);
        REQUIRE(output.GetData()[3] == 0xDD);
    }

    SECTION("Write zero bytes does nothing")
    {
        Byte data[] = { 0xFF };
        auto written = stream.Write(data, 0);

        REQUIRE(written == 0);
        REQUIRE(output.GetSize() == 0);
    }
}

TEST_CASE("MemoryOutputStream flush is no-op", "[IO][OutputStream]")
{
    ByteArray output;
    MemoryOutputStream<> stream(output);

    Byte data[] = { 0x42 };
    stream.Write(data, 1);
    stream.Flush(); // should not crash or lose data

    REQUIRE(output.GetSize() == 1);
    REQUIRE(output.GetData()[0] == 0x42);
}
