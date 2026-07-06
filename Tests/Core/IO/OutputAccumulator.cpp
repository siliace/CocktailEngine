#include <catch2/catch_all.hpp>

#include <Cocktail/Core/IO/Output/OutputAccumulator.hpp>

using namespace Ck;

namespace
{
    /**
     * \brief Test implementation of OutputAccumulator that collects commits into a vector
     */
    class TestOutputAccumulator : public OutputAccumulator<Byte>
    {
    public:

        explicit TestOutputAccumulator(Uint32 bufferSize) :
            OutputAccumulator<Byte>(bufferSize),
            mTotalCommitted(0)
        {
        }

        Uint32 GetTotalCommitted() const { return mTotalCommitted; }
        const Byte* GetCommittedData() const { return mCommitted; }

        // Expose for testing
        void TestAppend(const Byte* data, Uint32 size)
        {
            Append(data, size);
        }

        void TestCommit()
        {
            Commit();
        }

    private:

        void DoCommit(const Byte* buffer, Uint32 bufferSize) override
        {
            Memory::Copy(mCommitted + mTotalCommitted, buffer, bufferSize);
            mTotalCommitted += bufferSize;
        }

        Byte mCommitted[256] = {};
        Uint32 mTotalCommitted;
    };
}

TEST_CASE("OutputAccumulator buffers and commits", "[IO][OutputAccumulator]")
{
    TestOutputAccumulator acc(4); // buffer of 4 bytes

    SECTION("Small append stays in buffer until commit")
    {
        Byte data[] = { 0x01, 0x02 };
        acc.TestAppend(data, 2);

        REQUIRE(acc.GetTotalCommitted() == 0);

        acc.TestCommit();
        REQUIRE(acc.GetTotalCommitted() == 2);
        REQUIRE(acc.GetCommittedData()[0] == 0x01);
        REQUIRE(acc.GetCommittedData()[1] == 0x02);
    }

    SECTION("Buffer overflow triggers commit")
    {
        Byte data1[] = { 0x01, 0x02, 0x03 };
        Byte data2[] = { 0x04, 0x05, 0x06 };

        acc.TestAppend(data1, 3); // fits in buffer of 4
        REQUIRE(acc.GetTotalCommitted() == 0);

        acc.TestAppend(data2, 3); // 3 + 3 = 6 > 4, triggers commit of first 3
        // After commit of first chunk, second chunk fits in buffer
        acc.TestCommit(); // flush remaining

        REQUIRE(acc.GetTotalCommitted() == 6);
        REQUIRE(acc.GetCommittedData()[0] == 0x01);
        REQUIRE(acc.GetCommittedData()[5] == 0x06);
    }

    SECTION("Large append bypasses buffer")
    {
        Byte data[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE };
        acc.TestAppend(data, 5); // 5 > buffer of 4, bypasses

        REQUIRE(acc.GetTotalCommitted() == 5);
        REQUIRE(acc.GetCommittedData()[0] == 0xAA);
        REQUIRE(acc.GetCommittedData()[4] == 0xEE);
    }

    SECTION("Commit on empty buffer is no-op")
    {
        acc.TestCommit();
        REQUIRE(acc.GetTotalCommitted() == 0);
    }
}
