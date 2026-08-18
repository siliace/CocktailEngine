#include <system_error>

#include <CocktailEngine/Core/IO/Input/Stream/FileInputStream.hpp>
#include <CocktailEngine/Core/IO/Output/Stream/FileOutputStream.hpp>
#include <CocktailEngine/Core/System/FileSystem/StorageService.hpp>
#include <CocktailEngine/Core/Utility/FileUtils.hpp>

namespace Ck
{
    namespace
    {
        void EnsureIsFile(const FileSystemDriver& driver, const Path& path)
        {
            switch (driver.GetPathInfo(path).Type)
            {
                case PathType::File: return;
                case PathType::None: throw NoSuchFileOrDirectoryException();
                case PathType::Directory: throw DirectoryAlreadyExistsException();
                case PathType::Other: throw OperationNotSupportedException();
            }
        }

        void EnsureIsDirectory(const FileSystemDriver& driver, const Path& path)
        {
            switch (driver.GetPathInfo(path).Type)
            {
                case PathType::Directory: return;
                case PathType::None: throw NoSuchFileOrDirectoryException();
                case PathType::File: throw NotADirectoryException();
                case PathType::Other: throw OperationNotSupportedException();
            }
        }

        void ValidateFileDestination(const FileSystemDriver& driver, const Path& path, bool overwrite)
        {
            switch (driver.GetPathInfo(path).Type)
            {
                case PathType::None: return;
                case PathType::File:
                    if (!overwrite)
                        throw FileAlreadyExistsException();

                    return;
                case PathType::Directory: throw DirectoryAlreadyExistsException();
                case PathType::Other: throw OperationNotSupportedException();
            }
        }

        void ValidateDirectoryDestination(FileSystemDriver& driver, const Path& path)
        {
            PathInfo pathInfo = driver.GetPathInfo(path);
            if (pathInfo.Type != PathType::Directory)
            {
                if (pathInfo.Type != PathType::None)
                    throw NotADirectoryException();

                FileUtils::MakeDirectories(path, driver);
            }
        }

        void ValidateDirectoryMoveDestination(FileSystemDriver& driver, const Path& path, bool overwrite)
        {
            switch (driver.GetPathInfo(path).Type)
            {
                case PathType::None: return;
                case PathType::File: throw NotADirectoryException();
                case PathType::Other: throw OperationNotSupportedException();
                case PathType::Directory: break;
            }

            if (!overwrite)
                throw FileAlreadyExistsException();

            UniquePtr<DirectoryIterator> iterator = driver.CreateDirectoryIterator(path);
            if (iterator == nullptr)
                throw OperationNotSupportedException();

            if (!iterator->IsEnd())
                throw DirectoryNotEmptyException();
        }

        void CopyFileAcrossDrivers(FileSystemDriver& sourceDriver, const Path& source, FileSystemDriver& destinationDriver, const Path& destination, const FileCopyOptions& options)
        {
            EnsureIsFile(sourceDriver, source);
            ValidateFileDestination(destinationDriver, destination, options.Overwrite);

            FileInputStream inputStream(source, sourceDriver);
            FileOutputStream outputStream(destination, true, destinationDriver);
            inputStream.TransferTo(outputStream);
        }

        void CopyDirectoryAcrossDrivers(FileSystemDriver& sourceDriver, const Path& source, FileSystemDriver& destinationDriver, const Path& destination,
                                        const DirectoryCopyOptions& options)
        {
            EnsureIsDirectory(sourceDriver, source);
            ValidateDirectoryDestination(destinationDriver, destination);

            UniquePtr<DirectoryIterator> iterator = sourceDriver.CreateDirectoryIterator(source);
            if (iterator == nullptr)
                throw OperationNotSupportedException();

            while (!iterator->IsEnd())
            {
                const Path name = iterator->GetPath().GetFilename();
                if (name.IsEmpty())
                    throw OperationNotSupportedException();

                const Path childSource = Path::Merge(source, name);
                const Path childDestination = Path::Merge(destination, name);

                switch (iterator->GetPathInfo().Type)
                {
                    case PathType::File:
                        if (!options.OnlyStructure)
                            CopyFileAcrossDrivers(sourceDriver, childSource, destinationDriver, childDestination, options);
                        break;
                    case PathType::Directory:
                        if (options.Recursive)
                            CopyDirectoryAcrossDrivers(sourceDriver, childSource, destinationDriver, childDestination, options);
                        break;
                    case PathType::None:
                    case PathType::Other: throw OperationNotSupportedException();
                }

                iterator->Next();
            }
        }
    }

    StorageService::StorageService(String defaultScheme) :
        mDefaultScheme(Move(defaultScheme))
    {
        assert(!mDefaultScheme.IsEmpty());
    }

    bool StorageService::IsFile(const URI& uri)
    {
        return ResolveDriver(uri).IsFile(uri.GetPath());
    }

    bool StorageService::IsDirectory(const URI& uri)
    {
        return ResolveDriver(uri).IsDirectory(uri.GetPath());
    }

    void StorageService::CreateFile(const URI& uri)
    {
        ResolveDriver(uri).CreateFile(uri.GetPath());
    }

    void StorageService::CreateDirectory(const URI& uri)
    {
        ResolveDriver(uri).CreateDirectory(uri.GetPath());
    }

    UniquePtr<DirectoryIterator> StorageService::CreateDirectoryIterator(const URI& uri)
    {
        return ResolveDriver(uri).CreateDirectoryIterator(uri.GetPath());
    }

    UniquePtr<File> StorageService::OpenFile(const URI& uri, FileOpenFlags flags)
    {
        return ResolveDriver(uri).OpenFile(uri.GetPath(), flags);
    }

    UniquePtr<Directory> StorageService::OpenDirectory(const URI& uri)
    {
        return ResolveDriver(uri).OpenDirectory(uri.GetPath());
    }

    void StorageService::CopyFile(const URI& source, const URI& destination, const FileCopyOptions& options)
    {
        FileSystemDriver& sourceDriver = ResolveDriver(source);
        FileSystemDriver& destinationDriver = ResolveDriver(destination);

        if (&sourceDriver == &destinationDriver)
            sourceDriver.CopyFile(source.GetPath(), destination.GetPath(), options);
        else
            CopyFileAcrossDrivers(sourceDriver, source.GetPath(), destinationDriver, destination.GetPath(), options);
    }

    void StorageService::CopyDirectory(const URI& source, const URI& destination, const DirectoryCopyOptions& options)
    {
        FileSystemDriver& sourceDriver = ResolveDriver(source);
        FileSystemDriver& destinationDriver = ResolveDriver(destination);

        if (&sourceDriver == &destinationDriver)
            sourceDriver.CopyDirectory(source.GetPath(), destination.GetPath(), options);
        else
            CopyDirectoryAcrossDrivers(sourceDriver, source.GetPath(), destinationDriver, destination.GetPath(), options);
    }

    void StorageService::MoveFile(const URI& source, const URI& destination, const FileMoveOptions& options)
    {
        FileSystemDriver& sourceDriver = ResolveDriver(source);
        FileSystemDriver& destinationDriver = ResolveDriver(destination);

        if (&sourceDriver == &destinationDriver)
        {
            sourceDriver.MoveFile(source.GetPath(), destination.GetPath(), options);
            return;
        }

        if (!options.AllowCopyFallback)
            throw CrossDeviceLinkException();

        CopyFileAcrossDrivers(sourceDriver, source.GetPath(), destinationDriver, destination.GetPath(), options);
        sourceDriver.RemoveFile(source.GetPath(), FileRemoveOptions{ true, false });
    }

    void StorageService::MoveDirectory(const URI& source, const URI& destination, const DirectoryMoveOptions& options)
    {
        FileSystemDriver& sourceDriver = ResolveDriver(source);
        FileSystemDriver& destinationDriver = ResolveDriver(destination);

        if (&sourceDriver == &destinationDriver)
        {
            sourceDriver.MoveDirectory(source.GetPath(), destination.GetPath(), options);
            return;
        }

        if (!options.AllowCopyFallback)
            throw CrossDeviceLinkException();

        if (!options.Recursive || options.OnlyStructure)
            throw InvalidParameterException(CK_TEXT("Cannot move directory from non-recursive path"));

        if (source.GetPath().IsEmpty())
            throw InvalidParameterException(CK_TEXT("Cannot move the root directory"));

        EnsureIsDirectory(sourceDriver, source.GetPath());
        ValidateDirectoryMoveDestination(destinationDriver, destination.GetPath(), options.Overwrite);

        DirectoryCopyOptions copyOptions;
        copyOptions.Overwrite = options.Overwrite;
        copyOptions.PreserveMetadata = options.PreserveMetadata;
        copyOptions.Recursive = true;
        copyOptions.OnlyStructure = false;
        CopyDirectoryAcrossDrivers(sourceDriver, source.GetPath(), destinationDriver, destination.GetPath(), copyOptions);

        DirectoryRemoveOptions removeOptions;
        removeOptions.Force = true;
        removeOptions.IgnoreMissing = false;
        removeOptions.Recursive = true;
        sourceDriver.RemoveDirectory(source.GetPath(), removeOptions);
    }

    void StorageService::RemoveFile(const URI& uri, const FileRemoveOptions& options)
    {
        ResolveDriver(uri).RemoveFile(uri.GetPath(), options);
    }

    void StorageService::RemoveDirectory(const URI& uri, const DirectoryRemoveOptions& options)
    {
        ResolveDriver(uri).RemoveDirectory(uri.GetPath(), options);
    }

    void StorageService::Mount(String scheme, UniquePtr<FileSystemDriver> fileSystemDriver)
    {
        mDrivers.Put(Move(scheme), fileSystemDriver.Get());
        mInternalDrivers.Add(Move(fileSystemDriver));
    }

    void StorageService::MountExternal(String scheme, FileSystemDriver* fileSystemDriver)
    {
        mDrivers.Put(Move(scheme), fileSystemDriver);
    }

    void StorageService::UnMount(const String& scheme)
    {
        mDrivers.Remove(scheme);
    }

    const String& StorageService::GetDefaultScheme() const
    {
        return mDefaultScheme;
    }

    FileSystemDriver& StorageService::ResolveDriver(const URI& uri)
    {
        return TryResolveDriver(uri).GetOrThrow<NoSuchDriverException>();
    }

    const FileSystemDriver& StorageService::ResolveDriver(const URI& uri) const
    {
        return const_cast<StorageService*>(this)->ResolveDriver(uri);
    }

    Optional<FileSystemDriver&> StorageService::TryResolveDriver(const URI& uri)
    {
        String scheme = uri.GetScheme();
        if (scheme.IsEmpty())
            scheme = mDefaultScheme;

        return mDrivers.TryGet(scheme).Map([](FileSystemDriver* driver) -> FileSystemDriver& {
            return *driver;
        });
    }

    Optional<const FileSystemDriver&> StorageService::TryResolveDriver(const URI& uri) const
    {
        return const_cast<StorageService*>(this)->TryResolveDriver(uri);
    }

    FileSystemDriver& StorageService::GetDriver(const String& scheme)
    {
        return TryGetDriver(scheme).GetOrThrow<NoSuchDriverException>();
    }

    const FileSystemDriver& StorageService::GetDriver(const String& scheme) const
    {
        return const_cast<StorageService*>(this)->GetDriver(scheme);
    }

    Optional<FileSystemDriver&> StorageService::TryGetDriver(const String& scheme)
    {
        return mDrivers.TryGet(scheme).Map([](FileSystemDriver* driver) -> FileSystemDriver& {
            return *driver;
        });
    }

    Optional<const FileSystemDriver&> StorageService::TryGetDriver(const String& scheme) const
    {
        return const_cast<StorageService*>(this)->TryGetDriver(scheme);
    }
}
