#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystem.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectoryIterator.hpp>
#include <Cocktail/Core/System/SystemError.hpp>

namespace Ck::Detail::Win32
{
    LocalDirectoryIterator::LocalDirectoryIterator(const Path& path) :
        mDirectoryPath(path),
        mHandle(INVALID_HANDLE_VALUE),
        mFirst(true),
        mValid(true)
    {
        String pattern = mDirectoryPath.ToString() + CK_TEXT("\\*");

        mHandle = FindFirstFileW(pattern.GetData(), &mFindData);
        if (mHandle == INVALID_HANDLE_VALUE)
        {
            mValid = false;
            return;
        }

        mValid = Advance();
    }

    LocalDirectoryIterator::~LocalDirectoryIterator()
    {
        if (mHandle != INVALID_HANDLE_VALUE)
            FindClose(mHandle);
    }

    void LocalDirectoryIterator::Next()
    {
        if (!mValid)
            return;

        mValid = Advance();
    }

    const Path& LocalDirectoryIterator::GetPath() const
    {
        return mEntryPath;
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
            if (!mFirst)
            {
                if (FindNextFileW(mHandle, &mFindData) == FALSE)
                    return false;
            }

            mFirst = false;
            StringView filename = mFindData.cFileName;

            if (filename != CK_TEXT(".") && filename != CK_TEXT(".."))
            {
                mEntryPath = Path::Merge(mDirectoryPath, filename);
                return true;
            }
        }
    }
}
