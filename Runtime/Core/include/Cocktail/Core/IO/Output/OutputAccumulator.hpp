#ifndef COCKTAIL_CORE_IO_OUTPUT_OUTPUTACCUMULATOR_HPP
#define COCKTAIL_CORE_IO_OUTPUT_OUTPUTACCUMULATOR_HPP

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>

namespace Ck
{
    template <typename E, typename TAllocator = SizedHeapAllocator<32>>
    class OutputAccumulator
    {
    public:

        using ElementAllocatorType = std::conditional_t<TAllocator::IsTyped, typename TAllocator::template Typed<E>, typename TAllocator::Raw>;

        using SizeType = typename TAllocator::SizeType;

        explicit OutputAccumulator(SizeType size) :
            mPos(0),
            mSize(size)
        {
            assert(mSize > 0);

            mAllocator.ResizeAllocation(0, mSize, sizeof(E));
        }

        virtual ~OutputAccumulator() = default;

        void Append(const E* data, SizeType size)
        {
            if (mPos + size <= mSize)
            {
                std::memcpy(mAllocator.GetAllocation() + mPos, data, size * sizeof(E));
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
                    std::memcpy(mAllocator.GetAllocation() + mPos, data, size * sizeof(E));
                    mPos += size;
                }
            }
        }

        void Commit()
        {
            if (mPos == 0)
                return;

            DoCommit(mAllocator.GetAllocation(), mPos);

            mPos = 0;
        }

    private:

        virtual void DoCommit(const E* buffer, SizeType bufferSize) = 0;

        SizeType mPos;
        SizeType mSize;
        ElementAllocatorType mAllocator;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_OUTPUTACCUMULATOR_HPP
