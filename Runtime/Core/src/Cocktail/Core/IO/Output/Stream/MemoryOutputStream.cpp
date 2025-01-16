#include <Cocktail/Core/IO/Output/Stream/MemoryOutputStream.hpp>

namespace Ck
{
    MemoryOutputStream::MemoryOutputStream(ByteArray& output) :
        mOutput(output)
    {
        /// Nothing
    }

    std::size_t MemoryOutputStream::Write(const void* data, std::size_t length)
    {
        mOutput.Append(data, length);
        return length;
    }

    void MemoryOutputStream::Flush()
    {
    }
}
