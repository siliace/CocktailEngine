#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectoryWatcher.hpp>

namespace Ck::Detail::Unix
{
	LocalDirectoryWatcher::LocalDirectoryWatcher(Directory& directory, bool recursive):
		mDirectory(&directory),
		mRecursive(recursive)
	{
		/// Nothing
	}

	void LocalDirectoryWatcher::PollEvents()
	{
	}

	Signal<std::filesystem::path>& LocalDirectoryWatcher::OnFileCreated()
	{
		return mOnFileCreated;
	}

	Signal<std::filesystem::path> & LocalDirectoryWatcher::OnFileModified()
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
