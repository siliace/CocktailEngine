#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectory.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectoryIterator.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectoryWatcher.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFile.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileLock.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemDriver.hpp>
#include <Cocktail/Core/System/SystemError.hpp>

namespace Ck::Detail::Unix
{
    LocalFileSystemDriver::LocalFileSystemDriver(Path base) :
        mBase(std::move(base))
    {
        /// Nothing
    }

    bool LocalFileSystemDriver::IsFile(const Path& path) const
    {
        struct stat64 filestats{};
        Path fullPath = Path::Merge(mBase, path);
        if (stat64(reinterpret_cast<const AnsiChar*>(fullPath.ToString().GetData()), &filestats) == -1)
            return false;

        return S_ISREG(filestats.st_mode);
    }

    bool LocalFileSystemDriver::IsDirectory(const Path& path) const
    {
        struct stat64 filestats{};
        Path fullPath = Path::Merge(mBase, path);
        if (stat64(reinterpret_cast<const AnsiChar*>(fullPath.ToString().GetData()), &filestats) == -1)
            return false;

        return S_ISDIR(filestats.st_mode);
    }

    void LocalFileSystemDriver::CreateFile(const Path& path)
    {
        Path fullPath = Path::Merge(mBase, path);
        int handle = ::open64(reinterpret_cast<const AnsiChar*>(fullPath.ToString().GetData()), O_CREAT | O_TRUNC | O_EXCL, S_IRWXU);
        if (handle == -1)
            throw SystemError::GetLastError();

        close(handle);
    }

    void LocalFileSystemDriver::CreateDirectory(const Path& path)
    {
        Path fullPath = Path::Merge(mBase, path);
        int error = mkdir(reinterpret_cast<const AnsiChar*>(fullPath.ToString().GetData()), S_IRWXU | S_IRGRP | S_IROTH);
        if (error == -1)
            throw SystemError::GetLastError();
    }

    UniquePtr<File> LocalFileSystemDriver::OpenFile(const Path& path, const FileOpenFlags& flags)
    {
        return MakeUnique<LocalFile>(Path::Merge(mBase, path), flags);
    }

    UniquePtr<Directory> LocalFileSystemDriver::OpenDirectory(const Path& path)
    {
        return MakeUnique<LocalDirectory>(Path::Merge(mBase, path));
    }

    UniquePtr<DirectoryIterator> LocalFileSystemDriver::CreateDirectoryIterator(const Path& path)
    {
        return MakeUnique<LocalDirectoryIterator>(Path::Merge(mBase, path));
    }

    void LocalFileSystemDriver::CopyFile(const Path& source, const Path& destination, const FileCopyOptions& options)
    {
        Path fullSource = Path::Merge(mBase, source);
        Path fullDestination = Path::Merge(mBase, destination);

        int src = open(reinterpret_cast<const char*>(fullSource.ToString().GetData()), O_RDONLY);
        if (src < 0)
            throw SystemError::GetLastError();

        int flags = O_WRONLY | O_CREAT;
        flags |= options.Overwrite ? O_TRUNC : O_EXCL;
        int dst = open(reinterpret_cast<const char*>(fullDestination.ToString().GetData()), flags, 0644);
        if (dst < 0)
        {
            close(src);
            throw SystemError::GetLastError();
        }

        char buffer[8192];
        ssize_t bytes;

        while ((bytes = read(src, buffer, sizeof(buffer))) > 0)
        {
            if (write(dst, buffer, bytes) != bytes)
            {
                close(src);
                close(dst);
                throw SystemError::GetLastError();
            }
        }

        if (bytes < 0)
            throw SystemError::GetLastError();

        if (options.PreserveMetadata)
        {
            struct stat st{};
            if (fstat(src, &st) == 0)
            {
                fchmod(dst, st.st_mode);
                timespec times[2] = { st.st_atim, st.st_mtim };
                futimens(dst, times);
            }
        }

        close(src);
        close(dst);
    }

    void LocalFileSystemDriver::CopyDirectory(const Path& source, const Path& destination, const DirectoryCopyOptions& options)
    {
        Path fullSource = Path::Merge(mBase, source);
        Path fullDestination = Path::Merge(mBase, destination);

        if (mkdir(reinterpret_cast<const char*>(fullDestination.ToString().GetData()), 0755) < 0)
        {
            if (errno != EEXIST || !options.PreserveMetadata)
                throw SystemError::GetLastError();
        }

        DIR* dir = opendir(reinterpret_cast<const char*>(fullSource.ToString().GetData()));
        if (!dir)
            throw SystemError::GetLastError();

        dirent* entry;
        while ((entry = readdir(dir)) != nullptr)
        {
            StringView entryName = reinterpret_cast<const Utf8Char*>(entry->d_name);
            if (entryName == CK_TEXT(".") || entryName == CK_TEXT(".."))
                continue;

            Path childSource = Path::Merge(fullSource, entryName);
            Path childDestination = Path::Merge(fullDestination, entryName);

            struct stat st{};
            if (stat(reinterpret_cast<const char*>(childSource.ToString().GetData()), &st) < 0)
                continue;

            if (S_ISDIR(st.st_mode))
            {
                if (options.Recursive)
                    CopyDirectory(childSource, childDestination, options);
            }
            else
            {
                if (!options.OnlyStructure)
                    CopyFile(childSource, childDestination, options);
            }
        }

        closedir(dir);
    }

    void LocalFileSystemDriver::MoveFile(const Path& source, const Path& destination, const FileMoveOptions& options)
    {
        Path fullSource = Path::Merge(mBase, source);
        Path fullDestination = Path::Merge(mBase, destination);

        if (rename(reinterpret_cast<const char*>(fullSource.ToString().GetData()), reinterpret_cast<const char*>(fullDestination.ToString().GetData())) < 0)
        {
            if (!options.AllowCopyFallback)
                throw std::system_error(errno, std::generic_category());

            CopyFile(source, destination, options);
            RemoveFile(source, FileRemoveOptions{ true, false });
        }
    }

    void LocalFileSystemDriver::MoveDirectory(const Path& source, const Path& destination, const DirectoryMoveOptions& options)
    {
        Path fullSource = Path::Merge(mBase, source);
        Path fullDestination = Path::Merge(mBase, destination);

        if (rename(reinterpret_cast<const char*>(fullSource.ToString().GetData()), reinterpret_cast<const char*>(fullDestination.ToString().GetData())) < 0)
        {
            if (options.AllowCopyFallback && errno == EXDEV)
            {
                DirectoryCopyOptions copyOptions;
                copyOptions.Overwrite = options.Overwrite;
                copyOptions.PreserveMetadata = options.PreserveMetadata;
                copyOptions.Recursive = options.Recursive;
                copyOptions.OnlyStructure = options.OnlyStructure;

                CopyDirectory(source, destination, copyOptions);

                DirectoryRemoveOptions removeOptions;
                removeOptions.Recursive = true;
                removeOptions.Force = true;

                RemoveDirectory(source, removeOptions);
            }

            throw std::system_error(errno, std::generic_category());
        }
    }

    void LocalFileSystemDriver::RemoveFile(const Path& path, const FileRemoveOptions& options)
    {
        Path fullPath = Path::Merge(mBase, path);

        if (unlink(reinterpret_cast<const char*>(fullPath.ToString().GetData())) < 0)
        {
            if (errno == ENOENT && options.IgnoreMissing)
                return;

            throw std::system_error(errno, std::generic_category());
        }
    }

    void LocalFileSystemDriver::RemoveDirectory(const Path& path, const DirectoryRemoveOptions& options)
    {
        Path fullPath = Path::Merge(mBase, path);

        if (options.Recursive)
        {
            DIR* dir = opendir(reinterpret_cast<const char*>(fullPath.ToString().GetData()));
            if (dir)
            {
                dirent* entry;
                while ((entry = readdir(dir)) != nullptr)
                {
                    StringView entryName = reinterpret_cast<const Utf8Char*>(entry->d_name);
                    if (entryName == CK_TEXT(".") || entryName == CK_TEXT(".."))
                        continue;

                    Path child = Path::Merge(fullPath, entryName);

                    struct stat st{};
                    if (stat(reinterpret_cast<const char*>(child.ToString().GetData()), &st) < 0)
                        continue;

                    if (S_ISDIR(st.st_mode))
                    {
                        RemoveDirectory(child, options);
                    }
                    else
                    {
                        RemoveFile(child, options);
                    }
                }

                closedir(dir);
            }
        }

        if (rmdir(reinterpret_cast<const char*>(fullPath.ToString().GetData())) < 0)
        {
            if (errno == ENOENT && options.IgnoreMissing)
                return;

            throw std::system_error(errno, std::generic_category());
        }
    }

    UniquePtr<FileLock> LocalFileSystemDriver::CreateLock(File& file, std::size_t offset, std::size_t length)
    {
        return MakeUnique<LocalFileLock>(file, offset, length);
    }

    UniquePtr<DirectoryWatcher> LocalFileSystemDriver::CreateWatcher(Directory& directory, bool recursive)
    {
        return MakeUnique<LocalDirectoryWatcher>(directory, recursive);
    }

    Path LocalFileSystemDriver::MakeCanonical(const Path& path)
    {
        return TryMakeCanonical(path).GetOrThrow<std::system_error>(std::make_error_code(std::errc::no_such_file_or_directory));
    }

    Optional<Path> LocalFileSystemDriver::TryMakeCanonical(const Path& path)
    {
        Path fullPath = Path::Merge(mBase, path);
        Utf8Char* resolvedPath = reinterpret_cast<Utf8Char*>(realpath(reinterpret_cast<const AnsiChar*>(fullPath.ToString().GetData()), nullptr));
        if (!resolvedPath)
            return Optional<Path>::Empty();

        Path resolved = Path::Parse(resolvedPath);
        free(resolvedPath);

        return Optional<Path>::Of(resolvedPath);
    }

    PathInfo LocalFileSystemDriver::GetPathInfo(const Path& path) const
    {
        PathInfo pathInfo;
        Path fullPath = Path::Merge(mBase, path);
        
        struct statx stx{};
        Memory::Zero(stx);

        long int result = statx(AT_FDCWD, reinterpret_cast<const AnsiChar*>(fullPath.ToString().GetData()), AT_STATX_SYNC_AS_STAT, STATX_BASIC_STATS | STATX_BTIME, &stx);
        if (result != 0)
        {
            if (errno != ENOENT)
                throw std::system_error(errno, std::generic_category());

            return pathInfo;
        }

        pathInfo.LastAccessTime = Instant::EpochSeconds(stx.stx_atime.tv_sec, stx.stx_atime.tv_nsec);
        pathInfo.LastChangeTime = Instant::EpochSeconds(stx.stx_mtime.tv_sec, stx.stx_mtime.tv_nsec);

        if (stx.stx_mask & STATX_BTIME)
        {
            pathInfo.CreationTime = Instant::EpochSeconds(stx.stx_btime.tv_sec, stx.stx_btime.tv_nsec);
        }
        else
        {
            pathInfo.CreationTime = Instant::EpochSeconds(stx.stx_ctime.tv_sec, stx.stx_ctime.tv_nsec);
        }

        if (S_ISREG(stx.stx_mode))
        {
            pathInfo.Type = PathType::File;
            pathInfo.Size = stx.stx_size;
        }
        else if (S_ISDIR(stx.stx_mode))
        {
            pathInfo.Type = PathType::Directory;
        }
        else
        {
            pathInfo.Type = PathType::Other;
        }

        return pathInfo;
    }
}
