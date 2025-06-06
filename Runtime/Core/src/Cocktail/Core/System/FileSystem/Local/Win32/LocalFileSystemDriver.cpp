#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectory.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectoryWatcher.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFile.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileLock.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemDriver.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

namespace Ck::Detail::Win32
{
	bool LocalFileSystemDriver::IsFile(const std::filesystem::path& path) const
	{
		DWORD attribtutes = GetFileAttributesW(path.c_str());
		if (attribtutes == INVALID_FILE_ATTRIBUTES)
			return false;

		return (attribtutes & FILE_ATTRIBUTE_DIRECTORY) == 0;
	}

	bool LocalFileSystemDriver::IsDirectory(const std::filesystem::path& path) const
	{
		DWORD attribtutes = GetFileAttributesW(path.c_str());
		if (attribtutes == INVALID_FILE_ATTRIBUTES)
			return false;

		return attribtutes == FILE_ATTRIBUTE_DIRECTORY;
	}

	void LocalFileSystemDriver::CreateFile(const std::filesystem::path& path)
	{
		HANDLE handle = CreateFileW(
			path.c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (handle == INVALID_HANDLE_VALUE)
			throw SystemError::GetLastError();

		CloseHandle(handle);
	}

	void LocalFileSystemDriver::CreateDirectory(const std::filesystem::path& path)
	{
		if (CreateDirectoryW(path.c_str(), nullptr) == FALSE)
			throw SystemError::GetLastError();
	}

	std::shared_ptr<File> LocalFileSystemDriver::OpenFile(const std::filesystem::path& path, const FileOpenFlags& flags)
	{
		return std::make_shared<LocalFile>(path, flags);
	}

	std::shared_ptr<Directory> LocalFileSystemDriver::OpenDirectory(const std::filesystem::path& path)
	{
		return std::make_shared<LocalDirectory>(path);
	}

	void LocalFileSystemDriver::Copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool failIfExists)
	{
		if (CopyFileW(source.c_str(), destination.c_str(), failIfExists) == FALSE)
			throw SystemError::GetLastError();
	}

	void LocalFileSystemDriver::Move(const std::filesystem::path& source, const std::filesystem::path& destination)
	{
		if (MoveFileW(source.c_str(), destination.c_str()) == FALSE)
			throw SystemError::GetLastError();
	}

	void LocalFileSystemDriver::Remove(const std::filesystem::path& path)
	{
		std::filesystem::path fullPath = path;

		if (IsFile(fullPath))
		{
			if (DeleteFileW(fullPath.c_str()) == FALSE)
				throw SystemError::GetLastError();
		}
		else if (IsDirectory(fullPath))
		{
			if (RemoveDirectoryW(fullPath.c_str()) == FALSE)
				throw SystemError::GetLastError();
		}
	}

	std::shared_ptr<FileLock> LocalFileSystemDriver::CreateLock(const std::shared_ptr<File>& file, std::size_t offset, std::size_t length)
	{
		return std::make_shared<LocalFileLock>(file, offset, length);
	}

	std::shared_ptr<DirectoryWatcher> LocalFileSystemDriver::CreateWatcher(const std::shared_ptr<Directory>& directory, bool recursive)
	{
		return std::make_shared<LocalDirectoryWatcher>(directory, recursive);
	}
}
