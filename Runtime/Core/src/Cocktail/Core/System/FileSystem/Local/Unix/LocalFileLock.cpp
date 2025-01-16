#include <cstdint>
#include <sys/file.h>

#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileLock.hpp>

namespace Ck::Detail::Unix
{
	LocalFileLock::LocalFileLock(const Ref<File>& file, std::size_t offset, std::size_t length):
		mFile(file),
		mOffset(offset),
		mLength(length)
	{
		/// Nothing
	}

	void LocalFileLock::Lock()
	{
		int error = flock((intptr_t)mFile->GetSystemHandle(), LOCK_EX);
		if (error == -1)
			throw SystemError::GetLastError();
	}

	bool LocalFileLock::TryLock()
	{
		int error = flock((intptr_t)mFile->GetSystemHandle(), LOCK_EX);
		if (error == -1)
		{
			if (errno != EWOULDBLOCK)
				throw SystemError::GetLastError();

			return false;
		}

		return true;
	}

	void LocalFileLock::Unlock()
	{
		int error = flock((intptr_t)mFile->GetSystemHandle(), LOCK_EX);
		if (error == -1)
			throw SystemError::GetLastError();
	}
}
