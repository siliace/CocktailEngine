#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Input/Stream/BufferedInputStream.hpp>
#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>

using namespace Ck;

TEST_CASE("BufferedInputStream basic read", "[IO][BufferedInputStream]")
{
    Byte data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
    MemoryInputStream<> inner(data, 8);
    BufferedInputStream<> buffered(inner, 4); // buffer of 4 bytes

    SECTION("Read all data through buffer")
    {
        Byte buffer[8] = {};
        auto read = buffered.Read(buffer, 8);

        REQUIRE(read == 8);
        for (int i = 0; i < 8; ++i)
            REQUIRE(buffer[i] == data[i]);
    }

    SECTION("Read in small chunks")
    {
        Byte buffer[2] = {};

        auto r1 = buffered.Read(buffer, 2);
        REQUIRE(r1 == 2);
        REQUIRE(buffer[0] == 0x01);
        REQUIRE(buffer[1] == 0x02);

        auto r2 = buffered.Read(buffer, 2);
        REQUIRE(r2 == 2);
        REQUIRE(buffer[0] == 0x03);
        REQUIRE(buffer[1] == 0x04);
    }

    SECTION("Read past end returns partial")
    {
        Byte buffer[10] = {};
        auto read = buffered.Read(buffer, 10);

        REQUIRE(read == 8);
    }
}

TEST_CASE("BufferedInputStream cursor operations", "[IO][BufferedInputStream]")
{
    Byte data[] = { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80 };
    MemoryInputStream<> inner(data, 8);
    BufferedInputStream<> buffered(inner, 4);

    SECTION("Tell reflects logical position")
    {
        REQUIRE(buffered.Tell() == 0);

        Byte buffer[2] = {};
        buffered.Read(buffer, 2);
        REQUIRE(buffered.Tell() == 2);
    }

    SECTION("Seek invalidates buffer and repositions")
    {
        Byte buffer[2] = {};
        buffered.Read(buffer, 2); // fill buffer, read 2

        buffered.Seek(6);
        REQUIRE(buffered.Tell() == 6);

        buffered.Read(buffer, 2);
        REQUIRE(buffer[0] == 0x70);
        REQUIRE(buffer[1] == 0x80);
    }

    SECTION("Rewind goes back to start")
    {
        Byte buffer[4] = {};
        buffered.Read(buffer, 4);

        buffered.Rewind();
        REQUIRE(buffered.Tell() == 0);

        buffered.Read(buffer, 1);
        REQUIRE(buffer[0] == 0x10);
    }

    SECTION("IsEof after reading all data")
    {
        REQUIRE_FALSE(buffered.IsEof());

        Byte buffer[8] = {};
        buffered.Read(buffer, 8);
        REQUIRE(buffered.IsEof());
    }

    SECTION("GetSize returns underlying stream size")
    {
        REQUIRE(buffered.GetSize() == 8);
    }

    SECTION("HasCursor delegates to underlying")
    {
        REQUIRE(buffered.HasCursor());
    }
}
