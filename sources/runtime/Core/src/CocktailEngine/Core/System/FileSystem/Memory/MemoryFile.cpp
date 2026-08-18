#include <system_error>

#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFile.hpp>

namespace Ck::Detail
{
    MemoryFile::MemoryFile(SharedPtr<MemoryFileSystemNode> node, const FileOpenFlags& flags) :
        mNode(Move(node)),
        mFlags(flags),
        mCursor(0)
    {
        assert(mNode.Get() != nullptr);
        assert(mNode->Type == PathType::File);

        if (mFlags & FileOpenFlagBits::Append)
            mCursor = mNode->Content.GetSize();
    }

    unsigned int MemoryFile::Read(void* buffer, unsigned int length)
    {
        if (!(mFlags & FileOpenFlagBits::Read))
            throw std::system_error(std::make_error_code(std::errc::permission_denied));

        const Uint64 read = mNode->Content.Read(mCursor, buffer, length);

        mCursor += read;
        mNode->LastAccessTime = Instant::Now();

        return static_cast<unsigned int>(read);
    }

    unsigned int MemoryFile::Write(const void* buffer, unsigned int length)
    {
        if (!(mFlags & FileOpenFlagBits::Write))
            throw std::system_error(std::make_error_code(std::errc::permission_denied));

        // An appending handle always writes at the end, wherever the cursor was left
        if (mFlags & FileOpenFlagBits::Append)
            mCursor = mNode->Content.GetSize();

        mNode->Content.Write(mCursor, buffer, length);

        mCursor += length;

        const Instant now = Instant::Now();
        mNode->LastChangeTime = now;
        mNode->LastAccessTime = now;

        return length;
    }

    Uint64 MemoryFile::GetCursor() const
    {
        return mCursor;
    }

    Uint64 MemoryFile::SetCursor(FileCursorMode mode, Int64 offset)
    {
        Int64 origin = 0;
        switch (mode)
        {
        case FileCursorMode::Begin:
            origin = 0;
            break;

        case FileCursorMode::Current:
            origin = static_cast<Int64>(mCursor);
            break;

        case FileCursorMode::End:
            origin = static_cast<Int64>(mNode->Content.GetSize());
            break;
        }

        const Int64 cursor = origin + offset;
        if (cursor < 0)
            throw std::system_error(std::make_error_code(std::errc::invalid_argument));

        // Seeking past the end is legal, the hole is only materialized on write
        mCursor = static_cast<Uint64>(cursor);

        return mCursor;
    }

    std::size_t MemoryFile::GetSize() const
    {
        return static_cast<std::size_t>(mNode->Content.GetSize());
    }

    const Path& MemoryFile::GetPath() const
    {
        return mNode->NodePath;
    }

    void MemoryFile::Flush() const
    {
        /// Nothing, the content already is the storage medium
    }

    void* MemoryFile::GetSystemHandle() const
    {
        return nullptr;
    }
}
