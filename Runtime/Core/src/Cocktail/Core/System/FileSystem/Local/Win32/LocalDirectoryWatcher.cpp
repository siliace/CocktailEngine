#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectoryWatcher.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	LocalDirectoryWatcher::LocalDirectoryWatcher(const std::shared_ptr<Directory>& directory, bool recursive):
		mDirectory(directory),
		mRecursive(recursive),
		mBuffer{}
	{
		std::memset(mBuffer, 0, sizeof(mBuffer));
	}

	void LocalDirectoryWatcher::PollEvents()
	{
		HANDLE directoryHandle = mDirectory->GetSystemHandle();

		BOOL success = ReadDirectoryChangesW(
			directoryHandle, mBuffer, sizeof(mBuffer), mRecursive,
			FILE_NOTIFY_CHANGE_FILE_NAME, nullptr, nullptr, nullptr
		);

		if (success == FALSE)
			throw SystemError::GetLastError();
	}

	Signal<std::filesystem::path>& LocalDirectoryWatcher::OnFileCreated()
	{
		return mOnFileCreated;
	}

	Signal<std::filesystem::path, std::filesystem::path>& LocalDirectoryWatcher::OnFileRenamed()
	{
		return mOnFileRenamed;
	}

	Signal<std::filesystem::path>& LocalDirectoryWatcher::OnFileDeleted()
	{
		return mOnFileDeleted;
	}
}
