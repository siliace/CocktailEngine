#ifndef COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_FTPFILESYSTEMDRIVER_HPP
#define COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_FTPFILESYSTEMDRIVER_HPP

#include <CocktailEngine/Core/System/FileSystem/FileSystemDriver.hpp>

#include <Ftp/Core/Client.hpp>

#include <Ftp/FileSystemDriver/Export.hpp>

namespace Ck
{
    namespace Ftp
    {
        class FTP_FILESYSTEMDRIVER_API FileSystemDriver : public Ck::FileSystemDriver
        {
        public:

            FileSystemDriver(const IpAddress& remote, unsigned short port = 21);
            bool IsFile(const Path& path) const override;
            bool IsDirectory(const Path& path) const override;
            void CreateFile(const Path& path) override;
            void CreateDirectory(const Path& path) override;
            UniquePtr<DirectoryIterator> CreateDirectoryIterator(const Path& path) override;
            UniquePtr<File> OpenFile(const Path& path, const FileOpenFlags& flags) override;
            UniquePtr<Directory> OpenDirectory(const Path& path) override;
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

            Client mClient;
        };
    }

    using FtpFileSystemDriver = Ftp::FileSystemDriver;
}

#endif // COCKTAILENGINE_FTP_FTPFILESYSTEMDRIVER_FTPFILESYSTEMDRIVER_HPP
