#include <CocktailEngine/Core/System/FileSystem/FileSystemException.hpp>
#include <CocktailEngine/Core/System/Network/TcpClient.hpp>

#include <Ftp/Core/Exception.hpp>

#include <Ftp/FileSystemDriver/Directory.hpp>
#include <Ftp/FileSystemDriver/File.hpp>
#include <Ftp/FileSystemDriver/FileSystemDriver.hpp>
#include <Ftp/FileSystemDriver/Transfer.hpp>

namespace Ck::Ftp
{
    using namespace Detail;

    namespace
    {
        /**
         * \brief Directory iterator working on a listing fetched at creation time
         */
        class ListingDirectoryIterator : public DirectoryIterator
        {
        public:

            struct Entry
            {
                Path EntryPath;
                PathInfo Info;
            };

            explicit ListingDirectoryIterator(Array<Entry> entries) :
                mEntries(Move(entries)),
                mIndex(0)
            {
                /// Nothing
            }

            void Next() override
            {
                if (!IsEnd())
                    mIndex++;
            }

            const Path& GetPath() const override
            {
                return IsEnd() ? Path::Empty : mEntries[mIndex].EntryPath;
            }

            PathInfo GetPathInfo() const override
            {
                return IsEnd() ? PathInfo{} : mEntries[mIndex].Info;
            }

            bool IsEnd() const override
            {
                return mIndex >= mEntries.GetSize();
            }

        private:

            Array<Entry> mEntries;
            unsigned int mIndex;
        };
    }

    FileSystemDriver::FileSystemDriver(const IpAddress& remote, const LoginInfo* loginInfo, unsigned short port) :
        mClient(remote, port)
    {
        Response response = mClient.WaitResponse();
        if (response.Code != 220)
            throw Exception(response.Code);

        if (loginInfo)
            Login(loginInfo->User, loginInfo->Password);

        // Files must be transferred byte per byte, listings are not affected by this mode
        mClient.Send(RepresentationTypeRequest(Utf8String(CK_UTF8_TEXT("I"))));
        response = mClient.WaitResponse();
        if (!response.IsPositiveCompletion())
            throw Exception(response.Code);
    }

    bool FileSystemDriver::IsFile(const Path& path) const
    {
        return GetPathInfo(path).Type == PathType::File;
    }

    bool FileSystemDriver::IsDirectory(const Path& path) const
    {
        return GetPathInfo(path).Type == PathType::Directory;
    }

    void FileSystemDriver::CreateFile(const Path& path)
    {
        PathInfo info = GetPathInfo(path);
        if (info.Type == PathType::File)
            return; // Creating an already existing file is not an error, like the local driver

        if (info.Type != PathType::None)
            throw FileSystemException(FileSystemErrorCode::IsADirectory);

        Upload(mClient, path, Array<Byte>());
    }

    void FileSystemDriver::CreateDirectory(const Path& path)
    {
        if (GetPathInfo(path).Type != PathType::None)
            throw FileSystemException(FileSystemErrorCode::FileExists);

        mClient.Send(MakeDirectoryRequest(ToFtpPath(path)));
        Response response = mClient.WaitResponse();
        if (response.Code != 257)
            throw Exception(response.Code);
    }

    UniquePtr<DirectoryIterator> FileSystemDriver::CreateDirectoryIterator(const Path& path)
    {
        if (GetPathInfo(path).Type != PathType::Directory)
            return nullptr;

        Array<ListingEntry> listing = ListDirectory(mClient, path);

        Array<ListingDirectoryIterator::Entry> entries;
        for (const ListingEntry& listed : listing)
        {
            if (listed.Skip)
                continue;

            entries.Add({ Path::Merge(path, String::Convert(Utf8StringView(listed.Name))), listed.Info });
        }

        return MakeUnique<ListingDirectoryIterator>(Move(entries));
    }

    UniquePtr<Ck::File> FileSystemDriver::OpenFile(const Path& path, const FileOpenFlags& flags)
    {
        if (!(flags & FileOpenFlagBits::Read) && !(flags & FileOpenFlagBits::Write))
            throw InvalidParameterException();

        PathInfo info = GetPathInfo(path);
        if (info.Type == PathType::Directory || info.Type == PathType::Other)
            throw FileSystemException(FileSystemErrorCode::IsADirectory);

        Array<Byte> content;
        bool modified = false;
        if (info.Type == PathType::None)
        {
            bool create = (flags & FileOpenFlagBits::Write) && !(flags & FileOpenFlagBits::Existing);
            if (!create)
                throw FileSystemException(FileSystemErrorCode::NoSuchFileOrDirectory);

            // Create the file right now so that it exists on the server even if it is never written
            Upload(mClient, path, content);
        }
        else if ((flags & FileOpenFlagBits::Write) && (flags & FileOpenFlagBits::Truncate))
        {
            modified = true;
        }
        else
        {
            content = Download(mClient, path);
        }

        return MakeUnique<Ftp::File>(mClient, path, flags, Move(content), modified);
    }

    UniquePtr<Ck::Directory> FileSystemDriver::OpenDirectory(const Path& path)
    {
        PathInfo info = GetPathInfo(path);
        if (info.Type == PathType::None)
            throw FileSystemException(FileSystemErrorCode::NoSuchFileOrDirectory);

        if (info.Type != PathType::Directory)
            throw FileSystemException(FileSystemErrorCode::NotADirectory);

        return MakeUnique<Ftp::Directory>(mClient, path);
    }

    void FileSystemDriver::CopyFile(const Path& source, const Path& destination, const FileCopyOptions& options)
    {
        PathInfo sourceInfo = GetPathInfo(source);
        if (sourceInfo.Type == PathType::None)
            throw FileSystemException(FileSystemErrorCode::NoSuchFileOrDirectory);

        if (sourceInfo.Type != PathType::File)
            throw FileSystemException(FileSystemErrorCode::IsADirectory);

        PathInfo destinationInfo = GetPathInfo(destination);
        if (destinationInfo.Type != PathType::None)
        {
            if (destinationInfo.Type != PathType::File)
                throw FileSystemException(FileSystemErrorCode::IsADirectory);

            if (!options.Overwrite)
                throw FileSystemException(FileSystemErrorCode::FileExists);
        }

        // FTP has no server side copy, the content goes through the client
        // options.PreserveMetadata is ignored, FTP can not set the creation time and the modification time setting (MFMT) is an optional extension
        Upload(mClient, destination, Download(mClient, source));
    }

    void FileSystemDriver::CopyDirectory(const Path& source, const Path& destination, const DirectoryCopyOptions& options)
    {
        PathInfo sourceInfo = GetPathInfo(source);
        if (sourceInfo.Type == PathType::None)
            throw FileSystemException(FileSystemErrorCode::NoSuchFileOrDirectory);

        if (sourceInfo.Type != PathType::Directory)
            throw FileSystemException(FileSystemErrorCode::NotADirectory);

        PathInfo destinationInfo = GetPathInfo(destination);
        if (destinationInfo.Type == PathType::None)
            CreateDirectory(destination);
        else if (destinationInfo.Type != PathType::Directory)
            throw FileSystemException(FileSystemErrorCode::NotADirectory);

        Array<ListingEntry> listing = ListDirectory(mClient, source);
        for (const ListingEntry& listed : listing)
        {
            if (listed.Skip)
                continue;

            String name = String::Convert(Utf8StringView(listed.Name));
            Path childSource = Path::Merge(source, name);
            Path childDestination = Path::Merge(destination, name);

            if (listed.Info.Type == PathType::Directory)
            {
                if (options.Recursive)
                    CopyDirectory(childSource, childDestination, options);
            }
            else if (listed.Info.Type == PathType::File)
            {
                if (!options.OnlyStructure)
                    CopyFile(childSource, childDestination, options);
            }
        }
    }

    void FileSystemDriver::MoveFile(const Path& source, const Path& destination, const FileMoveOptions& options)
    {
        PathInfo sourceInfo = GetPathInfo(source);
        if (sourceInfo.Type == PathType::None)
            throw FileSystemException(FileSystemErrorCode::NoSuchFileOrDirectory);

        if (sourceInfo.Type != PathType::File)
            throw FileSystemException(FileSystemErrorCode::IsADirectory);

        PathInfo destinationInfo = GetPathInfo(destination);
        if (destinationInfo.Type != PathType::None)
        {
            if (destinationInfo.Type != PathType::File)
                throw FileSystemException(FileSystemErrorCode::IsADirectory);

            if (!options.Overwrite)
                throw FileSystemException(FileSystemErrorCode::FileExists);

            // RNTO does not replace an existing file on every server
            RemoveFile(destination, FileRemoveOptions());
        }

        // Source and destination live on the same server, renaming is always possible so options.AllowCopyFallback is not needed
        Rename(source, destination);
    }

    void FileSystemDriver::MoveDirectory(const Path& source, const Path& destination, const DirectoryMoveOptions& options)
    {
        PathInfo sourceInfo = GetPathInfo(source);
        if (sourceInfo.Type == PathType::None)
            throw FileSystemException(FileSystemErrorCode::NoSuchFileOrDirectory);

        if (sourceInfo.Type != PathType::Directory)
            throw FileSystemException(FileSystemErrorCode::NotADirectory);

        PathInfo destinationInfo = GetPathInfo(destination);
        if (destinationInfo.Type != PathType::None)
        {
            if (destinationInfo.Type != PathType::Directory)
                throw FileSystemException(FileSystemErrorCode::NotADirectory);

            if (!options.Overwrite)
                throw FileSystemException(FileSystemErrorCode::FileExists);

            // Only an empty directory can be replaced
            mClient.Send(RemoveDirectoryRequest(ToFtpPath(destination)));
            Response response = mClient.WaitResponse();
            if (!response.IsPositiveCompletion())
                throw FileSystemException(FileSystemErrorCode::DirectoryNotEmpty);
        }

        // options.Recursive and options.OnlyStructure only apply to a copy fallback which is never needed on a single server
        Rename(source, destination);
    }

    void FileSystemDriver::RemoveFile(const Path& path, const FileRemoveOptions& options)
    {
        PathInfo info = GetPathInfo(path);
        if (info.Type == PathType::None)
        {
            if (options.IgnoreMissing)
                return;

            throw FileSystemException(FileSystemErrorCode::NoSuchFileOrDirectory);
        }

        if (info.Type == PathType::Directory)
            throw FileSystemException(FileSystemErrorCode::IsADirectory);

        // options.Force is a no-op, FTP has no read-only attribute handling
        mClient.Send(DeleteRequest(ToFtpPath(path)));
        Response response = mClient.WaitResponse();
        if (!response.IsPositiveCompletion())
            throw Exception(response.Code);
    }

    void FileSystemDriver::RemoveDirectory(const Path& path, const DirectoryRemoveOptions& options)
    {
        PathInfo info = GetPathInfo(path);
        if (info.Type == PathType::None)
        {
            if (options.IgnoreMissing)
                return;

            throw FileSystemException(FileSystemErrorCode::NoSuchFileOrDirectory);
        }

        if (info.Type != PathType::Directory)
            throw FileSystemException(FileSystemErrorCode::NotADirectory);

        // RMD only works on empty directories
        Array<ListingEntry> listing = ListDirectory(mClient, path);
        for (const ListingEntry& listed : listing)
        {
            if (listed.Skip)
                continue;

            if (!options.Recursive)
                throw FileSystemException(FileSystemErrorCode::DirectoryNotEmpty);

            Path child = Path::Merge(path, String::Convert(Utf8StringView(listed.Name)));
            if (listed.Info.Type == PathType::Directory)
                RemoveDirectory(child, options);
            else
                RemoveFile(child, options);
        }

        mClient.Send(RemoveDirectoryRequest(ToFtpPath(path)));
        Response response = mClient.WaitResponse();
        if (!response.IsPositiveCompletion())
            throw Exception(response.Code);
    }

    Path FileSystemDriver::MakeCanonical(const Path& path)
    {
        return TryMakeCanonical(path).GetOrThrow<FileSystemException>(FileSystemErrorCode::NoSuchFileOrDirectory);
    }

    Optional<Path> FileSystemDriver::TryMakeCanonical(const Path& path)
    {
        if (GetPathInfo(path).Type == PathType::None)
            return Optional<Path>::Empty();

        Path generic = path.ToFormat(Path::Format::Generic);
        String text = generic.ToString();
        if (!text.IsEmpty() && text[0] == CK_CHAR('/'))
            return Optional<Path>::Of(generic);

        // Relative paths are resolved from the working directory of the session
        Path workingDirectory = Path::Parse(String::Convert(Utf8StringView(GetWorkingDirectory(mClient))), Path::Format::Generic);

        return Optional<Path>::Of(Path::Merge(workingDirectory, generic));
    }

    PathInfo FileSystemDriver::GetPathInfo(const Path& path) const
    {
        mClient.Send(MachineListRequest(ToFtpPath(path)));
        Response response = mClient.WaitResponse();

        if (response.Code == 250)
        {
            Array<ListingEntry> entries;
            ParseListingLines(response.Message.GetData(), response.Message.GetLength(), true, entries);

            if (entries.IsEmpty())
                return PathInfo{};

            return entries[0].Info;
        }

        // The path does not exist
        if (response.Code == 550)
            return PathInfo{};

        // MLST is an optional extension, rely on legacy commands when the server does not know it
        if (response.Code >= 500 && response.Code <= 504)
            return GetPathInfoFromFallback(mClient, path);

        throw Exception(response.Code);
    }

    void FileSystemDriver::Rename(const Path& source, const Path& destination)
    {
        mClient.Send(RenameFromRequest(ToFtpPath(source)));
        Response response = mClient.WaitResponse();

        // RNFR must return 350 before RNTO can be sent
        if (response.Code != 350)
            throw Exception(response.Code);

        mClient.Send(RenameToRequest(ToFtpPath(destination)));
        response = mClient.WaitResponse();
        if (!response.IsPositiveCompletion())
            throw Exception(response.Code);
    }

    void FileSystemDriver::Login(const Utf8String& user, const Utf8String& password) const
    {
        mClient.Send(UserRequest(user));
        Response response = mClient.WaitResponse();
        if (response.Code == 331)
        {
            mClient.Send(PasswordRequest(password));
            response = mClient.WaitResponse();
            if (response.Code != 230)
                throw Exception(response.Code);
        }
        else if (response.Code != 230)
        {
            throw Exception(response.Code);
        }
    }
}
