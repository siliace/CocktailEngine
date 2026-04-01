#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP

#include <Cocktail/Core/Memory/UniquePtr.hpp>
#include <Cocktail/Core/System/FileSystem/Directory.hpp>
#include <Cocktail/Core/System/FileSystem/File.hpp>
#include <Cocktail/Core/System/FileSystem/FileOpenFlags.hpp>
#include <Cocktail/Core/Utility/Time/Instant.hpp>

namespace Ck
{
    /**
     * \brief Enumeration of type of filesystem entries
     */
    enum class PathType
    {
        /**
         * \brief The path does not exist
         */
        None,

        /**
         * \brief The path is a normal file
         */
        File,

        /**
         * \brief The path is a directory
         */
        Directory,

        /**
         * \brief The path is something else (e.g., symlink, device file)
         */
        Other
    };

    /**
     * \brief Structure containing information about a Path on a filesystem
     */
    struct PathInfo
    {
        /**
         * \brief The type of the path (None, File, Directory, Other)
         */
        PathType Type = PathType::None;

        /**
         * \brief The size in bytes of the entry
         *
         * \note This value is used only if the path is a file. For directories or other types, this may be zero or undefined.
         */
        Uint64 Size = 0;

        /**
         * \brief The creation timestamp of the entry
         *
         * \note This value is used only if the path is a file. May be invalid or empty for directories.
         */
        Instant CreationTime;

        /**
         * \brief The last modification timestamp of the entry
         *
         * \note This value is used only if the path is a file. May be invalid or empty for directories.
         */
        Instant LastChangeTime;

        /**
         * \brief The last access timestamp of the entry
         *
         * \note This value is used only if the path is a file. May be invalid or empty for directories.
         */
        Instant LastAccessTime;
    };

    /**
     * \brief Abstract interface representing a filesystem driver
     *
     * This interface provides operations for working with files and directories in a filesystem.
     */
    class FileSystemDriver
    {
    public:

        /**
         * \brief Destructor
         */
        virtual ~FileSystemDriver() = default;

        /**
         * \brief Checks if a path exists and is a regular file
         *
         * \param path The path to check
         *
         * \return True if the path exists and is a file, false otherwise
         */
        virtual bool IsFile(const Path& path) const = 0;

        /**
         * \brief Checks if a path exists and is a directory
         *
         * \param path The path to check
         *
         * \return True if the path exists and is a directory, false otherwise
         */
        virtual bool IsDirectory(const Path& path) const = 0;

        /**
         * \brief Creates an empty file at the specified path
         *
         * \param path The path where the file should be created
         */
        virtual void CreateFile(const Path& path) = 0;

        /**
         * \brief Creates a directory at the specified path
         *
         * \param path The path where the directory should be created
         */
        virtual void CreateDirectory(const Path& path) = 0;

        /**
         * \brief Opens a file for reading and/or writing
         *
         * \param path The path to the file
         * \param flags Flags controlling how the file should be opened (read, write, append, etc.)
         *
         * \return A unique pointer to a File object representing the opened file
         */
        virtual UniquePtr<File> OpenFile(const Path& path, const FileOpenFlags& flags) = 0;

        /**
         * \brief Opens a directory for iteration
         *
         * \param path The path to the directory
         *
         * \return A unique pointer to a Directory object representing the opened directory
         */
        virtual UniquePtr<Directory> OpenDirectory(const Path& path) = 0;

        /**
         * \brief Copies a file or directory to a new location
         *
         * \param source The path to the source file or directory
         * \param destination The path to copy to
         * \param failIfExists If true, operation fails if the destination already exists
         */
        virtual void Copy(const Path& source, const Path& destination, bool failIfExists = false) = 0;

        /**
         * \brief Moves (renames) a file or directory to a new location
         *
         * \param source The path to the source file or directory
         * \param destination The path to move to
         */
        virtual void Move(const Path& source, const Path& destination) = 0;

        /**
         * \brief Removes a file or directory
         *
         * \param path The path to remove
         */
        virtual void Remove(const Path& path) = 0;

        /**
         * \brief Converts a path to its canonical (absolute and normalized) form
         *
         * \param path The path to canonicalize
         * \return The canonicalized path
         */
        virtual Path MakeCanonical(const Path& path) = 0;

        /**
         * \brief Attempts to convert a path to its canonical form
         *
         * \param path The path to canonicalize
         *
         * \return Optional containing the canonical path if successful, empty if the path does not exist or cannot be canonicalized
         */
        virtual Optional<Path> TryMakeCanonical(const Path& path) = 0;

        /**
         * \brief Retrieves metadata for a path
         *
         * \param path The path to query
         *
         * \return A PathInfo structure containing type, size, and timestamps
         */
        virtual PathInfo GetPathInfo(const Path& path) const = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP
