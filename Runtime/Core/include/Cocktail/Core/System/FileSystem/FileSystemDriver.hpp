#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP

#include <Cocktail/Core/Memory/UniquePtr.hpp>
#include <Cocktail/Core/System/FileSystem/Directory.hpp>
#include <Cocktail/Core/System/FileSystem/DirectoryIterator.hpp>
#include <Cocktail/Core/System/FileSystem/File.hpp>
#include <Cocktail/Core/System/FileSystem/FileOpenFlags.hpp>
#include <Cocktail/Core/System/FileSystem/PathInfo.hpp>

namespace Ck
{
    /**
     * \brief Options used when copying a file
     *
     * Defines behavior for file copy operations such as overwriting existing files
     * and preserving metadata.
     */
    struct FileCopyOptions
    {
        /**
         * \brief Allow overwriting the destination file if it already exists
         */
        bool Overwrite = false;

        /**
         * \brief Preserve file metadata (timestamps, attributes, etc.)
         *
         * \note The exact metadata preserved depends on the underlying filesystem.
         */
        bool PreserveMetadata = true;
    };

    /**
     * \brief Options used when copying a directory
     *
     * Extends file copy options with directory-specific behavior such as recursion
     * and structure-only copying.
     */
    struct DirectoryCopyOptions : FileCopyOptions
    {
        /**
         * \brief Recursively copy subdirectories
         *
         * If false, only the top-level directory is copied.
         */
        bool Recursive = false;

        /**
         * \brief Copy only the directory structure without file contents
         *
         * When enabled, directories are created but files are not copied.
         */
        bool OnlyStructure = false;
    };

    /**
     * \brief Options used when moving a file
     */
    struct FileMoveOptions : FileCopyOptions
    {
        /**
         * \brief Allow fallback to copy + delete when moving across filesystems
         *
         * \note Some platforms cannot move files across different volumes atomically.
         *       When enabled, the implementation may perform a copy followed by a deletion.
         */
        bool AllowCopyFallback = true;
    };

    /**
     * \brief Options used when moving a directory
     */
    struct DirectoryMoveOptions : FileMoveOptions
    {
        /**
         * \brief Recursively move directory contents when fallback copy is used
         *
         * \note Ignored if the move is handled natively by the filesystem.
         */
        bool Recursive = true;

        /**
         * \brief Move only the directory structure without file contents
         *
         * \note Only applies when fallback copy is used.
         */
        bool OnlyStructure = false;
    };

    /**
     * \brief Options used when removing a file
     */
    struct FileRemoveOptions
    {
        /**
         * \brief Force removal even if the file is read-only or protected
         *
         * \note The exact behavior depends on the platform and may involve
         *       modifying file attributes before deletion.
         */
        bool Force = false;

        /**
         * \brief Ignore missing files instead of failing
         */
        bool IgnoreMissing = true;
    };

    /**
     * \brief Options used when removing a directory
     *
     * Extends file removal options with directory-specific behavior.
     */
    struct DirectoryRemoveOptions : FileRemoveOptions
    {
        /**
         * \brief Recursively remove directory contents
         *
         * If false, the operation will fail if the directory is not empty.
         */
        bool Recursive = false;
    };

    /**
     * \brief Abstract interface representing a filesystem driver
     *
     * FileSystemDriver defines a generic interface for interacting with a filesystem.
     * It abstracts operations such as file and directory creation, deletion,
     * copying, moving, and metadata retrieval.
     *
     * Implementations may represent local filesystems, virtual filesystems,
     * archives, or remote storage systems.
     */
    class FileSystemDriver
    {
    public:

        /**
         * \brief Destructor
         */
        virtual ~FileSystemDriver() = default;

        /**
         * \brief Check if a path refers to an existing file
         *
         * \param path Path to check
         *
         * \return True if the path exists and is a file, false otherwise
         */
        virtual bool IsFile(const Path& path) const = 0;

        /**
         * \brief Check if a path refers to an existing directory
         *
         * \param path Path to check
         *
         * \return True if the path exists and is a directory, false otherwise
         */
        virtual bool IsDirectory(const Path& path) const = 0;

        /**
         * \brief Create an empty file
         *
         * \param path Path of the file to create
         */
        virtual void CreateFile(const Path& path) = 0;

        /**
         * \brief Create a directory
         *
         * \param path Path of the directory to create
         */
        virtual void CreateDirectory(const Path& path) = 0;

        /**
         * \brief Create a directory iterator
         *
         * \param path The path of the directory
         *
         * return Unique pointer to a DirectoryIterator object
         */
        virtual UniquePtr<DirectoryIterator> CreateDirectoryIterator(const Path& path) = 0;

        /**
         * \brief Open a file with specified access flags
         *
         * \param path Path to the file
         * \param flags File open flags (read, write, append, etc.)
         *
         * \return Unique pointer to a File object
         */
        virtual UniquePtr<File> OpenFile(const Path& path, const FileOpenFlags& flags) = 0;

        /**
         * \brief Open a directory for access
         *
         * \param path Path to the directory
         *
         * \return Unique pointer to a Directory object
         */
        virtual UniquePtr<Directory> OpenDirectory(const Path& path) = 0;

        /**
         * \brief Copy a file
         *
         * \param source Source file path
         * \param destination Destination file path
         * \param options Copy behavior options
         */
        virtual void CopyFile(const Path& source, const Path& destination, const FileCopyOptions& options) = 0;

        /**
         * \brief Copy a directory
         *
         * \param source Source directory path
         * \param destination Destination directory path
         * \param options Copy behavior options
         */
        virtual void CopyDirectory(const Path& source, const Path& destination, const DirectoryCopyOptions& options) = 0;

        /**
         * \brief Move or rename a file or directory
         *
         * \param source Source path
         * \param destination Destination path
         * \param options Move behavior options
         */
        virtual void MoveFile(const Path& source, const Path& destination, const FileMoveOptions& options) = 0;

        /**
         * \brief Move or rename a file or directory
         *
         * \param source Source path
         * \param destination Destination path
         * \param options Move behavior options
         */
        virtual void MoveDirectory(const Path& source, const Path& destination, const DirectoryMoveOptions& options) = 0;

        /**
         * \brief Remove a file
         *
         * \param path Path of the file to remove
         * \param options Removal options
         */
        virtual void RemoveFile(const Path& path, const FileRemoveOptions& options) = 0;

        /**
         * \brief Remove a directory
         *
         * \param path Path of the directory to remove
         * \param options Removal options
         */
        virtual void RemoveDirectory(const Path& path, const DirectoryRemoveOptions& options) = 0;

        /**
         * \brief Convert a path to its canonical form
         *
         * Resolves the path to an absolute and normalized representation.
         *
         * \param path Path to canonicalize
         *
         * \return Canonical path
         */
        virtual Path MakeCanonical(const Path& path) = 0;

        /**
         * \brief Try to convert a path to its canonical form
         *
         * \param path Path to canonicalize
         *
         * \return Optional containing canonical path if successful, empty otherwise
         */
        virtual Optional<Path> TryMakeCanonical(const Path& path) = 0;

        /**
         * \brief Retrieve metadata about a path
         *
         * \param path Path to query
         *
         * \return PathInfo structure describing the entry
         */
        virtual PathInfo GetPathInfo(const Path& path) const = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP
