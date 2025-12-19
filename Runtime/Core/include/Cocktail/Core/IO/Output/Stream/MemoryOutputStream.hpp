#ifndef COCTAIL_CORE_IO_OUTPUT_STREAM_MEMORYOUTPUTSTREAM_HPP
#define COCTAIL_CORE_IO_OUTPUT_STREAM_MEMORYOUTPUTSTREAM_HPP

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
    template <typename TAllocator = HeapAllocator>
    class MemoryOutputStream : public OutputStream<TAllocator>
    {
    public:

        using SizeType = typename TAllocator::SizeType;

        explicit MemoryOutputStream(BasicByteArray<TAllocator>& output) :
            mOutput(output)
        {
            /// Nothing
        }

        SizeType Write(const Byte* data, SizeType length) override
        {
            mOutput.Append(data, length);
            return length;
        }

        void Flush() override
        {
        }

    private:

        BasicByteArray<TAllocator>& mOutput;
    };
}

#endif // COCTAIL_CORE_IO_OUTPUT_STREAM_MEMORYOUTPUTSTREAM_HPP
