#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_STORAGESERVICE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_STORAGESERVICE_HPP

#include <CocktailEngine/Core/HashMap.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileSystemDriver.hpp>
#include <CocktailEngine/Core/System/FileSystem/URI.hpp>
#include <CocktailEngine/Core/Utility/Optional.hpp>

namespace Ck
{
    COCKTAIL_DECLARE_EXCEPTION_FROM(NoSuchDriverException, RuntimeException);

    /**
     * \class StorageService
     * \brief Routes URI-addressed filesystem operations to mounted drivers.
     *
     * Each operation resolves the URI scheme to an active \ref FileSystemDriver.
     * URIs without a scheme use the default scheme supplied to the constructor.
     * Drivers can be owned by the service through \ref Mount or borrowed through
     * \ref MountExternal.
     *
     * Operations between two URIs use the native driver operation when both URIs
     * resolve to the same driver object, even when their schemes differ. Operations
     * between distinct drivers use the service's generic copy or copy-and-remove
     * fallback where supported.
     *
     * \see URI
     * \see FileSystemDriver
     * \see Storage
     */
    class COCKTAILENGINE_CORE_API StorageService
    {
    public:

        /**
         * \brief Creates a storage service with a default URI scheme.
         *
         * \param defaultScheme Scheme selected when resolving an URI without an
         *        explicit scheme. It must not be empty.
         */
        explicit StorageService(String defaultScheme = CK_TEXT("file"));

        StorageService(const StorageService& other) = delete;
        StorageService(StorageService&& other) = default;
        StorageService& operator=(const StorageService& other) = delete;
        StorageService& operator=(StorageService&& other) = default;

        /**
         * \brief Checks whether an URI identifies an existing file.
         *
         * \param uri Resource URI to check.
         *
         * \return True when the resolved driver reports a file at the URI path.
         *
         * \throws NoSuchDriverException When no driver is mounted for the URI
         *         scheme or its default fallback.
         */
        bool IsFile(const URI& uri);

        /**
         * \brief Checks whether an URI identifies an existing directory.
         *
         * \param uri Resource URI to check.
         *
         * \return True when the resolved driver reports a directory at the URI path.
         *
         * \throws NoSuchDriverException When no driver is mounted for the URI
         *         scheme or its default fallback.
         */
        bool IsDirectory(const URI& uri);

        /**
         * \brief Creates an empty file at an URI.
         *
         * Existing-file behaviour and parent-directory creation are defined by the
         * resolved driver.
         *
         * \param uri URI of the file to create.
         *
         * \throws NoSuchDriverException When no driver can resolve \p uri.
         */
        void CreateFile(const URI& uri);

        /**
         * \brief Creates one directory at an URI.
         *
         * This method delegates directly to the resolved driver; it does not create
         * missing parent directories itself.
         *
         * \param uri URI of the directory to create.
         *
         * \throws NoSuchDriverException When no driver can resolve \p uri.
         */
        void CreateDirectory(const URI& uri);

        /**
         * \brief Creates an iterator for an URI-addressed directory.
         *
         * \param uri URI of the directory to enumerate.
         *
         * \return A directory iterator supplied by the resolved driver, or null when
         *         the driver cannot create one for \p uri.
         *
         * \throws NoSuchDriverException When no driver can resolve \p uri.
         */
        UniquePtr<DirectoryIterator> CreateDirectoryIterator(const URI& uri);

        /**
         * \brief Opens an URI-addressed file.
         *
         * \param uri URI of the file to open.
         * \param flags Requested access and open behaviour.
         *
         * \return The file handle supplied by the resolved driver.
         *
         * \throws NoSuchDriverException When no driver can resolve \p uri.
         */
        UniquePtr<File> OpenFile(const URI& uri, FileOpenFlags flags);

        /**
         * \brief Opens an URI-addressed directory.
         *
         * \param uri URI of the directory to open.
         *
         * \return The directory handle supplied by the resolved driver.
         *
         * \throws NoSuchDriverException When no driver can resolve \p uri.
         */
        UniquePtr<Directory> OpenDirectory(const URI& uri);

        /**
         * \brief Copies a file between URI-addressed locations.
         *
         * When source and destination resolve to the same driver object, the native
         * driver copy operation is used. Otherwise the service transfers the bytes
         * through streams. The generic fallback honours \p options.Overwrite but
         * does not guarantee metadata preservation or creation of a missing
         * destination parent directory.
         *
         * \param source URI of the source file.
         * \param destination URI of the target file.
         * \param options Copy behaviour options.
         *
         * \throws NoSuchDriverException When either URI cannot be resolved.
         */
        void CopyFile(const URI& source, const URI& destination, const FileCopyOptions& options);

        /**
         * \brief Copies a directory between URI-addressed locations.
         *
         * When both URIs resolve to the same driver object, the native driver
         * operation is used. Otherwise the service creates the destination hierarchy
         * and transfers entries according to \p options. The generic fallback does
         * not guarantee metadata preservation.
         *
         * \param source URI of the source directory.
         * \param destination URI of the target directory.
         * \param options Copy behaviour options.
         *
         * \throws NoSuchDriverException When either URI cannot be resolved.
         */
        void CopyDirectory(const URI& source, const URI& destination, const DirectoryCopyOptions& options);

        /**
         * \brief Moves a file between URI-addressed locations.
         *
         * When both URIs resolve to the same driver object, the native driver move
         * operation is used. Otherwise, a copy followed by source removal is used
         * only when \p options.AllowCopyFallback is true. Source removal happens
         * only after a successful copy.
         *
         * \param source URI of the source file.
         * \param destination URI of the target file.
         * \param options Move behaviour options.
         *
         * \throws CrossDeviceLinkException When an inter-driver move disallows the
         *         copy fallback.
         * \throws NoSuchDriverException When either URI cannot be resolved.
         */
        void MoveFile(const URI& source, const URI& destination, const FileMoveOptions& options);

        /**
         * \brief Moves a directory between URI-addressed locations.
         *
         * When both URIs resolve to the same driver object, the native driver move
         * operation is used. The inter-driver fallback copies then removes the
         * source; it requires \p options.AllowCopyFallback and \p options.Recursive
         * to be true, \p options.OnlyStructure to be false, and the source path not
         * to designate a driver root.
         *
         * \param source URI of the source directory.
         * \param destination URI of the target directory.
         * \param options Move behaviour options.
         *
         * \throws CrossDeviceLinkException When an inter-driver move disallows the
         *         copy fallback.
         * \throws InvalidParameterException When the inter-driver fallback options
         *         are incompatible or the source designates a driver root.
         * \throws NoSuchDriverException When either URI cannot be resolved.
         */
        void MoveDirectory(const URI& source, const URI& destination, const DirectoryMoveOptions& options);

        /**
         * \brief Removes an URI-addressed file.
         *
         * \param uri URI of the file to remove.
         * \param options Removal behaviour options.
         *
         * \throws NoSuchDriverException When no driver can resolve \p uri.
         */
        void RemoveFile(const URI& uri, const FileRemoveOptions& options);

        /**
         * \brief Removes an URI-addressed directory.
         *
         * \param uri URI of the directory to remove.
         * \param options Removal behaviour options.
         *
         * \throws NoSuchDriverException When no driver can resolve \p uri.
         */
        void RemoveDirectory(const URI& uri, const DirectoryRemoveOptions& options);

        /**
         * \brief Mounts and retains a filesystem driver for a scheme.
         *
         * The service takes ownership of \p fileSystemDriver and records it as the
         * active driver for \p scheme. Mounting another driver under the same scheme
         * replaces only the active resolution entry; internally owned drivers remain
         * retained until the service is destroyed.
         *
         * \param scheme URI scheme to mount, such as "file" or "asset".
         * \param fileSystemDriver Non-null driver owned by the service.
         */
        void Mount(String scheme, UniquePtr<FileSystemDriver> fileSystemDriver);

        /**
         * \brief Mounts a non-owning filesystem driver for a scheme.
         *
         * The caller retains ownership of \p fileSystemDriver and must keep it alive
         * while the scheme remains mounted and any borrowed driver reference is used.
         * Mounting another driver under the same scheme replaces the active resolution
         * entry.
         *
         * \param scheme URI scheme to mount, such as "file" or "asset".
         * \param fileSystemDriver Non-null driver retained by the caller.
         */
        void MountExternal(String scheme, FileSystemDriver* fileSystemDriver);

        /**
         * \brief Removes the active driver resolution for a scheme.
         *
         * Internally owned drivers are not destroyed by this operation; it only
         * removes the scheme from the active resolution table.
         *
         * \param scheme Scheme to unmount.
         */
        void UnMount(const String& scheme);

        /**
         * \brief Gets the fallback scheme for URIs without an explicit scheme.
         *
         * \return The configured non-empty default scheme.
         */
        const String& GetDefaultScheme() const;

        /**
         * \brief Resolves the driver responsible for an URI.
         *
         * Uses \ref GetDefaultScheme when \p uri has no scheme. The returned
         * reference is borrowed: it remains valid only while the underlying driver
         * remains alive. In particular, externally mounted drivers are owned by the
         * caller.
         *
         * \param uri URI whose scheme selects the driver.
         *
         * \return The resolved non-null driver.
         *
         * \throws NoSuchDriverException When no active driver is mounted for the
         *         explicit or default scheme.
         */
        FileSystemDriver& ResolveDriver(const URI& uri);

        /**
         * \brief Resolves the driver responsible for an URI without mutable access.
         *
         * Applies the same resolution and lifetime rules as the non-const overload.
         *
         * \param uri URI whose scheme selects the driver.
         *
         * \return The resolved non-null driver.
         *
         * \throws NoSuchDriverException When no active driver is mounted for the
         *         explicit or default scheme.
         */
        const FileSystemDriver& ResolveDriver(const URI& uri) const;

        /**
         * \brief Attempts to resolve the driver responsible for an URI.
         *
         * Uses \ref GetDefaultScheme when \p uri has no scheme. Unlike
         * \ref ResolveDriver, this function returns an empty optional when no active
         * driver is mounted. A non-empty result is a borrowed reference with the same
         * lifetime constraints as \ref ResolveDriver.
         *
         * \param uri URI whose scheme selects the driver.
         *
         * \return The resolved driver, or an empty optional when the scheme has no
         *         active driver.
         */
        Optional<FileSystemDriver&> TryResolveDriver(const URI& uri);

        /**
         * \brief Attempts to resolve the driver responsible for an URI without mutable access.
         *
         * Applies the same resolution and lifetime rules as the non-const overload.
         *
         * \param uri URI whose scheme selects the driver.
         *
         * \return The resolved driver, or an empty optional when the scheme has no
         *         active driver.
         */
        Optional<const FileSystemDriver&> TryResolveDriver(const URI& uri) const;

        /**
         * \brief Gets the driver mounted under an exact scheme.
         *
         * Unlike \ref ResolveDriver, this function does not parse an URI and never
         * falls back to \ref GetDefaultScheme. The returned reference is borrowed;
         * its lifetime is constrained by the mounted driver, especially when it was
         * registered with \ref MountExternal.
         *
         * \param scheme Exact scheme key to look up.
         *
         * \return The non-null driver currently mounted under \p scheme.
         *
         * \throws NoSuchDriverException When no active driver is mounted under
         *         \p scheme.
         */
        FileSystemDriver& GetDriver(const String& scheme);

        /**
         * \brief Gets the driver mounted under an exact scheme without mutable access.
         *
         * Applies the same exact-match and lifetime rules as the non-const overload.
         *
         * \param scheme Exact scheme key to look up.
         *
         * \return The non-null driver currently mounted under \p scheme.
         *
         * \throws NoSuchDriverException When no active driver is mounted under
         *         \p scheme.
         */
        const FileSystemDriver& GetDriver(const String& scheme) const;

        /**
         * \brief Attempts to get the driver mounted under an exact scheme.
         *
         * Unlike \ref TryResolveDriver, this function does not parse an URI and
         * never falls back to \ref GetDefaultScheme. A non-empty result contains a
         * borrowed reference with the lifetime constraints of the mounted driver.
         *
         * \param scheme Exact scheme key to look up.
         *
         * \return The mounted driver, or an empty optional when \p scheme has no
         *         active driver.
         */
        Optional<FileSystemDriver&> TryGetDriver(const String& scheme);

        /**
         * \brief Attempts to get the driver mounted under an exact scheme without mutable access.
         *
         * Applies the same exact-match and lifetime rules as the non-const overload.
         *
         * \param scheme Exact scheme key to look up.
         *
         * \return The mounted driver, or an empty optional when \p scheme has no
         *         active driver.
         */
        Optional<const FileSystemDriver&> TryGetDriver(const String& scheme) const;

    private:

        String mDefaultScheme; ///< Fallback scheme used when an URI has no explicit scheme.
        HashMap<String, FileSystemDriver*> mDrivers; ///< Active, non-owning scheme-to-driver resolution table.
        Array<UniquePtr<FileSystemDriver>> mInternalDrivers; ///< Drivers owned and retained by this service.
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_STORAGESERVICE_HPP
