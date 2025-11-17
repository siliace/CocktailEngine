#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_OUTPUTSTREAMWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_OUTPUTSTREAMWRITER_HPP

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>

namespace Ck
{
    template <typename TEncoding, typename TAllocator = SizedHeapAllocator<32>>
    class OutputStreamWriter : public Writer<TEncoding>
    {
    public:

        using CharType = typename Writer<TEncoding>::CharType;

        using SizeType = typename Writer<TEncoding>::SizeType;

        using EncodingType = typename Writer<TEncoding>::EncodingType;

        explicit OutputStreamWriter(OutputStream<TAllocator>& destination) :
            mDestination(&destination)
        {
            /// Nothing
        }

        void Write(const CharType* text, SizeType length) override
        {
            mDestination->Write(reinterpret_cast<const Byte*>(text), length * sizeof(CharType));
        }

        void Flush() override
        {
            mDestination->Flush();
        }

    private:

        OutputStream<TAllocator>* mDestination;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_OUTPUTSTREAMWRITER_HPP
