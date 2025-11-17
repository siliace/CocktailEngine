#ifndef COCKTAIL_CORE_IO_INPUT_INPUTACCUMULATOR_HPP
#define COCKTAIL_CORE_IO_INPUT_INPUTACCUMULATOR_HPP

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>

namespace Ck
{
    template <typename T, typename TAllocator = SizedHeapAllocator<32>>
    class InputAccumulator
    {
    public:

        using AllocatorType = typename TAllocator::template Typed<T>;
        using SizeType = typename TAllocator::SizeType;

        explicit InputAccumulator(SizeType size) :
            mPos(0),
            mLimit(0),
            mSize(size)
        {
            assert(mSize > 0);

            mAllocator.ResizeAllocation(0, mSize, sizeof(T));
        }

        virtual ~InputAccumulator() = default;

        bool Extract(T& e)
        {
            if (mPos == mLimit && Advance() == 0)
                return false;

            e = GetDataAt(mPos++);
            return true;
        }

        SizeType Extract(T* buffer, SizeType length)
        {
            SizeType accumulated = 0;
            while (accumulated < length)
            {
                SizeType validLength = mLimit - mPos;
                if (validLength == 0)
                {
                    validLength = Advance();
                    if (validLength == 0)
                        return accumulated;
                }

                SizeType copyLength = std::min(validLength, length - accumulated);

                std::memcpy(buffer + accumulated, mAllocator.GetAllocation() + mPos, copyLength * sizeof(T));
                accumulated += copyLength;
                mPos += copyLength;
            }

            return accumulated;
        }

        bool IsInvalid() const
        {
            return mPos == mLimit;
        }

    protected:

        void Invalidate()
        {
            mPos = mLimit = 0;
        }

        SizeType Advance()
        {
            assert(mPos == mLimit);

            Invalidate();
            SizeType count = DoAdvance(GetData(), mSize);
            if (count > 0)
                mLimit = count;

            return count;
        }

        SizeType GetBufferPosition() const
        {
            return mPos;
        }

        T* GetData() const
        {
            return mAllocator.GetAllocation();
        }

        T& GetDataAt(SizeType index) const
        {
            assert(index < mSize);
            return *(mAllocator.GetAllocation() + index);
        }

    private:

        virtual SizeType DoAdvance(T* buffer, SizeType bufferSize) = 0;

        SizeType mPos;
        SizeType mLimit;
        SizeType mSize;
        AllocatorType mAllocator;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_INPUTACCUMULATOR_HPP
