#ifndef COCKTAIL_CORE_IO_OUTPUT_STREAM_BUFFEREDOUTPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_OUTPUT_STREAM_BUFFEREDOUTPUTSTREAM_HPP

#include <Cocktail/Core/IO/Output/OutputAccumulator.hpp>
#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>

namespace Ck
{
    template <typename TAllocator = SizedHeapAllocator<32>>
    class BufferedOutputStream : public OutputStream<TAllocator>, OutputAccumulator<Byte, TAllocator>
    {
    public:

        using SizeType = typename TAllocator::SizeType;

        static constexpr SizeType DefaultBufferSize = 8192;

        explicit BufferedOutputStream(OutputStream<TAllocator>& destination, SizeType bufferSize = DefaultBufferSize) :
            OutputAccumulator<Byte, TAllocator>(bufferSize),
            mDestination(&destination)
        {
            /// Nothing
        }

        SizeType Write(const Byte* buffer, SizeType length) override
        {
            return OutputAccumulator<Byte, TAllocator>::Append(buffer, length);
        }

        void Flush() override
        {
            OutputAccumulator<Byte, TAllocator>::Commit();
        }

    private:

        void DoCommit(const Byte* buffer, SizeType bufferSize) override
        {
            mDestination->Write(buffer, bufferSize);
        }

        OutputStream<TAllocator>* mDestination;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_STREAM_BUFFEREDOUTPUTSTREAM_HPP
