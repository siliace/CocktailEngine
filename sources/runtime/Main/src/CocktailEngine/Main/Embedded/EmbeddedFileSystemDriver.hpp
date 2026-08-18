#ifndef COCKTAILENGINE_MAIN_EMBEDDED_EMBEDDEDFILESYSTEMDRIVER_HPP
#define COCKTAILENGINE_MAIN_EMBEDDED_EMBEDDEDFILESYSTEMDRIVER_HPP

#include <cmrc/cmrc.hpp>

#include <CocktailEngine/Core/Export.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileSystemDriver.hpp>

namespace Ck
{
	class EmbeddedFileSystemDriver : public FileSystemDriver
	{
	public:

	    explicit EmbeddedFileSystemDriver(cmrc::embedded_filesystem fileSystem);

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

	private:

		cmrc::embedded_filesystem mFileSystem;
	};
}

#endif // COCKTAILENGINE_MAIN_EMBEDDED_EMBEDDEDFILESYSTEMDRIVER_HPP
