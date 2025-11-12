#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectory.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectoryWatcher.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFile.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileLock.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemDriver.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

namespace Ck::Detail::Unix
{
	LocalFileSystemDriver::LocalFileSystemDriver(Path base) :
		mBase(std::move(base))
	{
		/// Nothing
	}

	bool LocalFileSystemDriver::IsFile(const Path& path) const
	{
		struct stat64 filestats;
		Path fullPath = Path::Merge(mBase, path);
		if (stat64(reinterpret_cast<const AnsiChar*>(fullPath.ToString().GetData()), &filestats) == -1)
			return false;

		return S_ISREG(filestats.st_mode);
	}

	bool LocalFileSystemDriver::IsDirectory(const Path& path) const
	{
		struct stat64 filestats;
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

	std::unique_ptr<File> LocalFileSystemDriver::OpenFile(const Path& path, const FileOpenFlags& flags)
	{
		return std::make_unique<LocalFile>(Path::Merge(mBase, path), flags);
	}

	std::unique_ptr<Directory> LocalFileSystemDriver::OpenDirectory(const Path& path)
	{
		return std::make_unique<LocalDirectory>(Path::Merge(mBase, path));
	}

	void LocalFileSystemDriver::Copy(const Path& source, const Path& destination, bool failIfExists)
	{
	}

	void LocalFileSystemDriver::Move(const Path& source, const Path& destination)
	{
		auto old = Path::Merge(mBase, source).ToString();
		auto next = Path::Merge(mBase, destination).ToString();

		if (::rename(reinterpret_cast<const AnsiChar*>(old.GetData()), reinterpret_cast<const AnsiChar*>(next.GetData())) == -1)
			throw SystemError::GetLastError();
	}

	void LocalFileSystemDriver::Remove(const Path& path)
	{
		Path fullPath = Path::Merge(mBase, path);
		if (::unlink(reinterpret_cast<const AnsiChar*>(fullPath.ToString().GetData())) == -1)
			throw SystemError::GetLastError();
	}

	std::unique_ptr<FileLock> LocalFileSystemDriver::CreateLock(File& file, std::size_t offset, std::size_t length)
	{
		return std::make_unique<LocalFileLock>(file, offset, length);
	}

	std::unique_ptr<DirectoryWatcher> LocalFileSystemDriver::CreateWatcher(Directory& directory, bool recursive)
	{
		return std::make_unique<LocalDirectoryWatcher>(directory, recursive);
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
}
