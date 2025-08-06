#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileLock.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	LocalFileLock::LocalFileLock(File& file, std::size_t offset, std::size_t length):
		mFile(&file),
		mOffset(offset),
		mLength(length)
	{
		/// Nothing
	}

	void LocalFileLock::Lock()
	{
		LARGE_INTEGER offset, length;
		offset.QuadPart = mOffset;
		length.QuadPart = mLength;

		BOOL success = LockFile(
			mFile->GetSystemHandle(),
			offset.LowPart, offset.HighPart,
			length.LowPart, length.HighPart
		);
		if (success == FALSE)
			throw SystemError::GetLastError();
	}

	bool LocalFileLock::TryLock()
	{
		LARGE_INTEGER offset, length;
		offset.QuadPart = mOffset;
		length.QuadPart = mLength;

		OVERLAPPED overlapped;
		overlapped.Offset = offset.LowPart;
		overlapped.OffsetHigh = offset.HighPart;
		overlapped.hEvent = nullptr;

		BOOL success = LockFileEx(
			mFile->GetSystemHandle(),
			LOCKFILE_FAIL_IMMEDIATELY,
			0,
			length.LowPart, length.HighPart,
			&overlapped
		);
		if (success == FALSE)
		{
			if (DWORD error = GetLastError(); error != ERROR_IO_PENDING)
				throw std::system_error(error, SystemError::GetSystemErrorCategory());

			return false;
		}

		return true;
	}

	void LocalFileLock::Unlock()
	{
		LARGE_INTEGER offset, length;
		offset.QuadPart = mOffset;
		length.QuadPart = mLength;

		BOOL success = UnlockFile(
			mFile->GetSystemHandle(),
			offset.LowPart, offset.HighPart,
			length.LowPart, length.HighPart
		);
		if (success == FALSE)
			throw SystemError::GetLastError();
	}
}
