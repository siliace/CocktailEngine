#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryDirectoryWatcher.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemDriver.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemNode.hpp>

namespace Ck::Detail
{
    MemoryDirectoryWatcher::MemoryDirectoryWatcher(MemoryFileSystemDriver& driver, SharedPtr<MemoryFileSystemNode> root, bool recursive) :
        mDriver(&driver),
        mRoot(Move(root)),
        mRecursive(recursive),
        mSnapshot(TakeSnapshot())
    {
        /// Nothing
    }

    void MemoryDirectoryWatcher::PollEvents()
    {
        Snapshot current = TakeSnapshot();
        NodePathMap previousPaths;
        NodePathMap currentPaths;

        mSnapshot.ForEach([&previousPaths](const SnapshotEntry& entry, const Path& path) {
            previousPaths.Put(entry.Node.Get(), path);
        });

        current.ForEach([&currentPaths](const SnapshotEntry& entry, const Path& path) {
            currentPaths.Put(entry.Node.Get(), path);
        });

        mSnapshot.ForEach([this, &currentPaths](const SnapshotEntry& entry, const Path& path) {
            if (!currentPaths.Contains(entry.Node.Get()))
                mOnFileDeleted.Emit(path);
        });

        mSnapshot.ForEach([this, &currentPaths](const SnapshotEntry& entry, const Path& oldPath) {
            const Path* newPath = currentPaths.Get(entry.Node.Get());
            if (newPath != nullptr && *newPath != oldPath)
                mOnFileRenamed.Emit(oldPath, *newPath);
        });

        current.ForEach([this, &previousPaths](const SnapshotEntry& entry, const Path& path) {
            if (!previousPaths.Contains(entry.Node.Get()))
                mOnFileCreated.Emit(path);
        });

        mSnapshot.ForEach([this, &current, &currentPaths](const SnapshotEntry& entry, const Path& path) {
            const Path* currentPath = currentPaths.Get(entry.Node.Get());
            if (currentPath == nullptr || *currentPath != path || entry.Type != PathType::File)
                return;

            const SnapshotEntry* currentEntry = current.Get(path);
            if (currentEntry != nullptr && currentEntry->ContentRevision != entry.ContentRevision)
                mOnFileModified.Emit(path);
        });

        mSnapshot = Move(current);
    }

    Signal<Path>& MemoryDirectoryWatcher::OnFileCreated()
    {
        return mOnFileCreated;
    }

    Signal<Path>& MemoryDirectoryWatcher::OnFileModified()
    {
        return mOnFileModified;
    }

    Signal<Path, Path>& MemoryDirectoryWatcher::OnFileRenamed()
    {
        return mOnFileRenamed;
    }

    Signal<Path>& MemoryDirectoryWatcher::OnFileDeleted()
    {
        return mOnFileDeleted;
    }

    MemoryDirectoryWatcher::Snapshot MemoryDirectoryWatcher::TakeSnapshot() const
    {
        Snapshot snapshot;

        if (mRoot && mRoot->Type == PathType::Directory && mDriver->FindNode(mRoot->NodePath).Get() == mRoot.Get())
            CollectChildren(mRoot, snapshot);

        return snapshot;
    }

    void MemoryDirectoryWatcher::CollectChildren(const SharedPtr<MemoryFileSystemNode>& directory, Snapshot& snapshot) const
    {
        for (const SharedPtr<MemoryFileSystemNode>& node : directory->Children)
        {
            SnapshotEntry entry;
            entry.Node = node;
            entry.Type = node->Type;
            entry.ContentRevision = node->Type == PathType::File ? node->Content.GetRevision() : 0;

            snapshot.Put(node->NodePath, Move(entry));

            if (mRecursive && node->Type == PathType::Directory)
                CollectChildren(node, snapshot);
        }
    }
}

namespace Ck
{
    UniquePtr<DirectoryWatcher> MemoryFileSystemDriver::CreateWatcher(Directory& directory, bool recursive)
    {
        return MakeUnique<Detail::MemoryDirectoryWatcher>(*this, RequireDirectory(directory.GetPath()), recursive);
    }
}
