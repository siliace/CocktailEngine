#ifndef COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>

namespace Ck
{
    template <typename TAllocator = HeapAllocator>
    class OutputStream
    {
    public:

        using SizeType = typename TAllocator::SizeType;

        virtual ~OutputStream() = default;

        virtual SizeType Write(const Byte* data, SizeType length) = 0;

        virtual void Flush() = 0;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP
