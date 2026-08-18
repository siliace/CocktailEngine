#include <Ftp/FileSystemDriver/Directory.hpp>

namespace Ck::Ftp
{
    Array<Path> Directory::GetContent() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    const Path& Directory::GetPath() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    void* Directory::GetSystemHandle() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }
}
