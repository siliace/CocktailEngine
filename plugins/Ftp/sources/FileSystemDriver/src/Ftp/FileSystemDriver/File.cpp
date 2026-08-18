#include <Ftp/FileSystemDriver/File.hpp>

namespace Ck::Ftp
{
    unsigned int File::Read(void* buffer, unsigned int length)
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    unsigned int File::Write(const void* buffer, unsigned int length)
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    Uint64 File::GetCursor() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    Uint64 File::SetCursor(FileCursorMode mode, Int64 offset)
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    std::size_t File::GetSize() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    const Path& File::GetPath() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    void File::Flush() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    void* File::GetSystemHandle() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }
}
