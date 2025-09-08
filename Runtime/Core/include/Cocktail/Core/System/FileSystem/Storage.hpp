#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGE_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/System/FileSystem/StorageService.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Storage : public ServiceFacade<StorageService>
	{
	public:

		/**
		 * \brief
		 * \param uri
		 * \return
		 */
		static bool IsFile(const URI& uri);

		/**
		 * \brief
		 * \param uri
		 * \return
		 */
		static bool IsDirectory(const URI& uri);

		/**
		 * \brief
		 * \param uri
		 */
		static void CreateFile(const URI& uri);

		/**
		 * \brief
		 * \param uri
		 */
		static void CreateDirectory(const URI& uri);

		/**
		 * \brief
		 * \param uri
		 * \param flags
		 * \return
		 */
		static std::unique_ptr<File> OpenFile(const URI& uri, FileOpenFlags flags);

		/**
		 * \brief
		 * \param uri
		 * \return
		 */
		static std::unique_ptr<Directory> OpenDirectory(const URI& uri);

		/**
		 * \brief
		 * \param source
		 * \param destination
		 * \param failIfExists
		 */
		static void Copy(const URI& source, const URI& destination, bool failIfExists = false);

		/**
		 * \brief
		 * \param source
		 * \param destination
		 */
		static void Move(const URI& source, const URI& destination);

		/**
		 * \brief
		 * \param uri
		 */
		static void Remove(const URI& uri);

		/**
		 * \brief
		 * \param protocol
		 * \param fileSystemDriver
		 */
		static void Mount(String protocol, FileSystemDriver* fileSystemDriver);

		/**
		 * \brief
		 * \param protocol
		 */
		static void UnMount(const String& protocol);
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGE_HPP
