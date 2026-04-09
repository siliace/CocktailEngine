#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDFILESYSTEMDRIVER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDFILESYSTEMDRIVER_HPP

#include <cmrc/cmrc.hpp>

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/System/FileSystem/FileSystemDriver.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API EmbeddedFileSystemDriver : public FileSystemDriver
	{
	public:

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

		Path MakeCanonical(const Path& path) override;
		Optional<Path> TryMakeCanonical(const Path& path) override;

        PathInfo GetPathInfo(const Path& path) const override;

		void Register(cmrc::embedded_filesystem fileSystem);

	private:

		Array<cmrc::embedded_filesystem> mFileSystems;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDFILESYSTEMDRIVER_HPP
