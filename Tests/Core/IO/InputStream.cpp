#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>
#include <Cocktail/Core/IO/Output/Stream/MemoryOutputStream.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

using namespace Ck;

TEST_CASE("MemoryInputStream basic read", "[IO][InputStream]")
{
    Byte data[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    MemoryInputStream<> stream(data, 5);

    SECTION("Read all bytes at once")
    {
        Byte buffer[5] = {};
        auto read = stream.Read(buffer, 5);

        REQUIRE(read == 5);
        REQUIRE(buffer[0] == 0x01);
        REQUIRE(buffer[4] == 0x05);
    }

    SECTION("Read in chunks")
    {
        Byte buffer[3] = {};
        auto read1 = stream.Read(buffer, 3);
        REQUIRE(read1 == 3);
        REQUIRE(buffer[0] == 0x01);
        REQUIRE(buffer[2] == 0x03);

        Byte buffer2[3] = {};
        auto read2 = stream.Read(buffer2, 3);
        REQUIRE(read2 == 2);
        REQUIRE(buffer2[0] == 0x04);
        REQUIRE(buffer2[1] == 0x05);
    }

    SECTION("Read from empty returns zero")
    {
        Byte buffer[5] = {};
        stream.Read(buffer, 5);

        Byte buffer2[1] = {};
        auto read = stream.Read(buffer2, 1);
        REQUIRE(read == 0);
    }
}

TEST_CASE("MemoryInputStream cursor operations", "[IO][InputStream]")
{
    Byte data[] = { 0x10, 0x20, 0x30, 0x40, 0x50 };
    MemoryInputStream<> stream(data, 5);

    SECTION("HasCursor returns true")
    {
        REQUIRE(stream.HasCursor());
    }

    SECTION("Tell starts at zero")
    {
        REQUIRE(stream.Tell() == 0);
    }

    SECTION("Tell advances after read")
    {
        Byte buffer[2] = {};
        stream.Read(buffer, 2);
        REQUIRE(stream.Tell() == 2);
    }

    SECTION("Seek to position")
    {
        stream.Seek(3);
        REQUIRE(stream.Tell() == 3);

        Byte buffer[1] = {};
        stream.Read(buffer, 1);
        REQUIRE(buffer[0] == 0x40);
    }

    SECTION("Seek beyond size clamps")
    {
        stream.Seek(100);
        REQUIRE(stream.Tell() == 5);
    }

    SECTION("Rewind resets to beginning")
    {
        Byte buffer[3] = {};
        stream.Read(buffer, 3);
        stream.Rewind();
        REQUIRE(stream.Tell() == 0);

        stream.Read(buffer, 1);
        REQUIRE(buffer[0] == 0x10);
    }

    SECTION("GetSize returns buffer length")
    {
        REQUIRE(stream.GetSize() == 5);
    }

    SECTION("IsEof after full read")
    {
        REQUIRE_FALSE(stream.IsEof());

        Byte buffer[5] = {};
        stream.Read(buffer, 5);
        REQUIRE(stream.IsEof());
    }
}

TEST_CASE("MemoryInputStream TransferTo", "[IO][InputStream]")
{
    Byte data[] = { 0xAA, 0xBB, 0xCC, 0xDD };
    MemoryInputStream<> input(data, 4);

    ByteArray output;
    MemoryOutputStream<> outputStream(output);

    auto transferred = input.TransferTo(outputStream);

    REQUIRE(transferred == 4);
    REQUIRE(output.GetSize() == 4);
    REQUIRE(output.GetData()[0] == 0xAA);
    REQUIRE(output.GetData()[3] == 0xDD);
}
