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

	Signal<Path>& LocalDirectoryWatcher::OnFileCreated()
	{
		return mOnFileCreated;
	}

	Signal<Path> & LocalDirectoryWatcher::OnFileModified()
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
