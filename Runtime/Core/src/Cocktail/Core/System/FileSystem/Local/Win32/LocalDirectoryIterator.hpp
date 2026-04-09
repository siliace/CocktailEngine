#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_WIN32_LOCALDIRECTORYITERATOR_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_WIN32_LOCALDIRECTORYITERATOR_HPP

#include <Cocktail/Core/System/FileSystem/DirectoryIterator.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
    class LocalDirectoryIterator : public DirectoryIterator
    {
    public:

        LocalDirectoryIterator(const Path& path);
        ~LocalDirectoryIterator() override;
        void Next() override;
        const Path& GetPath() const override;
        PathInfo GetPathInfo() const override;
        bool IsEnd() const override;

    private:

        bool Advance();

        Path mDirectoryPath;
        Path mEntryPath;
        HANDLE mHandle;
        WIN32_FIND_DATAW mFindData;
        bool mFirst;
        bool mValid;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_WIN32_LOCALDIRECTORYITERATOR_HPP
