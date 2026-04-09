#include <dirent.h>

#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystem.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectoryIterator.hpp>
#include <Cocktail/Core/System/SystemError.hpp>

namespace Ck::Detail::Unix
{
    LocalDirectoryIterator::LocalDirectoryIterator(Path directoryPath) :
        mDirectoryPath(std::move(directoryPath)),
        mDirectoryHandle(nullptr),
        mValid(true)
    {
        mDirectoryHandle = opendir(reinterpret_cast<const char*>(mDirectoryPath.ToString().GetData()));
        if (!mDirectoryHandle)
        {
            mValid = false;
            return;
        }

        mValid = Advance();
    }

    LocalDirectoryIterator::~LocalDirectoryIterator()
    {
        if (mDirectoryHandle)
        {
            closedir(mDirectoryHandle);
            mDirectoryHandle = nullptr;
        }
    }

    void LocalDirectoryIterator::Next()
    {
        if (!mValid)
            return;

        mValid = Advance();
    }

    const Path& LocalDirectoryIterator::GetPath() const
    {
        return mValid ? mEntryPath : Path::Empty;
    }

    PathInfo LocalDirectoryIterator::GetPathInfo() const
    {
        if (!mValid)
            return {};

        return LocalFileSystem::GetRootDriver()->GetPathInfo(mEntryPath);
    }

    bool LocalDirectoryIterator::IsEnd() const
    {
        return !mValid;
    }

    bool LocalDirectoryIterator::Advance()
    {
        while (true)
        {
            dirent* entry = readdir(mDirectoryHandle);
            if (!entry)
                return false;

            StringView filename = reinterpret_cast<const Utf8Char*>(entry->d_name);
            if (filename != CK_TEXT(".") && filename != CK_TEXT(".."))
            {
                mEntryPath = Path::Merge(mDirectoryPath, filename);
                return true;
            }
        }
    }
}
