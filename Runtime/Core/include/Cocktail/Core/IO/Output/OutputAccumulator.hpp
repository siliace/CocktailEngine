#ifndef COCKTAIL_CORE_IO_OUTPUT_OUTPUTACCUMULATOR_HPP
#define COCKTAIL_CORE_IO_OUTPUT_OUTPUTACCUMULATOR_HPP

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>

namespace Ck
{
    template <typename E, typename TAllocator = SizedHeapAllocator<32>>
    class OutputAccumulator
    {
    public:

        using AllocatorType = typename TAllocator::template ForType<E>;

        using SizeType = typename AllocatorType::SizeType;

        explicit OutputAccumulator(SizeType size) :
            mPos(0),
            mSize(size)
        {
            assert(mSize > 0);

            mBuffer = mAllocator.Allocate(0, mSize, sizeof(E));
        }

        virtual ~OutputAccumulator()
        {
            mAllocator.Deallocate(mBuffer);
        }

        void Append(const E* data, SizeType size)
        {
            if (mPos + size <= mSize)
            {
                Memory::Copy(mBuffer + mPos, data, size * sizeof(E));
                mPos += size;
            }
            else
            {
                Commit();
                if (size >= mSize)
                {
                    DoCommit(data, size);
                }
                else
                {
                    Memory::Copy(mBuffer + mPos, data, size * sizeof(E));
                    mPos += size;
                }
            }
        }

        void Commit()
        {
            if (mPos == 0)
                return;

            DoCommit(mBuffer, mPos);

            mPos = 0;
        }

    private:

        virtual void DoCommit(const E* buffer, SizeType bufferSize) = 0;

        SizeType mPos;
        SizeType mSize;
        E* mBuffer;
        TAllocator mAllocator;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_OUTPUTACCUMULATOR_HPP
