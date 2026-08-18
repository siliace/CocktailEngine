#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYDIRECTORYWATCHER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYDIRECTORYWATCHER_HPP

#include <CocktailEngine/Core/HashMap.hpp>
#include <CocktailEngine/Core/Memory/SharedPtr.hpp>
#include <CocktailEngine/Core/System/FileSystem/DirectoryWatcher.hpp>
#include <CocktailEngine/Core/System/FileSystem/PathInfo.hpp>

namespace Ck
{
    class MemoryFileSystemDriver;

    namespace Detail
    {
        struct MemoryFileSystemNode;

        /**
         * \brief Polling directory watcher for MemoryFileSystemDriver
         *
         * The watcher compares the state visible at consecutive PollEvents() calls.
         * Snapshots retain their entries so an unlinked node can be identified as a
         * deletion rather than being confused with a newly allocated node.
         */
        class MemoryDirectoryWatcher final : public DirectoryWatcher
        {
        public:

            /**
             * \brief Construct a watcher and capture its initial state
             *
             * \param driver Driver that owns the observed directory
             * \param root Directory node to observe
             * \param recursive Whether descendant directories are observed
             */
            MemoryDirectoryWatcher(MemoryFileSystemDriver& driver, SharedPtr<MemoryFileSystemNode> root, bool recursive);

            void PollEvents() override;

            Signal<Path>& OnFileCreated() override;
            Signal<Path>& OnFileModified() override;
            Signal<Path, Path>& OnFileRenamed() override;
            Signal<Path>& OnFileDeleted() override;

        private:

            /**
             * \brief Information retained for an entry between polls
             */
            struct SnapshotEntry
            {
                SharedPtr<MemoryFileSystemNode> Node;
                PathType Type;
                Uint64 ContentRevision;
            };

            using Snapshot = HashMap<Path, SnapshotEntry>;
            using NodePathMap = HashMap<const MemoryFileSystemNode*, Path>;

            /**
             * \brief Capture the entries currently visible to this watcher
             *
             * \return Snapshot of the watched directory scope
             */
            Snapshot TakeSnapshot() const;

            /**
             * \brief Add the children of a directory to a snapshot
             *
             * \param directory Directory whose children are collected
             * \param snapshot Snapshot being built
             */
            void CollectChildren(const SharedPtr<MemoryFileSystemNode>& directory, Snapshot& snapshot) const;

            MemoryFileSystemDriver* mDriver;
            SharedPtr<MemoryFileSystemNode> mRoot;
            bool mRecursive;
            Snapshot mSnapshot;
            Signal<Path> mOnFileCreated;
            Signal<Path> mOnFileModified;
            Signal<Path, Path> mOnFileRenamed;
            Signal<Path> mOnFileDeleted;
        };
    }
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYDIRECTORYWATCHER_HPP
