#ifndef COCKTAIL_CORE_IO_INPUT_INPUTACCUMULATOR_HPP
#define COCKTAIL_CORE_IO_INPUT_INPUTACCUMULATOR_HPP

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>

namespace Ck
{
    /**
     * \brief Generic input buffer to accumulate elements from a source
     *
     * InputAccumulator provides a reusable buffer that allows derived classes
     * to efficiently fetch and extract elements in chunks from a data source.
     *
     * It is typically used as a base class for readers (e.g., BufferedReader)
     * to reduce the number of direct reads from the underlying source.
     *
     * \tparam T The type of elements to accumulate
     * \tparam TAllocator Allocator type used to manage the internal buffer
     */
    template <typename T, typename TAllocator = SizedHeapAllocator<32>>
    class InputAccumulator
    {
    public:

        /**
         * \brief Type of elements stored in the buffer
         */
        using ElementType = T;

        /**
         * \brief Allocator type used to allocate the buffer
         */
        using AllocatorType = TAllocator;

        /**
         * \brief Type used for sizes and counts
         */
        using SizeType = typename AllocatorType::SizeType;

        /**
         * \brief Allocator specialized for ElementType
         */
        using ElementAllocatorType = typename AllocatorType::template ForType<ElementType>;

        /**
         * \brief Constructor
         *
         * Allocates an internal buffer of \p size elements.
         *
         * \param size The size of the buffer (must be > 0)
         */
        explicit InputAccumulator(SizeType size) :
            mPos(0),
            mLimit(0),
            mSize(size)
        {
            assert(mSize > 0);
            mBuffer = mAllocator.Allocate(mSize);
        }

        /**
         * \brief Destructor
         *
         * Frees the internal buffer.
         */
        virtual ~InputAccumulator()
        {
            mAllocator.Deallocate(mBuffer);
        }

        /**
         * \brief Extract a single element from the buffer
         *
         * \param e The element to extract
         *
         * \return True if an element was successfully extracted, false if no more data is available
         */
        bool Extract(T& e)
        {
            if (mPos == mLimit && Advance() == 0)
                return false;

            e = GetDataAt(mPos++);
            return true;
        }

        /**
         * \brief Extract multiple elements into a buffer
         *
         * Copies up to \p length elements into \p buffer. If the internal buffer
         * is empty, more data is fetched using DoAdvance().
         *
         * \param buffer Destination buffer
         * \param length Maximum number of elements to extract
         *
         * \return Number of elements actually extracted
         */
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
                Memory::Copy(buffer + accumulated, mBuffer + mPos, copyLength * sizeof(T));
                accumulated += copyLength;
                mPos += copyLength;
            }

            return accumulated;
        }

        /**
         * \brief Check if the buffer is currently empty
         *
         * \return True if no data is available in the buffer, false otherwise
         */
        bool IsInvalid() const
        {
            return mPos == mLimit;
        }

    protected:

        /**
         * \brief Invalidate the buffer
         *
         * Resets the read position and limit.
         */
        void Invalidate()
        {
            mPos = mLimit = 0;
        }

        /**
         * \brief Advance the buffer by fetching more data
         *
         * Calls the virtual DoAdvance to fill the buffer.
         *
         * \return Number of elements read into the buffer
         */
        SizeType Advance()
        {
            assert(mPos == mLimit);
            Invalidate();
            SizeType count = DoAdvance(GetData(), mSize);
            if (count > 0)
                mLimit = count;
            return count;
        }

        /**
         * \brief Get the current buffer read position
         *
         * \return Position index in the buffer
         */
        SizeType GetBufferPosition() const
        {
            return mPos;
        }

        /**
         * \brief Get pointer to the buffer
         *
         * \return Pointer to the start of the buffer
         */
        ElementType* GetData() const
        {
            return mBuffer;
        }

        /**
         * \brief Access an element at a specific index in the buffer
         *
         * \param index Index of the element
         * \return Reference to the element
         */
        ElementType& GetDataAt(SizeType index) const
        {
            assert(index < mSize);
            return *(mBuffer + index);
        }

    private:

        /**
         * \brief Fill the buffer with data from the source
         *
         * Must be implemented by derived classes.
         *
         * \param buffer Destination buffer
         * \param bufferSize Maximum number of elements to fill
         *
         * \return Number of elements actually filled
         */
        virtual SizeType DoAdvance(ElementType* buffer, SizeType bufferSize) = 0;

        SizeType mPos; ///< Current read position in the buffer
        SizeType mLimit; ///< End of valid data in the buffer
        SizeType mSize; ///< Total buffer size
        ElementType* mBuffer; ///< Pointer to allocated buffer
        ElementAllocatorType mAllocator; ///< Allocator instance
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_INPUTACCUMULATOR_HPP
