#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFile.hpp>
#include <Cocktail/Core/System/SystemError.hpp>

namespace Ck::Detail::Win32
{
	LocalFile::LocalFile(const Path& path, const FileOpenFlags& flags) :
		mPath(path)
	{
		DWORD desiredAccess = 0;
		DWORD shareMode = FILE_SHARE_READ;
		DWORD creationDisposition = 0;
		DWORD attributes = flags & FileOpenFlagBits::Unbuffered ? FILE_FLAG_NO_BUFFERING : FILE_ATTRIBUTE_NORMAL;

		if (flags & FileOpenFlagBits::Read)
		{
			desiredAccess |= GENERIC_READ;
			creationDisposition = OPEN_EXISTING;
		}

		if (flags & FileOpenFlagBits::Write)
		{
			desiredAccess |= flags & FileOpenFlagBits::Append ? FILE_APPEND_DATA : GENERIC_WRITE;

			if (flags & FileOpenFlagBits::Truncate)
			{
				creationDisposition = CREATE_ALWAYS;
			}
			else if (flags & FileOpenFlagBits::Existing)
			{
				creationDisposition = OPEN_EXISTING;
			}
			else
			{
				creationDisposition = OPEN_ALWAYS;
			}
		}

		if (!(flags & FileOpenFlagBits::Locked))
			shareMode |= FILE_SHARE_WRITE;
		
		mHandle = CreateFileW(
			mPath.ToString().GetData(),
			desiredAccess,
			shareMode,
			nullptr,
			creationDisposition,
			attributes,
			nullptr
		);

		if (mHandle == INVALID_HANDLE_VALUE)
			throw SystemError::GetLastError();
	}

	LocalFile::~LocalFile()
	{
		CloseHandle(mHandle);
	}

	unsigned int LocalFile::Read(void* buffer, unsigned int length)
	{
		DWORD read;
		if (ReadFile(mHandle, buffer, length, &read, nullptr) == FALSE)
			throw SystemError::GetLastError();

		return read;
	}

	unsigned int LocalFile::Write(const void* buffer, unsigned int length)
	{
		DWORD written;
		if (WriteFile(mHandle, buffer, length, &written, nullptr) == FALSE)
			throw SystemError::GetLastError();

		return written;
	}

	Uint64 LocalFile::GetCursor() const
	{
		LARGE_INTEGER distanceToMove;
		distanceToMove.QuadPart = 0;

		LARGE_INTEGER newFilePointer;
		if (SetFilePointerEx(mHandle, distanceToMove, &newFilePointer, FILE_CURRENT) == FALSE)
			throw SystemError::GetLastError();

		return newFilePointer.QuadPart;
	}

	Uint64 LocalFile::SetCursor(FileCursorMode mode, Int64 offset)
	{
		LARGE_INTEGER distanceToMove;
		distanceToMove.QuadPart = offset;

		DWORD moveMethod = 0;
		switch (mode)
		{
		case FileCursorMode::Begin:
			moveMethod = FILE_BEGIN;
			break;

		case FileCursorMode::Current:
			moveMethod = FILE_CURRENT;
			break;

		case FileCursorMode::End:
			moveMethod = FILE_END;
			break;
		}

		LARGE_INTEGER newFilePointer;
		if (SetFilePointerEx(mHandle, distanceToMove, &newFilePointer, moveMethod) == FALSE)
			throw SystemError::GetLastError();

		return newFilePointer.QuadPart;
	}

	std::size_t LocalFile::GetSize() const
	{
		LARGE_INTEGER fileSize;
		if (GetFileSizeEx(mHandle, &fileSize) == FALSE)
			throw SystemError::GetLastError();

		return fileSize.QuadPart;
	}

	const Path& LocalFile::GetPath() const
	{
		return mPath;
	}

	void LocalFile::Flush() const
	{
		if (FlushFileBuffers(mHandle) == FALSE)
			throw SystemError::GetLastError();
	}

	void* LocalFile::GetSystemHandle() const
	{
		return mHandle;
	}
}
