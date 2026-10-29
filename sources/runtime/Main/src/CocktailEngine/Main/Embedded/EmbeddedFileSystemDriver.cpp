#include <CocktailEngine/Core/System/FileSystem/FileSystemException.hpp>

#include <CocktailEngine/Main/Embedded/EmbdeddDirectory.hpp>
#include <CocktailEngine/Main/Embedded/EmbdeddDirectoryIterator.hpp>
#include <CocktailEngine/Main/Embedded/EmbdeddFile.hpp>
#include <CocktailEngine/Main/Embedded/EmbeddedFileSystemDriver.hpp>

namespace Ck
{
    EmbeddedFileSystemDriver::EmbeddedFileSystemDriver(cmrc::embedded_filesystem fileSystem) :
        mFileSystem(Move(fileSystem))
    {
        /// Nothing
    }

    bool EmbeddedFileSystemDriver::IsFile(const Path& path) const
    {
        AsciiString p = AsciiString::Convert(path.ToFormat(Path::Format::Generic).ToString());
        return mFileSystem.is_file(p.GetData());
    }

    bool EmbeddedFileSystemDriver::IsDirectory(const Path& path) const
    {
        AsciiString p = AsciiString::Convert(path.ToFormat(Path::Format::Generic).ToString());
        return mFileSystem.is_directory(p.GetData());
    }

    void EmbeddedFileSystemDriver::CreateFile(const Path& path)
    {
        throw FileSystemException(FileSystemErrorCode::ReadOnlyFileSystem);
    }

    void EmbeddedFileSystemDriver::CreateDirectory(const Path& path)
    {
        throw FileSystemException(FileSystemErrorCode::ReadOnlyFileSystem);
    }

    UniquePtr<File> EmbeddedFileSystemDriver::OpenFile(const Path& path, const FileOpenFlags& flags)
    {
        AsciiString p = AsciiString::Convert(path.ToFormat(Path::Format::Generic).ToString());
        if (!mFileSystem.is_file(p.GetData()))
            throw FileSystemException(FileSystemErrorCode::NoSuchFileOrDirectory);

        return MakeUnique<EmbeddedFile>(mFileSystem, path, flags);
    }

    UniquePtr<Directory> EmbeddedFileSystemDriver::OpenDirectory(const Path& path)
    {
        AsciiString p = AsciiString::Convert(path.ToFormat(Path::Format::Generic).ToString());
        if (!mFileSystem.is_directory(p.GetData()))
            throw FileSystemException(FileSystemErrorCode::NoSuchFileOrDirectory);

        return MakeUnique<EmbeddedDirectory>(mFileSystem, path);
    }

    UniquePtr<DirectoryIterator> EmbeddedFileSystemDriver::CreateDirectoryIterator(const Path& path)
    {
        AsciiString p = AsciiString::Convert(path.ToFormat(Path::Format::Generic).ToString());
        if (!mFileSystem.is_directory(p.GetData()))
            throw FileSystemException(FileSystemErrorCode::NoSuchFileOrDirectory);

        return MakeUnique<EmbeddedDirectoryIterator>(mFileSystem, path);
    }

    void EmbeddedFileSystemDriver::CopyFile(const Path& source, const Path& destination, const FileCopyOptions& options)
    {
        throw FileSystemException(FileSystemErrorCode::ReadOnlyFileSystem);
    }

    void EmbeddedFileSystemDriver::CopyDirectory(const Path& source, const Path& destination, const DirectoryCopyOptions& options)
    {
        throw FileSystemException(FileSystemErrorCode::ReadOnlyFileSystem);
    }

    void EmbeddedFileSystemDriver::MoveFile(const Path& source, const Path& destination, const FileMoveOptions& options)
    {
        throw FileSystemException(FileSystemErrorCode::ReadOnlyFileSystem);
    }

    void EmbeddedFileSystemDriver::MoveDirectory(const Path& source, const Path& destination, const DirectoryMoveOptions& options)
    {
        throw FileSystemException(FileSystemErrorCode::ReadOnlyFileSystem);
    }

    void EmbeddedFileSystemDriver::RemoveFile(const Path& path, const FileRemoveOptions& options)
    {
        throw FileSystemException(FileSystemErrorCode::ReadOnlyFileSystem);
    }

    void EmbeddedFileSystemDriver::RemoveDirectory(const Path& path, const DirectoryRemoveOptions& options)
    {
        throw FileSystemException(FileSystemErrorCode::ReadOnlyFileSystem);
    }

    Path EmbeddedFileSystemDriver::MakeCanonical(const Path& path)
    {
        return path;
    }

    Optional<Path> EmbeddedFileSystemDriver::TryMakeCanonical(const Path& path)
    {
        return Optional<Path>::Of(path);
    }

    PathInfo EmbeddedFileSystemDriver::GetPathInfo(const Path& path) const
    {
        AsciiString p = AsciiString::Convert(path.ToFormat(Path::Format::Generic).ToString());
        if (!mFileSystem.exists(p.GetData()))
            return {};

        PathInfo pathInfo;
        if (mFileSystem.is_file(p.GetData()))
        {
            pathInfo.Type = PathType::File;
            pathInfo.Size = mFileSystem.open(p.GetData()).size();
        }
        else if (mFileSystem.is_directory(p.GetData()))
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
