#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDDIRECTORYITERATOR_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDDIRECTORYITERATOR_HPP

#include <Cocktail/Core/System/FileSystem/DirectoryIterator.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>

namespace Ck
{
    class EmbeddedDirectoryIterator final : public DirectoryIterator
    {
    public:

        EmbeddedDirectoryIterator(cmrc::embedded_filesystem fileSystem, Path directoryPath);

        void Next() override;

        const Path& GetPath() const override;
        PathInfo GetPathInfo() const override;

        bool IsEnd() const override;

    private:

        cmrc::embedded_filesystem mFileSystem;
        cmrc::directory_iterator mIterable;
        Path mDirectoryPath;
        Path mCurrentEntry;
    };
}

#endif //COCKTAILENGINE_EMBDEDDDIRECTORYITERATOR_HPP
