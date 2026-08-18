#include <limits>
#include <system_error>

#include <CocktailEngine/Core/Memory/Memory.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileContent.hpp>

namespace Ck::Detail
{
    namespace
    {
        /**
         * \brief Largest amount of bytes a single in-memory file can hold
         */
        constexpr Uint64 MaxContentSize = std::numeric_limits<ByteArray::SizeType>::max();
    }

    Uint64 MemoryFileContent::GetSize() const
    {
        return mSize;
    }

    Uint64 MemoryFileContent::Read(Uint64 offset, void* buffer, Uint64 length) const
    {
        if (length == 0 || offset >= mSize)
            return 0;

        const Uint64 available = mSize - offset;
        const Uint64 count = length < available ? length : available;

        Memory::Copy(buffer, mBuffer.GetData() + offset, static_cast<std::size_t>(count));

        return count;
    }

    void MemoryFileContent::Write(Uint64 offset, const void* buffer, Uint64 length)
    {
        if (length == 0)
            return;

        // Writing past the end of the file leaves a hole that must read back as zeros
        if (offset > mSize)
            Resize(offset);

        const Uint64 end = offset + length;
        if (end > mSize)
        {
            Reserve(end);
            mSize = end;
        }

        Memory::Copy(mBuffer.GetData() + offset, buffer, static_cast<std::size_t>(length));
    }

    void MemoryFileContent::Resize(Uint64 size)
    {
        if (size == mSize)
            return;

        if (size > mSize)
        {
            Reserve(size);
            Memory::Set(mBuffer.GetData() + mSize, 0, static_cast<std::size_t>(size - mSize));
        }

        mSize = size;
    }

    void MemoryFileContent::Clear()
    {
        mBuffer.Resize(0);
        mSize = 0;
    }

    void MemoryFileContent::Reserve(Uint64 capacity)
    {
        if (capacity > MaxContentSize)
            throw std::system_error(std::make_error_code(std::errc::file_too_large));

        const Uint64 currentCapacity = mBuffer.GetSize();
        if (capacity <= currentCapacity)
            return;

        Uint64 newCapacity = currentCapacity > 0 ? currentCapacity : 1;
        while (newCapacity < capacity)
        {
            // Never overshoot the largest representable buffer size
            newCapacity = newCapacity > MaxContentSize / 2 ? MaxContentSize : newCapacity * 2;
        }

        mBuffer.Resize(static_cast<ByteArray::SizeType>(newCapacity));
    }
}
