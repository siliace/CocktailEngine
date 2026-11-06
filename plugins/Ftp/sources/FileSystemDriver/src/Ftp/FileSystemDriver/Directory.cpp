#include <Ftp/FileSystemDriver/Directory.hpp>
#include <Ftp/FileSystemDriver/Transfer.hpp>

namespace Ck::Ftp
{
    Directory::Directory(const Client& client, Path path) :
        mClient(&client),
        mPath(Move(path))
    {
        /// Nothing
    }

    Array<Path> Directory::GetContent() const
    {
        Array<Detail::ListingEntry> listing = Detail::ListDirectory(*mClient, mPath);

        Array<Path> content;
        content.Reserve(listing.GetSize());

        for (const Detail::ListingEntry& entry : listing)
        {
            if (entry.Skip)
                continue;

            content.Add(Path::Merge(mPath, String::Convert(Utf8StringView(entry.Name))));
        }

        return content;
    }

    const Path& Directory::GetPath() const
    {
        return mPath;
    }

    void* Directory::GetSystemHandle() const
    {
        return nullptr;
    }
}
