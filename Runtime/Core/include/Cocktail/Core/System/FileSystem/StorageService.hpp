#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGESERVICE_HPP

#include <unordered_map>

#include <Cocktail/Core/System/FileSystem/FileSystemDriver.hpp>
#include <Cocktail/Core/System/FileSystem/URI.hpp>

namespace Ck
{
	/**
	 * \class StorageService
	 * \brief Unified access interface for multiple filesystem backends using URI schemes
	 *
	 * The Storage class acts as a high-level facade that delegates filesystem operations
	 * to specific drivers registered for different URI schemes. It enables transparent
	 * access to local filesystems, virtual filesystems, remote storage providers, or
	 * any custom file system implementation.
	 *
	 * A URI determines which backend to use:
	 *
	 *   file:/usr/local/data/file.txt
	 *   asset:/textures/wood.png
	 *   custom:/virtual/db/item
	 *
	 * Drivers are registered via the Mount() method by associating a URI scheme (protocol)
	 * with a FileSystemDriver instance.
	 *
	 * \see URI
	 * \see FileSystemDriver
	 * \see File
	 * \see Directory
	 */
	class COCKTAIL_CORE_API StorageService
	{
	public:

		/**
		 * \brief Constructor
		 *
		 * \param defaultScheme The scheme to use when a URI without it is provided
		 */
		explicit StorageService(String defaultScheme = CK_TEXT("file"));

		StorageService(const StorageService& other) = delete;
		StorageService(StorageService&& other) = default;
		StorageService& operator=(const StorageService& other) = delete;
		StorageService& operator=(StorageService&& other) = default;

		/**
		 * \brief Checks whether the given URI refers to an existing file
		 *
		 * Delegated to the filesystem driver associated with the URI scheme
		 *
		 * \param uri Resource location to check
		 *
		 * \return True if a file exists at the given location, otherwise false
		 */
		bool IsFile(const URI& uri) const;

		/**
		 * \brief Checks whether the given URI refers to an existing directory
		 *
		 * \param uri Resource location to check
		 *
		 * \return True if a directory exists at the given location, otherwise false
		 */
		bool IsDirectory(const URI& uri) const;

		/**
		 * \brief Creates an empty file at the given URI
		 *
		 * If the file already exists, the behavior depends on the implementation
		 * of the filesystem driver
		 *
		 * \param uri Location where the file should be created
		 */
		void CreateFile(const URI& uri) const;

		/**
		 * \brief Creates a directory at the given URI, including any necessary parent directories
		 *
		 * \param uri Directory location to create
		 */
		void CreateDirectory(const URI& uri) const;

		/**
		 * \brief Opens a file for reading and/or writing
		 *
		 * \param uri Resource location to open
		 * \param flags Specifies the access mode to use (read, write, truncate, etc.)
		 *
		 * \return A unique pointer to a File interface, or nullptr on failure
		 *
		 * \see File
		 * \see FileOpenFlags
		 */
		UniquePtr<File> OpenFile(const URI& uri, FileOpenFlags flags) const;

		/**
		  * \brief Opens a directory for enumeration
		  *
		  * \param uri Directory location to open
		  *
		  * \return A unique pointer to a Directory interface, or nullptr if the directory does not exist
		  *
		  * \see Directory
		  */
		UniquePtr<Directory> OpenDirectory(const URI& uri) const;

		/**
		 * \brief Copies a resource from a source URI to a destination URI
		 *
		 * \param source URI of the resource to copy
		 * \param destination Target location
		 * \param failIfExists If true, the operation fails when the destination already exists
		 *
		 * \throws May throw if copying is not supported by the backend or if preconditions fail
		 */
		void Copy(const URI& source, const URI& destination, bool failIfExists = false) const;

		/**
		 * \brief Moves (renames) a resource from a source URI to a destination URI
		 *
		 * After a successful move, the source resource no longer exists.
		 *
		 * \param source Location of the existing resource
		 * \param destination New target location
		 */
		void Move(const URI& source, const URI& destination) const;

		/**
		 * \brief Removes a file or directory
		 *
		 * Removal of non-empty directories depends on the backend implementation
		 *
		 * \param uri Location of the resource to delete
		 */
		void Remove(const URI& uri) const;

		/**
		 * \brief Registers and retains a filesystem driver for the given URI scheme
		 *
		 * Once mounted, all operations on URIs whose scheme matches \p protocol
		 * will be handled by the provided FileSystemDriver.
		 *
		 * \param scheme URI scheme (e.g. "file", "asset")
		 * \param fileSystemDriver Driver instance responsible for this scheme
		 */
		void Mount(String scheme, UniquePtr<FileSystemDriver> fileSystemDriver);

		/**
		 * \brief Registers a filesystem driver for the given URI scheme
		 *
		 * Once mounted, all operations on URIs whose scheme matches \p protocol
		 * will be handled by the provided FileSystemDriver.
		 *
		 * \param scheme URI scheme (e.g. "file", "asset")
		 * \param fileSystemDriver Driver instance responsible for this scheme
		 *
		 * \note The caller retains ownership of the driver object
		 */
		void MountExternal(String scheme, FileSystemDriver* fileSystemDriver);

		/**
		 * \brief Unregisters a filesystem driver associated with a scheme
		 *
		 * Calls to URIs using this protocol will fail after the unmount operation
		 *
		 * \param scheme The previously registered URI scheme
		 */
		void UnMount(const String& scheme);

		/**
		 * \brief Returns the default URI scheme used by the Storage system
		 *
		 * When a URI does not explicitly specify a scheme, the default scheme will be used
		 * to resolve the request. This enables operations such as:
		 *
		 *     OpenFile("path/to/file.txt")
		 *
		 * to implicitly target a specific filesystem driver, e.g. "file" or "asset".
		 *
		 * @return A constant reference to the string representing the default scheme
		 */
	    const String& GetDefaultScheme() const;

	    /**
         * \brief
         * \param scheme
         * \return
         */
	    FileSystemDriver* ResolveDriver(const String& scheme) const;

	private:

		String mDefaultScheme;
		std::unordered_map<String, FileSystemDriver*> mDrivers;
		Array<UniquePtr<FileSystemDriver>> mInternalDrivers;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGESERVICE_HPP
