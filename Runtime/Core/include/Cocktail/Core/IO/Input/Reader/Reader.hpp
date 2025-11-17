#ifndef COCKTAIL_CORE_IO_INPUT_READER_READER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_READER_HPP

#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>

namespace Ck
{
    template <typename>
    class Writer;

    template <typename TEncoding = Encoders::Text>
    class Reader
    {
    public:

        using EncodingType = TEncoding;

        using CharType = typename EncodingType::CharType;

        using SizeType = typename EncodingType::SizeType;

        virtual ~Reader() = default;

        virtual bool Read(CharType& c)
        {
            return Read(&c, 1) == 1;
        }

        virtual SizeType Read(CharType* text, SizeType length) = 0;

        virtual SizeType TransferTo(Writer<EncodingType>& writer)
        {
            const SizeType TransferBufferSize = 8196;

            SizeType transferred = 0;
            CharType transferBuffer[TransferBufferSize];
            while (!IsEof())
            {
                SizeType size = Read(transferBuffer, TransferBufferSize);
                writer.Write(transferBuffer, size);

                transferred += size;
            }

            return transferred;
        }

        virtual bool HasCursor() const = 0;

        virtual Uint64 Seek(Uint64 position) = 0;

        virtual void Rewind() = 0;

        virtual Uint64 Tell() const = 0;

        virtual SizeType GetSize() const = 0;

        virtual bool IsEof() const = 0;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_READER_HPP
