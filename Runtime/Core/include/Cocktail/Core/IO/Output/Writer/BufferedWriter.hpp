#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_BUFFEREDWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_BUFFEREDWRITER_HPP

#include <Cocktail/Core/IO/Output/OutputAccumulator.hpp>
#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>

namespace Ck
{
    template <typename TEncoding, typename TAllocator = SizedHeapAllocator<32>>
    class BufferedWriter : public Writer<TEncoding>, OutputAccumulator<typename TEncoding::CharType, TAllocator>
    {
    public:

        using CharType = typename Writer<TEncoding>::CharType;

        using SizeType = typename Writer<TEncoding>::SizeType;

        using EncodingType = typename Writer<TEncoding>::EncodingType;

        static constexpr SizeType DefaultBufferSize = 8192;

        explicit BufferedWriter(Writer<TEncoding>& destination, SizeType bufferSize = DefaultBufferSize) :
            OutputAccumulator<CharType, TAllocator>(bufferSize),
            mDestination(&destination)
        {
            /// Nothing
        }

        ~BufferedWriter() override
        {
            BufferedWriter::Flush();
        }

        void Write(const CharType* text, SizeType length) override
        {
            OutputAccumulator<CharType, TAllocator>::Append(text, length);
        }

        void Flush() override
        {
            OutputAccumulator<CharType, TAllocator>::Commit();
        }

    private:

        void DoCommit(const CharType* text, SizeType length) override
        {
            mDestination->Write(text, length);
        }

        Writer<TEncoding>* mDestination;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_BUFFEREDWRITER_HPP
