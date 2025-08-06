#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectoryWatcher.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	std::string FromUtf16(WCHAR* wString, DWORD length)
	{
		if (!wString || length == 0)
			return "";

		int utf8Length = WideCharToMultiByte(CP_UTF8, 0, wString, length, nullptr, 0, nullptr, nullptr);
		if (utf8Length == 0)
			return "";

		std::string utf8(utf8Length, 0);
		WideCharToMultiByte(CP_UTF8, 0, wString, length, utf8.data(), utf8Length, nullptr, nullptr);

		return utf8;
	}

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

		std::pair<std::filesystem::path, std::filesystem::path> renamePair;

		DWORD offset = 0;
		PFILE_NOTIFY_INFORMATION notify;
		do
		{
			notify = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(&buffer[offset]);
			offset += notify->NextEntryOffset;

			std::filesystem::path filename = mDirectory->GetPath() / FromUtf16(notify->FileName, notify->FileNameLength);
			switch (notify->Action)
			{
			case FILE_ACTION_ADDED:
				mOnFileCreated.Emit(filename);
				break;

			case FILE_ACTION_MODIFIED:
				mOnFileModified.Emit(filename);
				break;

			case FILE_ACTION_RENAMED_OLD_NAME:
				renamePair.first = std::move(filename);
				break;

			case FILE_ACTION_RENAMED_NEW_NAME:
				renamePair.second = std::move(filename);
				break;

			case FILE_ACTION_REMOVED:
				mOnFileDeleted.Emit(filename);
				break;
			}

			if (!renamePair.first.empty() && !renamePair.second.empty())
			{
				mOnFileRenamed.Emit(renamePair.first, renamePair.second);

				renamePair.first = "";
				renamePair.second = "";
			}
		} while (notify->NextEntryOffset != 0);
	}

	Signal<std::filesystem::path>& LocalDirectoryWatcher::OnFileCreated()
	{
		return mOnFileCreated;
	}

	Signal<std::filesystem::path>& LocalDirectoryWatcher::OnFileModified()
	{
		return mOnFileModified;
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
