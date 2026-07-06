#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Input/InputAccumulator.hpp>

using namespace Ck;

namespace
{
    /**
     * \brief Test implementation of InputAccumulator that feeds from a fixed array
     */
    class TestAccumulator : public InputAccumulator<Byte>
    {
    public:

        TestAccumulator(const Byte* data, Uint32 dataSize, Uint32 bufferSize) :
            InputAccumulator<Byte>(bufferSize),
            mData(data),
            mDataSize(dataSize),
            mPos(0)
        {
        }

    private:

        Uint32 DoAdvance(Byte* buffer, Uint32 bufferSize) override
        {
            Uint32 available = mDataSize - mPos;
            Uint32 toCopy = std::min(available, bufferSize);
            Memory::Copy(buffer, mData + mPos, toCopy);
            mPos += toCopy;
            return toCopy;
        }

        const Byte* mData;
        Uint32 mDataSize;
        Uint32 mPos;
    };
}

TEST_CASE("InputAccumulator single element extraction", "[IO][InputAccumulator]")
{
    Byte data[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    TestAccumulator acc(data, 5, 3); // buffer of 3

    SECTION("Extract elements one by one")
    {
        Byte e = 0;
        for (int i = 0; i < 5; ++i)
        {
            REQUIRE(acc.Extract(e));
            REQUIRE(e == data[i]);
        }

        REQUIRE_FALSE(acc.Extract(e));
    }
}

TEST_CASE("InputAccumulator bulk extraction", "[IO][InputAccumulator]")
{
    Byte data[] = { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80 };
    TestAccumulator acc(data, 8, 3); // buffer of 3

    SECTION("Extract all at once")
    {
        Byte buffer[8] = {};
        auto extracted = acc.Extract(buffer, 8);

        REQUIRE(extracted == 8);
        for (int i = 0; i < 8; ++i)
            REQUIRE(buffer[i] == data[i]);
    }

    SECTION("Extract more than available returns partial")
    {
        Byte buffer[10] = {};
        auto extracted = acc.Extract(buffer, 10);
        REQUIRE(extracted == 8);
    }
}

TEST_CASE("InputAccumulator IsInvalid", "[IO][InputAccumulator]")
{
    Byte data[] = { 0xAA, 0xBB };
    TestAccumulator acc(data, 2, 4);

    SECTION("Initially buffer is invalid (empty)")
    {
        REQUIRE(acc.IsInvalid());
    }

    SECTION("After extraction, may become valid")
    {
        Byte e = 0;
        acc.Extract(e);
        // After extracting 1 element from buffer of 4, the buffer was loaded
        // Position is 1, limit is 2, so still valid
        REQUIRE_FALSE(acc.IsInvalid());
    }
}
