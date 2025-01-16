#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectoryWatcher.hpp>

namespace Ck::Detail::Unix
{
	LocalDirectoryWatcher::LocalDirectoryWatcher(const Ref<Directory>& directory, bool recursive):
		mDirectory(directory),
		mRecursive(recursive)
	{
	}

	void LocalDirectoryWatcher::PollEvents()
	{
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
