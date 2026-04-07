#ifndef COCKTAIL_CORE_IO_OUTPUT_OUTPUTACCUMULATOR_HPP
#define COCKTAIL_CORE_IO_OUTPUT_OUTPUTACCUMULATOR_HPP

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>

namespace Ck
{
    /**
     * \brief Base class for buffering elements before writing them to an output destination
     *
     * OutputAccumulator provides an internal buffer to accumulate elements of type E
     * before committing them to an output destination. This reduces the number of write
     * operations and improves performance when writing small chunks.
     *
     * \tparam E Type of elements to buffer (e.g., Byte or CharType)
     * \tparam TAllocator Allocator type used for the internal buffer
     */
    template <typename E, typename TAllocator = SizedHeapAllocator<32>>
    class OutputAccumulator
    {
    public:

        /**
         * \brief Allocator type used for buffer allocation
         */
        using AllocatorType = typename TAllocator::template ForType<E>;

        /**
         * \brief Size type used for buffer operations
         */
        using SizeType = typename AllocatorType::SizeType;

        /**
         * \brief Constructor
         *
         * Allocates an internal buffer of size \p size elements.
         *
         * \param size Number of elements to allocate in the buffer
         */
        explicit OutputAccumulator(SizeType size) :
            mPos(0),
            mSize(size)
        {
            assert(mSize > 0);

            mBuffer = mAllocator.Allocate(0, mSize, sizeof(E));
        }

        /**
         * \brief Destructor
         *
         * Deallocates the internal buffer.
         */
        virtual ~OutputAccumulator()
        {
            mAllocator.Deallocate(mBuffer);
        }

        /**
         * \brief Append elements to the buffer
         *
         * Copies \p size elements from \p data into the internal buffer. If the buffer
         * becomes full, it will be automatically committed to the output destination.
         * If \p size is larger than the buffer, it will bypass the buffer and be written
         * directly via DoCommit.
         *
         * \param data Pointer to the elements to append
         * \param size Number of elements to append
         */
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

        /**
         * \brief Commit buffered elements to the output
         *
         * Calls DoCommit with the contents of the buffer and resets the buffer position.
         * If the buffer is empty, this function does nothing.
         */
        void Commit()
        {
            if (mPos == 0)
                return;

            DoCommit(mBuffer, mPos);

            mPos = 0;
        }

    protected:

        /**
         * \brief Perform the actual write of elements to the output destination
         *
         * Must be implemented by derived classes. This function is called when
         * the buffer is full or when Commit() is called.
         *
         * \param buffer Pointer to elements to commit
         * \param bufferSize Number of elements to commit
         */
        virtual void DoCommit(const E* buffer, SizeType bufferSize) = 0;

    private:

        SizeType mPos; ///< Current position in the buffer (number of elements filled)
        SizeType mSize; ///< Total size of the buffer
        E* mBuffer; ///< Internal buffer
        TAllocator mAllocator; ///< Allocator for the buffer
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_OUTPUTACCUMULATOR_HPP
