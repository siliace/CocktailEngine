#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILESYSTEMNODE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILESYSTEMNODE_HPP

#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileContent.hpp>
#include <CocktailEngine/Core/System/FileSystem/Path.hpp>
#include <CocktailEngine/Core/System/FileSystem/PathInfo.hpp>

namespace Ck::Detail
{
    /**
     * \brief Entry of the tree held by a MemoryFileSystemDriver
     *
     * A node is either a file, in which case only Content is meaningful, or a
     * directory, in which case only Children is meaningful.
     *
     * Nodes are owned through shared pointers: the driver holds one reference for as
     * long as the entry is reachable, and every open File or Directory handle holds
     * another one. An entry removed while a handle is still open therefore stays
     * readable through that handle.
     *
     * \see MemoryFileSystemDriver
     */
    struct MemoryFileSystemNode
    {
        /**
         * \brief Path of the entry, normalized and relative to the root of the virtual filesystem
         */
        Path NodePath;

        /**
         * \brief Kind of entry
         */
        PathType Type = PathType::None;

        /**
         * \brief Bytes held by the entry
         *
         * \note Only meaningful when Type is PathType::File
         */
        MemoryFileContent Content;

        /**
         * \brief Names of the direct children of the entry
         *
         * \note Only meaningful when Type is PathType::Directory
         */
        Array<String> Children;

        /**
         * \brief Instant the entry was created at
         */
        Instant CreationTime;

        /**
         * \brief Instant the entry was modified at for the last time
         */
        Instant LastChangeTime;

        /**
         * \brief Instant the entry was accessed at for the last time
         */
        Instant LastAccessTime;

        /**
         * \brief Stamp the entry as freshly created
         *
         * \param instant The instant to use for every timestamp
         */
        void Touch(const Instant& instant)
        {
            CreationTime = instant;
            LastChangeTime = instant;
            LastAccessTime = instant;
        }

        /**
         * \brief Build the metadata describing the entry
         *
         * \return The metadata of the entry
         */
        PathInfo GetPathInfo() const
        {
            PathInfo pathInfo;
            pathInfo.Type = Type;
            pathInfo.Size = Type == PathType::File ? Content.GetSize() : 0;
            pathInfo.CreationTime = CreationTime;
            pathInfo.LastChangeTime = LastChangeTime;
            pathInfo.LastAccessTime = LastAccessTime;

            return pathInfo;
        }
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILESYSTEMNODE_HPP
