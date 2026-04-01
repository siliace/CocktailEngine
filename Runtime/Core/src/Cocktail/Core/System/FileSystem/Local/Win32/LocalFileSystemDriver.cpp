#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectory.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectoryWatcher.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFile.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileLock.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemDriver.hpp>

namespace Ck::Detail::Win32
{
    namespace
    {
        Instant FiletimeToInstant(const FILETIME& fileTime)
        {
            // Number of 100ns periods between 1st january 1601 and 1st january 1970
            static constexpr Uint64 DurationToEpoch = 116444736000000000ULL;

            LARGE_INTEGER time;
            time.HighPart = fileTime.dwHighDateTime;
            time.LowPart = fileTime.dwLowDateTime;

            // Number of 100ns periods since 1st january 1601
            Uint64 fileTime100ns = time.QuadPart;

            // Number of 100ns periods since 1st january 1970
            Uint64 epoch100ns = fileTime100ns - DurationToEpoch;

            Uint64 seconds = epoch100ns / 10000000ULL;
            Uint32 nanoseconds = static_cast<Uint32>(epoch100ns % 10000000ULL * 100ULL);

            return Instant::EpochSeconds(seconds, nanoseconds);
        }
    }

	LocalFileSystemDriver::LocalFileSystemDriver(Path base) :
		mBase(std::move(base))
	{
		/// Nothing
	}

	bool LocalFileSystemDriver::IsFile(const Path& path) const
	{
		Path fullPath = Path::Merge(mBase, path);
		DWORD attribtutes = GetFileAttributes(fullPath.ToString().GetData());
		if (attribtutes == INVALID_FILE_ATTRIBUTES)
			return false;

		return (attribtutes & FILE_ATTRIBUTE_DIRECTORY) == 0;
	}

	bool LocalFileSystemDriver::IsDirectory(const Path& path) const
	{
		Path fullPath = Path::Merge(mBase, path);
		DWORD attribtutes = GetFileAttributes(fullPath.ToString().GetData());
		if (attribtutes == INVALID_FILE_ATTRIBUTES)
			return false;

		return attribtutes == FILE_ATTRIBUTE_DIRECTORY;
	}

	void LocalFileSystemDriver::CreateFile(const Path& path)
	{
		Path fullPath = Path::Merge(mBase, path);
		HANDLE handle = ::CreateFileW(
			fullPath.ToString().GetData(),
			GENERIC_WRITE,
			0,
			nullptr,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (handle == INVALID_HANDLE_VALUE)
			throw SystemError::GetLastError();

		// Flush the error code
		// With OPEN_ALWAYS, if the file already exists, this last error code is set to ERROR_ALREADY_EXISTS but we don't care here
		::GetLastError();

		CloseHandle(handle);
	}

	void LocalFileSystemDriver::CreateDirectory(const Path& path)
	{
		Path fullPath = Path::Merge(mBase, path);
		if (::CreateDirectoryW(fullPath.ToString().GetData(), nullptr) == FALSE)
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

	void LocalFileSystemDriver::Copy(const Path& source, const Path& destination, bool failIfExists)
	{
		if (CopyFile(Path::Merge(mBase, source).ToString().GetData(), Path::Merge(mBase, destination).ToString().GetData(), failIfExists) == FALSE)
			throw SystemError::GetLastError();
	}

	void LocalFileSystemDriver::Move(const Path& source, const Path& destination)
	{
		if (MoveFile(Path::Merge(mBase, source).ToString().GetData(), Path::Merge(mBase, destination).ToString().GetData()) == FALSE)
			throw SystemError::GetLastError();
	}

	void LocalFileSystemDriver::Remove(const Path& path)
	{
		Path fullpath = Path::Merge(mBase, path);
		if (IsFile(fullpath))
		{
			if (DeleteFile(fullpath.ToString().GetData()) == FALSE)
				throw SystemError::GetLastError();
		}
		else if (IsDirectory(fullpath))
		{
			if (RemoveDirectory(fullpath.ToString().GetData()) == FALSE)
				throw SystemError::GetLastError();
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
		return TryMakeCanonical(Path::Merge(mBase, path)).GetOrThrow<std::system_error>(std::make_error_code(std::errc::no_such_file_or_directory));
	}

	Optional<Path> LocalFileSystemDriver::TryMakeCanonical(const Path& path)
	{
		TextChar fullPath[MAX_PATH];
		DWORD fullPathLength = GetFullPathName(path.ToString().GetData(), MAX_PATH, fullPath, nullptr);
		if (fullPathLength == 0 || fullPathLength > MAX_PATH)
		{
			DWORD lastError = ::GetLastError();
			if (lastError == ERROR_FILE_NOT_FOUND)
				return Optional<Path>::Empty();

			throw std::system_error(lastError, SystemError::GetSystemErrorCategory());
		}

		HANDLE hFile = ::CreateFileW(
			fullPath,
			0,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			nullptr,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS,
			nullptr
		);
		if (hFile == INVALID_HANDLE_VALUE)
			throw SystemError::GetLastError();

		TextChar finalPath[MAX_PATH];
		DWORD finalPathLength = GetFinalPathNameByHandle(hFile, finalPath, MAX_PATH, FILE_NAME_NORMALIZED);
		CloseHandle(hFile);

		if (finalPathLength == 0 || finalPathLength > MAX_PATH)
		{
			DWORD lastError = ::GetLastError();
			if (lastError == ERROR_FILE_NOT_FOUND)
				return Optional<Path>::Empty();

			throw std::system_error(lastError, SystemError::GetSystemErrorCategory());
		}

		return Optional<Path>::Of(InPlace, finalPath, finalPathLength);
	}

    PathInfo LocalFileSystemDriver::GetPathInfo(const Path& path) const
    {
        PathInfo pathInfo;

        WIN32_FILE_ATTRIBUTE_DATA fileInfo;
        BOOL result = GetFileAttributesEx(path.ToString().GetData(), GetFileExInfoStandard, &fileInfo);
        if (result == FALSE)
        {
            DWORD lastError = ::GetLastError();
            if (lastError != ERROR_FILE_NOT_FOUND)
                throw std::system_error(lastError, SystemError::GetSystemErrorCategory());

            return pathInfo;
        }

        pathInfo.CreationTime = FiletimeToInstant(fileInfo.ftCreationTime);
        pathInfo.LastAccessTime = FiletimeToInstant(fileInfo.ftLastAccessTime);
        pathInfo.LastChangeTime = FiletimeToInstant(fileInfo.ftLastWriteTime);

        if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
        {
            LARGE_INTEGER fileSize;
            fileSize.HighPart = fileInfo.nFileSizeHigh;
            fileSize.LowPart = fileInfo.nFileSizeLow;

            pathInfo.Type = PathType::File;
            pathInfo.Size = fileSize.QuadPart;
        }
        else if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
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
