#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_STORAGE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_STORAGE_HPP

#include <CocktailEngine/Core/Application/ServiceFacade.hpp>
#include <CocktailEngine/Core/System/FileSystem/StorageService.hpp>

namespace Ck
{
    /**
     * \class Storage
     * \brief Static facade for the application StorageService.
     *
     * Storage forwards URI-addressed filesystem operations to the active
     * \ref StorageService. Driver selection, default-scheme fallback and ownership
     * rules are defined by that service.
     *
     * \note This facade requires an active StorageService in the application context.
     *
     * \see StorageService
     * \see URI
     * \see FileSystemDriver
     */
    class COCKTAILENGINE_CORE_API Storage : public ServiceFacade<StorageService>
    {
    public:

        /**
         * \brief Checks whether the given URI refers to an existing file.
         *
         * \param uri Resource URI to check.
         *
         * \return True when the resolved driver reports a file at the URI path.
         */
        static bool IsFile(const URI& uri);

        /**
         * \brief Checks whether the given URI refers to an existing directory.
         *
         * \param uri Resource URI to check.
         *
         * \return True when the resolved driver reports a directory at the URI path.
         */
        static bool IsDirectory(const URI& uri);

        /**
         * \brief Creates an empty file at the given URI.
         *
         * \param uri URI of the file to create.
         */
        static void CreateFile(const URI& uri);

        /**
         * \brief Creates one directory at the given URI.
         *
         * Parent-directory creation is defined by the resolved driver.
         *
         * \param uri URI of the directory to create.
         */
        static void CreateDirectory(const URI& uri);

        /**
         * \brief Creates an iterator for a URI-addressed directory.
         *
         * \param uri URI of the directory to enumerate.
         *
         * \return The iterator supplied by the resolved driver.
         */
        static UniquePtr<DirectoryIterator> CreateDirectoryIterator(const URI& uri);

        /**
         * \brief Opens a URI-addressed file.
         *
         * \param uri URI of the file to open.
         * \param flags Requested access and open behaviour.
         *
         * \return The file handle supplied by the resolved driver.
         */
        static UniquePtr<File> OpenFile(const URI& uri, FileOpenFlags flags);

        /**
         * \brief Opens a URI-addressed directory.
         *
         * \param uri URI of the directory to open.
         *
         * \return The directory handle supplied by the resolved driver.
         */
        static UniquePtr<Directory> OpenDirectory(const URI& uri);

        /**
         * \brief Copies a file between URI-addressed locations.
         *
         * \param source URI of the source file.
         * \param destination URI of the target file.
         * \param options Copy behaviour options.
         */
        static void CopyFile(const URI& source, const URI& destination, const FileCopyOptions& options);

        /**
         * \brief Copies a directory between URI-addressed locations.
         *
         * \param source URI of the source directory.
         * \param destination URI of the target directory.
         * \param options Copy behaviour options.
         */
        static void CopyDirectory(const URI& source, const URI& destination, const DirectoryCopyOptions& options);

        /**
         * \brief Moves a file between URI-addressed locations.
         *
         * \param source URI of the source file.
         * \param destination URI of the target file.
         * \param options Move behaviour options.
         */
        static void MoveFile(const URI& source, const URI& destination, const FileMoveOptions& options);

        /**
         * \brief Moves a directory between URI-addressed locations.
         *
         * \param source URI of the source directory.
         * \param destination URI of the target directory.
         * \param options Move behaviour options.
         */
        static void MoveDirectory(const URI& source, const URI& destination, const DirectoryMoveOptions& options);

        /**
         * \brief Removes a URI-addressed file.
         *
         * \param uri URI of the file to remove.
         * \param options Removal behaviour options.
         */
        static void RemoveFile(const URI& uri, const FileRemoveOptions& options);

        /**
         * \brief Removes a URI-addressed directory.
         *
         * \param uri URI of the directory to remove.
         * \param options Removal behaviour options.
         */
        static void RemoveDirectory(const URI& uri, const DirectoryRemoveOptions& options);

        /**
         * \brief Mounts and transfers ownership of a driver for a scheme.
         *
         * \param scheme URI scheme to mount, such as "file" or "asset".
         * \param fileSystemDriver Non-null driver owned by the active StorageService.
         */
        static void Mount(String scheme, UniquePtr<FileSystemDriver> fileSystemDriver);

        /**
         * \brief Mounts a non-owning driver for a scheme.
         *
         * The caller must keep \p fileSystemDriver alive while the scheme remains
         * mounted and while a resolved reference to it is in use.
         *
         * \param scheme URI scheme to mount, such as "file" or "asset".
         * \param fileSystemDriver Non-null driver retained by the caller.
         */
        static void MountExternal(String scheme, FileSystemDriver* fileSystemDriver);

        /**
         * \brief Removes the active driver resolution for a scheme.
         *
         * \param scheme Scheme to unmount.
         */
        static void UnMount(const String& scheme);

        /**
         * \brief Resolves the driver responsible for an URI.
         *
         * The active StorageService selects the URI scheme, or its default scheme
         * when the URI has none. The returned reference is borrowed.
         *
         * \param uri URI whose scheme selects the driver.
         *
         * \return The resolved non-null driver.
         *
         * \throws NoSuchDriverException When no active driver is mounted for the
         *         explicit or default scheme.
         */
        static FileSystemDriver& ResolveDriver(const URI& uri);

        /**
         * \brief Attempts to resolve the driver responsible for an URI.
         *
         * The active StorageService selects the URI scheme, or its default scheme
         * when the URI has none.
         *
         * \param uri URI whose scheme selects the driver.
         *
         * \return The resolved borrowed driver, or an empty optional when no active
         *         driver is mounted for the explicit or default scheme.
         */
        static Optional<FileSystemDriver&> TryResolveDriver(const URI& uri);

        /**
         * \brief Gets the driver mounted under an exact scheme.
         *
         * This performs no URI parsing and does not fall back to the default scheme.
         * The returned reference is borrowed from the active StorageService.
         *
         * \param scheme Exact scheme key to look up.
         *
         * \return The non-null driver currently mounted under \p scheme.
         *
         * \throws NoSuchDriverException When no active driver is mounted under
         *         \p scheme.
         */
        static FileSystemDriver& GetDriver(const String& scheme);

        /**
         * \brief Attempts to get the driver mounted under an exact scheme.
         *
         * This performs no URI parsing and does not fall back to the default scheme.
         * A non-empty result contains a borrowed reference from the active
         * StorageService.
         *
         * \param scheme Exact scheme key to look up.
         *
         * \return The mounted driver, or an empty optional when \p scheme has no
         *         active driver.
         */
        static Optional<FileSystemDriver&> TryGetDriver(const String& scheme);
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_STORAGE_HPP
