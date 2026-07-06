#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Output/Stream/BufferedOutputStream.hpp>
#include <Cocktail/Core/IO/Output/Stream/MemoryOutputStream.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

using namespace Ck;

TEST_CASE("BufferedOutputStream buffers writes", "[IO][BufferedOutputStream]")
{
    ByteArray output;
    MemoryOutputStream<> inner(output);
    BufferedOutputStream<> buffered(inner, 8); // buffer of 8 bytes

    SECTION("Small writes are buffered")
    {
        Byte data[] = { 0x01, 0x02, 0x03 };
        buffered.Write(data, 3);

        // Data should not yet be in output (still in buffer)
        // After flush, it should appear
        buffered.Flush();

        REQUIRE(output.GetSize() == 3);
        REQUIRE(output.GetData()[0] == 0x01);
        REQUIRE(output.GetData()[2] == 0x03);
    }

    SECTION("Buffer overflow triggers commit")
    {
        Byte data1[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
        Byte data2[] = { 0x06, 0x07, 0x08, 0x09, 0x0A };

        buffered.Write(data1, 5);
        buffered.Write(data2, 5); // exceeds 8-byte buffer, triggers flush

        buffered.Flush(); // flush remaining

        REQUIRE(output.GetSize() == 10);
        REQUIRE(output.GetData()[0] == 0x01);
        REQUIRE(output.GetData()[9] == 0x0A);
    }

    SECTION("Large write bypasses buffer")
    {
        Byte data[16] = {};
        for (int i = 0; i < 16; ++i)
            data[i] = static_cast<Byte>(i + 1);

        buffered.Write(data, 16); // larger than buffer, bypasses
        buffered.Flush();

        REQUIRE(output.GetSize() == 16);
        REQUIRE(output.GetData()[0] == 0x01);
        REQUIRE(output.GetData()[15] == 0x10);
    }
}

TEST_CASE("BufferedOutputStream flush on empty is no-op", "[IO][BufferedOutputStream]")
{
    ByteArray output;
    MemoryOutputStream<> inner(output);
    BufferedOutputStream<> buffered(inner, 8);

    buffered.Flush();
    REQUIRE(output.GetSize() == 0);
}
