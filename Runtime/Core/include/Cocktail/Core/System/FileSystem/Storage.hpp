#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGE_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/System/FileSystem/StorageService.hpp>

namespace Ck
{
    /**
     * \class Storage
     * \brief Static facade providing global access to the StorageService
     *
     * Storage is a convenience static interface that forwards all filesystem
     * operations to the underlying StorageService instance associated with
     * the current application context.
     *
     * It allows accessing filesystem features without explicitly managing
     * a StorageService instance, simplifying usage in most application code.
     *
     * All operations support URI-based resource addressing and are delegated
     * to the appropriate FileSystemDriver depending on the URI scheme.
     *
     * \note This class relies on ServiceFacade to retrieve the active
     *       StorageService instance from the application.
     *
     * \see StorageService
     * \see URI
     * \see FileSystemDriver
     * \see File
     * \see Directory
     */
    class COCKTAIL_CORE_API Storage : public ServiceFacade<StorageService>
    {
    public:

        /**
         * \brief Checks whether the given URI refers to an existing file
         *
         * Delegates the call to the underlying StorageService.
         *
         * \param uri Resource location to check
         *
         * \return True if a file exists at the given location, otherwise false
         */
        static bool IsFile(const URI& uri);

        /**
         * \brief Checks whether the given URI refers to an existing directory
         *
         * \param uri Resource location to check
         *
         * \return True if a directory exists at the given location, otherwise false
         */
        static bool IsDirectory(const URI& uri);

        /**
         * \brief Creates an empty file at the given URI
         *
         * \param uri Location where the file should be created
         */
        static void CreateFile(const URI& uri);

        /**
         * \brief Creates a directory at the given URI
         *
         * Parent directories may be created depending on the backend implementation.
         *
         * \param uri Directory location to create
         */
        static void CreateDirectory(const URI& uri);

        /**
         * \brief Create a directory iterator
         *
         * \param uri The uri of the directory
         *
         * return Unique pointer to a DirectoryIterator object
         */
        static UniquePtr<DirectoryIterator> CreateDirectoryIterator(const URI& uri);

        /**
         * \brief Opens a file for reading and/or writing
         *
         * \param uri Resource location to open
         * \param flags Specifies the access mode (read, write, truncate, etc.)
         *
         * \return A unique pointer to a File interface, or nullptr on failure
         */
        static UniquePtr<File> OpenFile(const URI& uri, FileOpenFlags flags);

        /**
         * \brief Opens a directory for enumeration
         *
         * \param uri Directory location to open
         *
         * \return A unique pointer to a Directory interface, or nullptr if not found
         */
        static UniquePtr<Directory> OpenDirectory(const URI& uri);

        /**
         * \brief Copies a file from a source URI to a destination URI
         *
         * \param source URI of the file to copy
         * \param destination Target location
         * \param options Copy behavior options
         */
        static void CopyFile(const URI& source, const URI& destination, const FileCopyOptions& options);

        /**
         * \brief Copies a directory from a source URI to a destination URI
         *
         * Behavior such as recursion, overwriting, and metadata preservation
         * is controlled by \p options.
         *
         * \param source URI of the directory to copy
         * \param destination Target directory location
         * \param options Copy behavior options
         */
        static void CopyDirectory(const URI& source, const URI& destination, const DirectoryCopyOptions& options);

        /**
         * \brief Moves (renames) a file from a source URI to a destination URI
         *
         * After a successful move, the source file no longer exists.
         *
         * \param source Source file location
         * \param destination Destination location
         * \param options Move behavior options
         */
        static void MoveFile(const URI& source, const URI& destination, const FileMoveOptions& options);

        /**
         * \brief Moves (renames) a directory from a source URI to a destination URI
         *
         * The operation may be performed atomically or fallback to a copy + remove
         * strategy depending on the underlying filesystem.
         *
         * \param source Source directory location
         * \param destination Destination location
         * \param options Move behavior options
         */
        static void MoveDirectory(const URI& source, const URI& destination, const DirectoryMoveOptions& options);

        /**
         * \brief Removes a file at the given URI
         *
         * \param uri Location of the file to delete
         * \param options Removal behavior options
         */
        static void RemoveFile(const URI& uri, const FileRemoveOptions& options);

        /**
         * \brief Removes a directory at the given URI
         *
         * \param uri Location of the directory to delete
         * \param options Removal behavior options
         */
        static void RemoveDirectory(const URI& uri, const DirectoryRemoveOptions& options);

        /**
         * \brief Registers a filesystem driver for a URI scheme
         *
         * The StorageService takes ownership of the driver.
         *
         * \param protocol URI scheme (e.g. "file", "asset")
         * \param fileSystemDriver Driver instance responsible for this scheme
         */
        static void Mount(String protocol, UniquePtr<FileSystemDriver> fileSystemDriver);

        /**
         * \brief Registers an external filesystem driver for a URI scheme
         *
         * The caller retains ownership of the driver.
         *
         * \param protocol URI scheme (e.g. "file", "asset")
         * \param fileSystemDriver Driver instance responsible for this scheme
         */
        static void MountExternal(String protocol, FileSystemDriver* fileSystemDriver);

        /**
         * \brief Unregisters a filesystem driver associated with a scheme
         *
         * \param protocol The previously registered URI scheme
         */
        static void UnMount(const String& protocol);

        /**
         * \brief Resolves the filesystem driver associated with a scheme
         *
         * \param scheme URI scheme to resolve
         *
         * \return Pointer to the associated FileSystemDriver, or nullptr if not found
         */
        static FileSystemDriver* ResolveDriver(const String& scheme);
    };
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGE_HPP
