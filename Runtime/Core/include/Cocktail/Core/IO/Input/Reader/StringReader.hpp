#ifndef COCKTAIL_CORE_IO_INPUT_READER_STRINGREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_STRINGREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>
#include <Cocktail/Core/String.hpp>

namespace Ck
{
    /**
     * \brief Reader that reads text from an in-memory string
     *
     * StringReader wraps a BasicString (or any string-like object) and provides
     * a Reader interface for sequential reading, seeking, and cursor operations.
     *
     * \tparam TEncoding The character encoding of the string
     * \tparam TAllocator Allocator type used by the internal string
     */
    template <typename TEncoding, typename TAllocator = SizedHeapAllocator<32>>
    class StringReader : public Reader<TEncoding>
    {
    public:

        /**
         * \brief Character type associated with the encoding
         */
        using CharType = typename Reader<TEncoding>::CharType;

        /**
         * \brief Size type associated with the encoding
         */
        using SizeType = typename Reader<TEncoding>::SizeType;

        /**
         * \brief Type of the string being read
         */
        using StringType = BasicString<TEncoding, TAllocator>;

        /**
         * \brief Constructor
         *
         * Wraps a string for reading. The string is moved into the reader.
         *
         * \param string The string to read
         */
        explicit StringReader(StringType string) :
            mCursor(0),
            mString(std::move(string))
        {
            /// Nothing to initialize
        }

        /**
         * \brief Read multiple characters into a buffer
         *
         * Copies up to \p length characters from the current cursor position
         * into \p text.
         *
         * \param text Destination buffer
         * \param length Maximum number of characters to read
         *
         * \return Number of characters actually read
         */
        SizeType Read(CharType* text, SizeType length) override
        {
            SizeType available = mString.GetLength() - mCursor;
            SizeType copyLength = std::min(available, length);

            Memory::Copy(text, mString.GetData() + mCursor, copyLength * sizeof(CharType));

            mCursor += copyLength;

            return copyLength;
        }

        /**
         * \brief Check if this reader supports cursor operations
         *
         * \return Always true for StringReader
         */
        bool HasCursor() const override
        {
            return true;
        }

        /**
         * \brief Move the read cursor to a specific position
         *
         * \param position The new position in characters
         *
         * \return The new cursor position
         */
        Uint64 Seek(Uint64 position) override
        {
            mCursor = std::min(mString.GetLength(), static_cast<SizeType>(position));
            return mCursor;
        }

        /**
         * \brief Reset the read cursor to the beginning of the string
         */
        void Rewind() override
        {
            mCursor = 0;
        }

        /**
         * \brief Get the current cursor position
         *
         * \return Cursor position in characters
         */
        Uint64 Tell() const override
        {
            return mCursor;
        }

        /**
         * \brief Get the total size of the string
         *
         * \return Length of the string in characters
         */
        SizeType GetSize() const override
        {
            return mString.GetLength();
        }

        /**
         * \brief Check if the end of the string has been reached
         *
         * \return True if the cursor is at the end of the string
         */
        bool IsEof() const override
        {
            return mCursor == mString.GetLength();
        }

    private:

        SizeType mCursor; ///< Current read position in the string
        StringType mString; ///< The string being read
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_STRINGREADER_HPP
