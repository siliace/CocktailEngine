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
	bool LocalFileSystemDriver::IsFile(const Path& path) const
	{
		struct stat64 filestats;
		if (stat64(CK_TEXT_TO_ANSI(path.ToString().GetData()), &filestats) == -1)
			return false;

		return S_ISREG(filestats.st_mode);
	}

	bool LocalFileSystemDriver::IsDirectory(const Path& path) const
	{
		struct stat64 filestats;
		if (stat64(CK_TEXT_TO_ANSI(path.ToString().GetData()), &filestats) == -1)
			return false;

		return S_ISDIR(filestats.st_mode);
	}

	void LocalFileSystemDriver::CreateFile(const Path& path)
	{
        int handle = ::open64(CK_TEXT_TO_ANSI(path.ToString().GetData()), O_CREAT | O_TRUNC | O_EXCL, S_IRWXU);
		if (handle == -1)
			throw SystemError::GetLastError();
		
		close(handle);
	}
	
	void LocalFileSystemDriver::CreateDirectory(const Path& path)
	{
		int error = mkdir(CK_TEXT_TO_ANSI(path.ToString().GetData()), S_IRWXU | S_IRGRP | S_IROTH);
		if (error == -1)
			throw SystemError::GetLastError();
	}

	std::unique_ptr<File> LocalFileSystemDriver::OpenFile(const Path& path, const FileOpenFlags& flags)
	{
		return std::make_unique<LocalFile>(path, flags);
	}

	std::unique_ptr<Directory> LocalFileSystemDriver::OpenDirectory(const Path& path)
	{
		return std::make_unique<LocalDirectory>(path);
	}

	void LocalFileSystemDriver::Copy(const Path& source, const Path& destination, bool failIfExists)
	{
	}

	void LocalFileSystemDriver::Move(const Path& source, const Path& destination)
	{
		if (::rename(CK_TEXT_TO_ANSI(source.ToString().GetData()), CK_TEXT_TO_ANSI(destination.ToString().GetData())) == -1)
			throw SystemError::GetLastError();
	}

	void LocalFileSystemDriver::Remove(const Path& path)
	{
		if (::unlink(CK_TEXT_TO_ANSI(path.ToString().GetData())) == -1)
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
}
