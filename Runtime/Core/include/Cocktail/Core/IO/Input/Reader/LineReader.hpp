#ifndef COCKTAIL_CORE_IO_INPUT_READER_LINEREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_LINEREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>
#include <Cocktail/Core/String.hpp>

namespace Ck
{
    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
    class LineReader
    {
    public:

        using CharType = typename Reader<TEncoding>::CharType;

        using SizeType = typename Reader<TEncoding>::SizeType;

        using EncodingType = typename Reader<TEncoding>::EncodingType;

        explicit LineReader(Reader<TEncoding>& source) :
            mSource(&source)
        {
            /// Nothing
        }

        Optional<BasicString<EncodingType, TAllocator>> ReadLine(bool ignoreEmptyLines)
        {
            BasicString<EncodingType, TAllocator> line;
            line.Reserve(1024); /// This should cover a vaste majority of cases

            if (!ReadLine(line, ignoreEmptyLines))
                return Optional<BasicString<EncodingType, TAllocator>>::Empty();

            return Optional<BasicString<EncodingType, TAllocator>>::Of(std::move(line));
        }

        bool ReadLine(BasicString<EncodingType, TAllocator>& line, bool ignoreEmptyLines = true)
        {
            line.Clear();

            CharType c;
            while (mSource->Read(c))
            {
                if (c == static_cast<CharType>('\n'))
                {
#ifdef COCKTAIL_OS_WINDOWS
                    line.TryLast().Then([&](CharType last) {
                        if (last == static_cast<CharType>(last))
                            line.PopLast();
                    });
#endif
                    if (!line.IsEmpty() || !ignoreEmptyLines)
                        return true;
                }
                else
                {
                    line.Append(c);
                }
            }

            return false;
        }

    private:

        Reader<EncodingType>* mSource;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_LINEREADER_HPP
