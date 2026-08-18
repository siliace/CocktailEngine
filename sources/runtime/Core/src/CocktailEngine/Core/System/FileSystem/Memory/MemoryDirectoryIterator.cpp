#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryDirectoryIterator.hpp>

namespace Ck::Detail
{
    MemoryDirectoryIterator::MemoryDirectoryIterator(EntryArray entries) :
        mEntries(Move(entries)),
        mIndex(0)
    {
        /// Nothing
    }

    void MemoryDirectoryIterator::Next()
    {
        if (IsEnd())
            return;

        mIndex++;
    }

    const Path& MemoryDirectoryIterator::GetPath() const
    {
        return IsEnd() ? Path::Empty : mEntries[mIndex]->NodePath;
    }

    PathInfo MemoryDirectoryIterator::GetPathInfo() const
    {
        if (IsEnd())
            return {};

        return mEntries[mIndex]->GetPathInfo();
    }

    bool MemoryDirectoryIterator::IsEnd() const
    {
        return mIndex >= mEntries.GetSize();
    }
}
