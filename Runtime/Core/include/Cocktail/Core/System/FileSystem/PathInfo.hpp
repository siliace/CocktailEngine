#ifndef COCKTAILENGINE_PATHINFO_HPP
#define COCKTAILENGINE_PATHINFO_HPP

#include <Cocktail/Core/Utility/Time/Instant.hpp>

namespace Ck
{
    /**
     * \brief Enumeration of type of filesystem entries
     *
     * Represents the kind of object a Path refers to in a filesystem.
     */
    enum class PathType
    {
        /**
         * \brief The path does not exist
         */
        None,

        /**
         * \brief The path is a regular file
         */
        File,

        /**
         * \brief The path is a directory
         */
        Directory,

        /**
         * \brief The path is another type (symbolic link, device, socket, etc.)
         */
        Other
    };

    /**
     * \brief Structure containing metadata about a filesystem path
     *
     * PathInfo provides information about a filesystem entry such as its type,
     * size, and timestamps. Some fields may be invalid depending on the type
     * of the entry and the underlying filesystem.
     */
    struct PathInfo
    {
        /**
         * \brief Type of the filesystem entry
         */
        PathType Type = PathType::None;

        /**
         * \brief Size of the entry in bytes
         *
         * \note Only meaningful for regular files. For directories or other types,
         *       the value may be zero or undefined.
         */
        Uint64 Size = 0;

        /**
         * \brief Creation timestamp
         *
         * \note May not be available on all platforms or for all entry types.
         */
        Instant CreationTime;

        /**
         * \brief Last modification timestamp
         */
        Instant LastChangeTime;

        /**
         * \brief Last access timestamp
         */
        Instant LastAccessTime;
    };
}

#endif // COCKTAILENGINE_PATHINFO_HPP
