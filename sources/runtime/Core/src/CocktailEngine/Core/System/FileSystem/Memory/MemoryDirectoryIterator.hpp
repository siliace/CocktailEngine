#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYDIRECTORYITERATOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYDIRECTORYITERATOR_HPP

#include <CocktailEngine/Core/Memory/SharedPtr.hpp>
#include <CocktailEngine/Core/System/FileSystem/DirectoryIterator.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemNode.hpp>

namespace Ck::Detail
{
    /**
     * \brief Iterator over the entries of a directory held in RAM
     *
     * The iterator works on a snapshot of the entries taken when it is created, so
     * that the virtual filesystem can be modified while it is being walked. Each
     * snapshotted entry is kept alive by the iterator.
     *
     * \see MemoryFileSystemDriver
     */
    class MemoryDirectoryIterator final : public DirectoryIterator
    {
    public:

        /**
         * \brief Type of the snapshot the iterator walks
         */
        using EntryArray = Array<SharedPtr<MemoryFileSystemNode>>;

        /**
         * \brief Constructor
         *
         * \param entries The entries to walk
         */
        explicit MemoryDirectoryIterator(EntryArray entries);

        void Next() override;

        /**
         * \brief Get the path of the current entry
         *
         * \return The path of the entry, relative to the root of the virtual
         *         filesystem, or an empty path once the end has been reached
         */
        const Path& GetPath() const override;

        PathInfo GetPathInfo() const override;

        bool IsEnd() const override;

    private:

        EntryArray mEntries; /*!< Snapshot of the entries of the directory */
        EntryArray::SizeType mIndex; /*!< Index of the current entry within the snapshot */
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYDIRECTORYITERATOR_HPP
