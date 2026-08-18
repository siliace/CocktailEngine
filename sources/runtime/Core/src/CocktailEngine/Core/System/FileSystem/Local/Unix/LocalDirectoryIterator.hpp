#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_UNIX_LOCALDIRECTORYITERATOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_UNIX_LOCALDIRECTORYITERATOR_HPP

#include <CocktailEngine/Core/System/FileSystem/DirectoryIterator.hpp>

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

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_UNIX_LOCALDIRECTORYITERATOR_HPP
