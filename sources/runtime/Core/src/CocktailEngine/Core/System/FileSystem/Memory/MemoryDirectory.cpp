#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryDirectory.hpp>

namespace Ck::Detail
{
    MemoryDirectory::MemoryDirectory(SharedPtr<MemoryFileSystemNode> node) :
        mNode(Move(node))
    {
        assert(mNode.Get() != nullptr);
        assert(mNode->Type == PathType::Directory);
    }

    Array<Path> MemoryDirectory::GetContent() const
    {
        mNode->LastAccessTime = Instant::Now();

        Array<Path> content;
        content.Reserve(mNode->Children.GetSize());

        for (const String& child : mNode->Children)
            content.Emplace(child);

        return content;
    }

    const Path& MemoryDirectory::GetPath() const
    {
        return mNode->NodePath;
    }

    void* MemoryDirectory::GetSystemHandle() const
    {
        return nullptr;
    }
}
