#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILESYSTEMDRIVER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILESYSTEMDRIVER_HPP

#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemDriver.hpp>

namespace Ck::Detail::Unix
{
	class LocalFileSystemDriver : public Ck::LocalFileSystemDriver
	{
	public:

		explicit LocalFileSystemDriver(Path base = Path::Empty);

		bool IsFile(const Path& path) const override;
		bool IsDirectory(const Path& path) const override;

		void CreateFile(const Path& path) override;
		void CreateDirectory(const Path& path) override;

		UniquePtr<File> OpenFile(const Path& path, const FileOpenFlags& flags) override;
		UniquePtr<Directory> OpenDirectory(const Path& path) override;
        UniquePtr<DirectoryIterator> CreateDirectoryIterator(const Path& path) override;

	    void CopyFile(const Path& source, const Path& destination, const FileCopyOptions& options) override;
        void CopyDirectory(const Path& source, const Path& destination, const DirectoryCopyOptions& options) override;
        void MoveFile(const Path& source, const Path& destination, const FileMoveOptions& options) override;
        void MoveDirectory(const Path& source, const Path& destination, const DirectoryMoveOptions& options) override;
        void RemoveFile(const Path& path, const FileRemoveOptions& options) override;
        void RemoveDirectory(const Path& path, const DirectoryRemoveOptions& options) override;

        UniquePtr<FileLock> CreateLock(File& file, std::size_t offset, std::size_t length) override;
		UniquePtr<DirectoryWatcher> CreateWatcher(Directory& directory, bool recursive) override;

		Path MakeCanonical(const Path& path) override;
		Optional<Path> TryMakeCanonical(const Path& path) override;

        PathInfo GetPathInfo(const Path& path) const override;

    private:

		Path mBase;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILESYSTEMDRIVER_HPP
