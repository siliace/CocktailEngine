#ifndef COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>
#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>

namespace Ck
{
    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
    class InputStreamReader : public Reader<TEncoding>
    {
    public:

        using CharType = typename Reader<TEncoding>::CharType;

        using SizeType = typename Reader<TEncoding>::SizeType;

        using EncodingType = typename Reader<TEncoding>::EncodingType;

        explicit InputStreamReader(InputStream<TAllocator>& inputStream) :
            mInner(&inputStream)
        {
            /// Nothing
        }

        bool Read(CharType& c) override
        {
            return Reader<TEncoding>::Read(c);
        }

        SizeType Read(CharType* buffer, SizeType length) override
        {
            return mInner->Read(reinterpret_cast<Byte*>(buffer), length * sizeof(CharType)) / sizeof(CharType);
        }

        SizeType TransferTo(Writer<EncodingType>& writer) override
        {
            return Reader<TEncoding>::TransferTo(writer);
        }

        bool HasCursor() const override
        {
            return mInner->HasCursor();
        }

        Uint64 Seek(Uint64 position) override
        {
            return mInner->Seek(position / sizeof(CharType));
        }

        void Rewind() override
        {
            mInner->Rewind();
        }

        Uint64 Tell() const override
        {
            return mInner->Tell();
        }

        SizeType GetSize() const override
        {
            return mInner->GetSize() / sizeof(CharType);
        }

        bool IsEof() const override
        {
            return mInner->IsEof();
        }

    private:

        InputStream<TAllocator>* mInner;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP
