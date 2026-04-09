#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_UNIX_LOCALDIRECTORYITERATOR_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_UNIX_LOCALDIRECTORYITERATOR_HPP

#include <Cocktail/Core/System/FileSystem/DirectoryIterator.hpp>

namespace Ck::Detail::Unix
{
    class LocalDirectoryIterator : public DirectoryIterator
    {
    public:

        LocalDirectoryIterator(Path directoryPath);
        ~LocalDirectoryIterator() override;

        void Next() override;

        const Path& GetPath() const override;
        PathInfo GetPathInfo() const override;

        bool IsEnd() const override;

    private:

        bool Advance();
        Path mDirectoryPath;
        Path mEntryPath;
        DIR* mDirectoryHandle;
        bool mValid;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_UNIX_LOCALDIRECTORYITERATOR_HPP
