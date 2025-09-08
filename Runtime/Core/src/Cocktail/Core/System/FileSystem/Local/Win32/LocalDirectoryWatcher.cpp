#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectoryWatcher.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	LocalDirectoryWatcher::LocalDirectoryWatcher(Directory& directory, bool recursive):
		mDirectory(&directory),
		mRecursive(recursive)
	{
		/// Nothing
	}

	void LocalDirectoryWatcher::PollEvents()
	{
		DWORD bufferSize;
		DWORD buffer[NotificationBufferSize] = {};

		BOOL success = ReadDirectoryChangesW(
			mDirectory->GetSystemHandle(),
			&buffer, sizeof(buffer),
			mRecursive,
			FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_WRITE | 
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME,
			&bufferSize, 
			nullptr, 
			nullptr
		);

		if (success == FALSE)
			throw SystemError::GetLastError();

		std::pair<Path, Path> renamePair;

		DWORD offset = 0;
		PFILE_NOTIFY_INFORMATION notify;
		do
		{
			notify = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(&buffer[offset]);
			offset += notify->NextEntryOffset;

			Path changeFullPath = mDirectory->GetPath().Join(notify->FileName, notify->FileNameLength);
			switch (notify->Action)
			{
			case FILE_ACTION_ADDED:
				mOnFileCreated.Emit(changeFullPath);
				break;

			case FILE_ACTION_MODIFIED:
				mOnFileModified.Emit(changeFullPath);
				break;

			case FILE_ACTION_RENAMED_OLD_NAME:
				renamePair.first = std::move(changeFullPath);
				break;

			case FILE_ACTION_RENAMED_NEW_NAME:
				renamePair.second = std::move(changeFullPath);
				break;

			case FILE_ACTION_REMOVED:
				mOnFileDeleted.Emit(changeFullPath);
				break;
			}

			if (!renamePair.first.IsEmpty() && !renamePair.second.IsEmpty())
			{
				mOnFileRenamed.Emit(renamePair.first, renamePair.second);

				renamePair.first = String::Empty;
				renamePair.second = String::Empty;
			}
		} while (notify->NextEntryOffset != 0);
	}

	Signal<Path>& LocalDirectoryWatcher::OnFileCreated()
	{
		return mOnFileCreated;
	}

	Signal<Path>& LocalDirectoryWatcher::OnFileModified()
	{
		return mOnFileModified;
	}

	Signal<Path, Path>& LocalDirectoryWatcher::OnFileRenamed()
	{
		return mOnFileRenamed;
	}

	Signal<Path>& LocalDirectoryWatcher::OnFileDeleted()
	{
		return mOnFileDeleted;
	}
}
