#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileSystemException.hpp>

#include <CocktailEngine/Main/Embedded/EmbdeddFile.hpp>

namespace Ck
{
    EmbeddedFile::EmbeddedFile(const cmrc::embedded_filesystem& fileSystem, const Path& path, const FileOpenFlags& flags) :
        mPath(path)
    {
        mPath.ToFormatInPlace(Path::Format::Generic);
        if (flags & FileOpenFlagBits::Truncate | flags & FileOpenFlagBits::Write)
            throw FileSystemException(FileSystemErrorCode::ReadOnlyFileSystem);

        Utf8String utf8Path = Utf8String::Convert(path.ToString());
        mHandle = fileSystem.open(reinterpret_cast<const char*>(utf8Path.GetData()));
        mCursor = flags & FileOpenFlagBits::Append ? mHandle.cend() : mHandle.cbegin();
    }

    unsigned EmbeddedFile::Read(void* buffer, unsigned length)
    {
        unsigned int copySize = std::min(static_cast<unsigned int>(mHandle.cend() - mCursor), length);

        Memory::Copy(buffer, mCursor, copySize);

        return copySize;
    }

    unsigned EmbeddedFile::Write(const void* buffer, unsigned length)
    {
        throw FileSystemException(FileSystemErrorCode::ReadOnlyFileSystem);
    }

    Uint64 EmbeddedFile::GetCursor() const
    {
        return mCursor - mHandle.cbegin();
    }

    Uint64 EmbeddedFile::SetCursor(FileCursorMode mode, Int64 offset)
    {
        switch (mode)
        {
            case FileCursorMode::Begin: mCursor = mHandle.cbegin() + offset; break;
            case FileCursorMode::Current: mCursor = mCursor + offset; break;
            case FileCursorMode::End: mCursor = mHandle.cend() - offset; break;
        }

        return GetCursor();
    }

    std::size_t EmbeddedFile::GetSize() const
    {
        return mHandle.size();
    }

    const Path& EmbeddedFile::GetPath() const
    {
        return mPath;
    }

    void EmbeddedFile::Flush() const
    {
        /// Nothing
    }

    void* EmbeddedFile::GetSystemHandle() const
    {
        return (void*) &mHandle;
    }
}
