#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddDirectory.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddDirectoryIterator.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddFile.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>

namespace Ck
{
	bool EmbeddedFileSystemDriver::IsFile(const Path& path) const
	{
		AsciiString p = AsciiString::Convert(path.ToFormat(Path::Format::Generic).ToString());
		for (const cmrc::embedded_filesystem& fileSystem : mFileSystems)
		{
			if (fileSystem.is_file(p.GetData()))
				return true;
		}

		return false;
	}

	bool EmbeddedFileSystemDriver::IsDirectory(const Path& path) const
	{
		AsciiString p = AsciiString::Convert(path.ToFormat(Path::Format::Generic).ToString());
		for (const cmrc::embedded_filesystem& fileSystem : mFileSystems)
		{
			if (fileSystem.is_directory(p.GetData()))
				return true;
		}

		return false;
	}

	void EmbeddedFileSystemDriver::CreateFile(const Path& path)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	void EmbeddedFileSystemDriver::CreateDirectory(const Path& path)
	{
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
	}

	UniquePtr<File> EmbeddedFileSystemDriver::OpenFile(const Path& path, const FileOpenFlags& flags)
	{
		AsciiString p = AsciiString::Convert(path.ToFormat(Path::Format::Generic).ToString());
		for (const cmrc::embedded_filesystem& fileSystem : mFileSystems)
		{
			if (fileSystem.is_file(p.GetData()))
				return MakeUnique<EmbeddedFile>(fileSystem, path, flags);
		}

		throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
	}

	UniquePtr<Directory> EmbeddedFileSystemDriver::OpenDirectory(const Path& path)
	{
		AsciiString p = AsciiString::Convert(path.ToFormat(Path::Format::Generic).ToString());
		for (const cmrc::embedded_filesystem& mFileSystem : mFileSystems)
		{
			if (mFileSystem.is_directory(p.GetData()))
				return MakeUnique<EmbeddedDirectory>(mFileSystem, path);
		}

		throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));
	}

    UniquePtr<DirectoryIterator> EmbeddedFileSystemDriver::CreateDirectoryIterator(const Path& path)
    {
	    AsciiString p = AsciiString::Convert(path.ToFormat(Path::Format::Generic).ToString());
	    for (const cmrc::embedded_filesystem& mFileSystem : mFileSystems)
	    {
	        if (mFileSystem.is_directory(p.GetData()))
	            return MakeUnique<EmbeddedDirectoryIterator>(mFileSystem, path);
	    }

	    return nullptr;
    }

    void EmbeddedFileSystemDriver::CopyFile(const Path& source, const Path& destination, const FileCopyOptions& options)
    {
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
    }

    void EmbeddedFileSystemDriver::CopyDirectory(const Path& source, const Path& destination, const DirectoryCopyOptions& options)
    {
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
    }

    void EmbeddedFileSystemDriver::MoveFile(const Path& source, const Path& destination, const FileMoveOptions& options)
    {
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
    }

    void EmbeddedFileSystemDriver::MoveDirectory(const Path& source, const Path& destination, const DirectoryMoveOptions& options)
    {
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
    }

    void EmbeddedFileSystemDriver::RemoveFile(const Path& path, const FileRemoveOptions& options)
    {
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
    }

    void EmbeddedFileSystemDriver::RemoveDirectory(const Path& path, const DirectoryRemoveOptions& options)
    {
		throw std::system_error(std::make_error_code(std::errc::read_only_file_system));
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
	    return mFileSystems.FindIf([&](const cmrc::embedded_filesystem& filesystem) {
	        return filesystem.exists(p.GetData());
	    }).Map([&](const cmrc::embedded_filesystem& filesystem) {
	        PathInfo pathInfo;
	        if (filesystem.is_file(p.GetData()))
	        {
	            pathInfo.Type = PathType::File;
	            pathInfo.Size = filesystem.open(p.GetData()).size();
	        }
	        else if (filesystem.is_directory(p.GetData()))
	        {
	            pathInfo.Type = PathType::Directory;
	        }
	        else
	        {
	            pathInfo.Type = PathType::Other;
	        }

	        return pathInfo;
	    }).GetOr(PathInfo{});
    }

    void EmbeddedFileSystemDriver::Register(cmrc::embedded_filesystem fileSystem)
	{
		mFileSystems.Add(std::move(fileSystem));
	}
}
