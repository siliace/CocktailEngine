#include <cstring>

#include <CocktailEngine/Core/System/FileSystem/FileSystemException.hpp>

#include <Ftp/FileSystemDriver/File.hpp>
#include <Ftp/FileSystemDriver/Transfer.hpp>

namespace Ck::Ftp
{
    File::File(const Client& client, Path path, const FileOpenFlags& flags, Array<Byte> content, bool modified) :
        mClient(&client),
        mPath(Move(path)),
        mFlags(flags),
        mContent(Move(content)),
        mCursor(0),
        mModified(modified)
    {
        if (mFlags & FileOpenFlagBits::Append)
            mCursor = mContent.GetSize();
    }

    File::~File()
    {
        // A destructor must not throw, call Flush explicitly to be notified of an upload failure
        try
        {
            Flush();
        }
        catch (...)
        {
            /// Nothing
        }
    }

    unsigned int File::Read(void* buffer, unsigned int length)
    {
        if (!(mFlags & FileOpenFlagBits::Read))
            throw FileSystemException(FileSystemErrorCode::PermissionDenied);

        if (mCursor >= mContent.GetSize() || length == 0)
            return 0;

        Uint64 available = mContent.GetSize() - mCursor;
        unsigned int count = available < length ? static_cast<unsigned int>(available) : length;

        std::memcpy(buffer, mContent.GetData() + mCursor, count);
        mCursor += count;

        return count;
    }

    unsigned int File::Write(const void* buffer, unsigned int length)
    {
        if (!(mFlags & FileOpenFlagBits::Write))
            throw FileSystemException(FileSystemErrorCode::PermissionDenied);

        // An appending handle always writes at the end, wherever the cursor was left
        if (mFlags & FileOpenFlagBits::Append)
            mCursor = mContent.GetSize();

        if (length == 0)
            return 0;

        Uint64 end = mCursor + length;
        if (end > mContent.GetSize())
            mContent.Resize(static_cast<Array<Byte>::SizeType>(end), Byte(0)); // A hole is filled with zeros

        std::memcpy(mContent.GetData() + mCursor, buffer, length);
        mCursor = end;
        mModified = true;

        return length;
    }

    Uint64 File::GetCursor() const
    {
        return mCursor;
    }

    Uint64 File::SetCursor(FileCursorMode mode, Int64 offset)
    {
        Int64 origin = 0;
        switch (mode)
        {
            case FileCursorMode::Begin: origin = 0; break;
            case FileCursorMode::Current: origin = static_cast<Int64>(mCursor); break;
            case FileCursorMode::End: origin = static_cast<Int64>(mContent.GetSize()); break;
        }

        Int64 cursor = origin + offset;
        if (cursor < 0)
            throw InvalidParameterException();

        // Seeking past the end is legal, the hole is only materialized on write
        mCursor = static_cast<Uint64>(cursor);

        return mCursor;
    }

    std::size_t File::GetSize() const
    {
        return mContent.GetSize();
    }

    const Path& File::GetPath() const
    {
        return mPath;
    }

    void File::Flush() const
    {
        if (!mModified)
            return;

        Detail::Upload(*mClient, mPath, mContent);
        mModified = false;
    }

    void* File::GetSystemHandle() const
    {
        return nullptr;
    }
}
