#ifndef COCKTAIL_CORE_IO_INPUT_STREAM_INPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_INPUT_STREAM_INPUTSTREAM_HPP

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>

namespace Ck
{
    template <typename TAllocator = SizedHeapAllocator<32>>
    class InputStream
    {
    public:

        using SizeType = typename TAllocator::SizeType;

        virtual ~InputStream() = default;

        virtual SizeType Read(Byte* buffer, SizeType length) = 0;

        virtual SizeType TransferTo(OutputStream<TAllocator>& outputStream)
        {
            const SizeType TransferBufferSize = 8196;

            SizeType transferred = 0;
            Byte transferBuffer[TransferBufferSize];
            while (!IsEof())
            {
                SizeType size = Read(transferBuffer, TransferBufferSize);

                /// Writes in output streams might be incomplete
                for (SizeType i = 0; i < size;)
                    i += outputStream.Write(transferBuffer + i, size - i);

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

#endif // COCKTAIL_CORE_IO_INPUT_STREAM_INPUTSTREAM_HPP
