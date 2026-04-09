#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>
#include <Cocktail/Core/IO/Output/Stream/FileOutputStream.hpp>
#include <Cocktail/Core/System/FileSystem/StorageService.hpp>

namespace Ck
{
    StorageService::StorageService(String defaultScheme) :
        mDefaultScheme(std::move(defaultScheme))
    {
        assert(!mDefaultScheme.IsEmpty());
    }

    bool StorageService::IsFile(const URI& uri) const
    {
        return ResolveDriver(uri.GetScheme())->IsFile(uri.GetPath());
    }

    bool StorageService::IsDirectory(const URI& uri) const
    {
        return ResolveDriver(uri.GetScheme())->IsDirectory(uri.GetPath());
    }

    void StorageService::CreateFile(const URI& uri) const
    {
        return ResolveDriver(uri.GetScheme())->CreateFile(uri.GetPath());
    }

    void StorageService::CreateDirectory(const URI& uri) const
    {
        return ResolveDriver(uri.GetScheme())->CreateDirectory(uri.GetPath());
    }

    UniquePtr<DirectoryIterator> StorageService::CreateDirectoryIterator(const URI& uri)
    {
        return ResolveDriver(uri.GetScheme())->CreateDirectoryIterator(uri.GetPath());
    }

    UniquePtr<File> StorageService::OpenFile(const URI& uri, FileOpenFlags flags) const
    {
        return ResolveDriver(uri.GetScheme())->OpenFile(uri.GetPath(), flags);
    }

    UniquePtr<Directory> StorageService::OpenDirectory(const URI& uri) const
    {
        return ResolveDriver(uri.GetScheme())->OpenDirectory(uri.GetPath());
    }

    void StorageService::CopyFile(const URI& source, const URI& destination, const FileCopyOptions& options) const
    {
        FileSystemDriver* sourceDriver = ResolveDriver(source.GetScheme());
        FileSystemDriver* destinationDriver = ResolveDriver(destination.GetScheme());

        if (sourceDriver == destinationDriver)
        {
            sourceDriver->CopyFile(source.GetPath(), destination.GetPath(), options);
        }
        else
        {
            if (destinationDriver->IsFile(source.GetPath()) && !options.Overwrite)
                return;

            FileInputStream inputStream(source.GetPath(), sourceDriver);
            FileOutputStream outputStream(destination.GetPath(), destinationDriver);

            inputStream.TransferTo(outputStream);
        }
    }

    void StorageService::CopyDirectory(const URI& source, const URI& destination, const DirectoryCopyOptions& options) const
    {
        FileSystemDriver* sourceDriver = ResolveDriver(source.GetScheme());
        FileSystemDriver* destinationDriver = ResolveDriver(destination.GetScheme());

        if (sourceDriver == destinationDriver)
        {
            sourceDriver->CopyDirectory(source.GetPath(), destination.GetPath(), options);
        }
        else
        {
        }
    }

    void StorageService::MoveFile(const URI& source, const URI& destination, const FileMoveOptions& options) const
    {
        FileSystemDriver* sourceDriver = ResolveDriver(source.GetScheme());
        FileSystemDriver* destinationDriver = ResolveDriver(destination.GetScheme());

        if (sourceDriver == destinationDriver)
        {
            sourceDriver->MoveFile(source.GetPath(), destination.GetPath(), options);
        }
        else
        {
        }
    }

    void StorageService::MoveDirectory(const URI& source, const URI& destination, const DirectoryMoveOptions& options) const
    {
        FileSystemDriver* sourceDriver = ResolveDriver(source.GetScheme());
        FileSystemDriver* destinationDriver = ResolveDriver(destination.GetScheme());

        if (sourceDriver == destinationDriver)
        {
            sourceDriver->MoveDirectory(source.GetPath(), destination.GetPath(), options);
        }
        else
        {
        }
    }

    void StorageService::RemoveFile(const URI& uri, const FileRemoveOptions& options) const
    {
        ResolveDriver(uri.GetScheme())->RemoveFile(uri.GetPath(), options);
    }

    void StorageService::RemoveDirectory(const URI& uri, const DirectoryRemoveOptions& options) const
    {
        ResolveDriver(uri.GetScheme())->RemoveDirectory(uri.GetPath(), options);
    }

    void StorageService::Mount(String scheme, UniquePtr<FileSystemDriver> fileSystemDriver)
    {
        mDrivers.insert_or_assign(std::move(scheme), fileSystemDriver.Get());
        mInternalDrivers.Add(std::move(fileSystemDriver));
    }

    void StorageService::MountExternal(String scheme, FileSystemDriver* fileSystemDriver)
    {
        mDrivers.insert_or_assign(std::move(scheme), fileSystemDriver);
    }

    void StorageService::UnMount(const String& scheme)
    {
        mDrivers.erase(scheme);
    }

    const String& StorageService::GetDefaultScheme() const
    {
        return mDefaultScheme;
    }

    FileSystemDriver* StorageService::ResolveDriver(const String& scheme) const
    {
        if (scheme.IsEmpty())
            return ResolveDriver(mDefaultScheme);

        if (auto it = mDrivers.find(scheme); it != mDrivers.end())
            return it->second;

        return nullptr;
    }
}
