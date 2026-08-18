#include <Ftp/FileSystemDriver/FtpFileSystemDriver.hpp>

namespace Ck::Ftp
{
    FileSystemDriver::FileSystemDriver(const IpAddress& remote, unsigned short port) :
        mClient(remote, port)
    {
        /// Nothing
    }

    bool FileSystemDriver::IsFile(const Path& path) const
    {
        FileSizeRequest request(Utf8String::Convert(path.ToString()));
        Response response = mClient.Send(request);

        // 213 indicates that the file size was returned
        return response.Code == 213;
    }

    bool FileSystemDriver::IsDirectory(const Path& path) const
    {
        MachineListRequest request(Utf8String::Convert(path.ToString()));
        Response response = mClient.Send(request);

        // TODO: Parse the MLST facts to determine whether the path is a directory
        // A 250 response alone does not distinguish files from directories
        return false;
    }

    void FileSystemDriver::CreateFile(const Path& path)
    {
        StoreRequest request(Utf8String::Convert(path.ToString()));

        // TODO: Open the passive-mode data connection before sending STOR
        // TODO: Close the data connection and consume the final FTP response
        mClient.Send(request);
    }

    void FileSystemDriver::CreateDirectory(const Path& path)
    {
        MakeDirectoryRequest request(Utf8String::Convert(path.ToString()));
        mClient.Send(request);
    }

    UniquePtr<DirectoryIterator> FileSystemDriver::CreateDirectoryIterator(const Path& path)
    {
        MachineListRequest request(Utf8String::Convert(path.ToString()));

        // TODO: Open a data connection, send MLSD, read its listing and build the iterator
        mClient.Send(request);

        return nullptr;
    }

    UniquePtr<File> FileSystemDriver::OpenFile(const Path& path, const FileOpenFlags& flags)
    {
        // TODO: Select RETR or STOR according to the requested access flags
        // TODO: Create the data connection and return a File implementation
        return nullptr;
    }

    UniquePtr<Directory> FileSystemDriver::OpenDirectory(const Path& path)
    {
        MachineListRequest request(Utf8String::Convert(path.ToString()));
        Response response = mClient.Send(request);

        // TODO: Parse the MLST facts and construct a Directory only if type=dir
        return nullptr;
    }

    void FileSystemDriver::CopyFile(const Path& source, const Path& destination, const FileCopyOptions& options)
    {
        // TODO: RETR source and stream the received bytes to STOR destination
        // TODO: Apply FileCopyOptions
    }

    void FileSystemDriver::CopyDirectory(const Path& source, const Path& destination, const DirectoryCopyOptions& options)
    {
        // TODO: Create the destination directory and recursively copy its contents
        // TODO: Apply DirectoryCopyOptions
    }

    void FileSystemDriver::MoveFile(const Path& source, const Path& destination, const FileMoveOptions& options)
    {
        RenameFromRequest fromRequest(Utf8String::Convert(source.ToString()));
        Response response = mClient.Send(fromRequest);

        // RNFR must return 350 before RNTO can be sent
        if (response.Code != 350)
            return;

        RenameToRequest toRequest(Utf8String::Convert(destination.ToString()));
        mClient.Send(toRequest);

        // TODO: Apply FileMoveOptions and report failures
    }

    void FileSystemDriver::MoveDirectory(const Path& source, const Path& destination, const DirectoryMoveOptions& options)
    {
        RenameFromRequest fromRequest(Utf8String::Convert(source.ToString()));
        Response response = mClient.Send(fromRequest);

        if (response.Code != 350)
            return;

        RenameToRequest toRequest(Utf8String::Convert(destination.ToString()));
        mClient.Send(toRequest);

        // TODO: Apply DirectoryMoveOptions and report failures
    }

    void FileSystemDriver::RemoveFile(const Path& path, const FileRemoveOptions& options)
    {
        DeleteRequest request(Utf8String::Convert(path.ToString()));
        mClient.Send(request);

        // TODO: Apply FileRemoveOptions and report failures
    }

    void FileSystemDriver::RemoveDirectory(const Path& path, const DirectoryRemoveOptions& options)
    {
        RemoveDirectoryRequest request(Utf8String::Convert(path.ToString()));
        mClient.Send(request);

        // TODO: Apply DirectoryRemoveOptions and report failures
    }

    Path FileSystemDriver::MakeCanonical(const Path& path)
    {
        return path;
    }

    Optional<Path> FileSystemDriver::TryMakeCanonical(const Path& path)
    {
        return Optional<Path>::Of(path);
    }

    PathInfo FileSystemDriver::GetPathInfo(const Path& path) const
    {
        return PathInfo{};
    }
}
