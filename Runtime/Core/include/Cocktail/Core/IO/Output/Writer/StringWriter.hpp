#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_STRINGWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_STRINGWRITER_HPP

#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>
#include <Cocktail/Core/String.hpp>

namespace Ck
{
    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
    class StringWriter : public Writer<TEncoding>
    {
    public:

        using CharType = typename Writer<TEncoding>::CharType;

        using SizeType = typename Writer<TEncoding>::SizeType;

        void Write(const CharType* text, SizeType length) override
        {
            mBuffer.Append(text, length);
        }

        void Flush() override
        {
            /// Nothing
        }

        BasicString<TEncoding, TAllocator> ToString() const
        {
            return mBuffer;
        }

    private:

        BasicString<TEncoding, TAllocator> mBuffer;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_STRINGWRITER_HPP
