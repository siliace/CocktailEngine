#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectory.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectoryWatcher.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFile.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileLock.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemDriver.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

namespace Ck::Detail::Unix
{
	bool LocalFileSystemDriver::IsFile(const std::filesystem::path& path) const
	{
		struct stat64 filestats;
		if (stat64(path.c_str(), &filestats) == -1)
			return false;

		return S_ISREG(filestats.st_mode);
	}

	bool LocalFileSystemDriver::IsDirectory(const std::filesystem::path& path) const
	{
		struct stat64 filestats;
		if (stat64(path.c_str(), &filestats) == -1)
			return false;

		return S_ISDIR(filestats.st_mode);
	}

	void LocalFileSystemDriver::CreateFile(const std::filesystem::path& path)
	{
        int handle = ::open64(path.c_str(), O_CREAT | O_TRUNC | O_EXCL, S_IRWXU);
		if (handle != -1)
			throw SystemError::GetLastError();
		
		close(handle);
	}
	
	void LocalFileSystemDriver::CreateDirectory(const std::filesystem::path& path)
	{
		int error = mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IROTH);
		if (error == -1)
			throw SystemError::GetLastError();
	}

	std::unique_ptr<File> LocalFileSystemDriver::OpenFile(const std::filesystem::path& path, const FileOpenFlags& flags)
	{
		return std::make_unique<LocalFile>(mRootLocation / path, flags);
	}

	std::unique_ptr<Directory> LocalFileSystemDriver::OpenDirectory(const std::filesystem::path& path)
	{
		return std::make_unique<LocalDirectory>(mRootLocation / path);
	}

	void LocalFileSystemDriver::Copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool failIfExists)
	{
	}

	void LocalFileSystemDriver::Move(const std::filesystem::path& source, const std::filesystem::path& destination)
	{
		if (::rename(source.c_str(), destination.c_str()) == -1)
			throw SystemError::GetLastError();
	}

	void LocalFileSystemDriver::Remove(const std::filesystem::path& path)
	{
		if (::unlink(path.c_str()) == -1)
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
