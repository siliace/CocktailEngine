#ifndef COCTAIL_CORE_IO_OUTPUT_STREAM_MEMORYOUTPUTSTREAM_HPP
#define COCTAIL_CORE_IO_OUTPUT_STREAM_MEMORYOUTPUTSTREAM_HPP

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
    class COCKTAIL_CORE_API MemoryOutputStream : public OutputStream
    {
    public:

        /**
         * \brief Constructor
         *
         * Create a new instance of MemoryOutputStream
         *
         * \param output The ByteArray to write
         */
        explicit MemoryOutputStream(ByteArray& output);
        
        std::size_t Write(const void* data, std::size_t length) override;

        void Flush() override;

    private:

        ByteArray& mOutput;
    };
}

#endif // COCTAIL_CORE_IO_OUTPUT_STREAM_MEMORYOUTPUTSTREAM_HPP