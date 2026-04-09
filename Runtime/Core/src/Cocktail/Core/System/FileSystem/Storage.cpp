#include <Cocktail/Core/System/FileSystem/Storage.hpp>

namespace Ck
{
    bool Storage::IsFile(const URI& uri)
    {
        return ResolveFacadeInstance()->IsFile(uri);
    }

    bool Storage::IsDirectory(const URI& uri)
    {
        return ResolveFacadeInstance()->IsDirectory(uri);
    }

    void Storage::CreateFile(const URI& uri)
    {
        return ResolveFacadeInstance()->CreateFile(uri);
    }

    void Storage::CreateDirectory(const URI& uri)
    {
        return ResolveFacadeInstance()->CreateDirectory(uri);
    }

    UniquePtr<DirectoryIterator> Storage::CreateDirectoryIterator(const URI& uri)
    {
        return ResolveFacadeInstance()->CreateDirectoryIterator(uri);
    }

    UniquePtr<File> Storage::OpenFile(const URI& uri, FileOpenFlags flags)
    {
        return ResolveFacadeInstance()->OpenFile(uri, flags);
    }

    UniquePtr<Directory> Storage::OpenDirectory(const URI& uri)
    {
        return ResolveFacadeInstance()->OpenDirectory(uri);
    }

    void Storage::CopyFile(const URI& source, const URI& destination, const FileCopyOptions& options)
    {
        ResolveFacadeInstance()->CopyFile(source, destination, options);
    }

    void Storage::CopyDirectory(const URI& source, const URI& destination, const DirectoryCopyOptions& options)
    {
        ResolveFacadeInstance()->CopyDirectory(source, destination, options);
    }

    void Storage::MoveFile(const URI& source, const URI& destination, const FileMoveOptions& options)
    {
        ResolveFacadeInstance()->MoveFile(source, destination, options);
    }

    void Storage::MoveDirectory(const URI& source, const URI& destination, const DirectoryMoveOptions& options)
    {
        ResolveFacadeInstance()->MoveDirectory(source, destination, options);
    }

    void Storage::RemoveFile(const URI& uri, const FileRemoveOptions& options)
    {
        ResolveFacadeInstance()->RemoveFile(uri, options);
    }

    void Storage::RemoveDirectory(const URI& uri, const DirectoryRemoveOptions& options)
    {
        ResolveFacadeInstance()->RemoveDirectory(uri, options);
    }

    void Storage::Mount(String protocol, UniquePtr<FileSystemDriver> fileSystemDriver)
    {
        ResolveFacadeInstance()->Mount(std::move(protocol), std::move(fileSystemDriver));
    }

    void Storage::MountExternal(String protocol, FileSystemDriver* fileSystemDriver)
    {
        ResolveFacadeInstance()->MountExternal(std::move(protocol), fileSystemDriver);
    }

    void Storage::UnMount(const String& protocol)
    {
        ResolveFacadeInstance()->UnMount(protocol);
    }
    FileSystemDriver* Storage::ResolveDriver(const String& scheme)
    {
        return ResolveFacadeInstance()->ResolveDriver(scheme);
    }
}
