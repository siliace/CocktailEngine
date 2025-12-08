#ifndef COCKTAIL_CORE_IO_INPUT_READER_STRINGREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_STRINGREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>
#include <Cocktail/Core/String.hpp>

namespace Ck
{
    template <typename TEncoding, typename TAllocator = SizedHeapAllocator<32>>
    class StringReader : public Reader<TEncoding>
    {
    public:

        using CharType = typename Reader<TEncoding>::CharType;

        using SizeType = typename Reader<TEncoding>::SizeType;

        using StringType = BasicString<TEncoding, TAllocator>;

        explicit StringReader(StringType string) :
            mCursor(0),
            mString(std::move(string))
        {
            /// Nothing
        }

        SizeType Read(CharType* text, SizeType length) override
        {
            SizeType available = mString.GetLength() - mCursor;
            SizeType copyLength = std::min(available, length);

            Memory::Copy(text, mString.GetData() + mCursor, copyLength * sizeof(CharType));

            mCursor += copyLength;

            return copyLength;
        }

        bool HasCursor() const override
        {
            return true;
        }

        Uint64 Seek(Uint64 position) override
        {
            mCursor = std::min(mString.GetLength(), mCursor + position);
            return mCursor;
        }

        void Rewind() override
        {
            mCursor = 0;
        }

        Uint64 Tell() const override
        {
            return mCursor;
        }

        SizeType GetSize() const override
        {
            return mString.GetLength();
        }

        bool IsEof() const override
        {
            return mCursor == mString.GetLength();
        }

    private:

        SizeType mCursor;
        StringType mString;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_STRINGREADER_HPP
