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
		 * \param path
		 * \return
		 */
		static bool IsFile(const std::filesystem::path& path);

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		static bool IsDirectory(const std::filesystem::path& path);

		/**
		 * \brief
		 * \param path
		 */
		static void CreateFile(const std::filesystem::path& path);

		/**
		 * \brief
		 * \param path
		 */
		static void CreateDirectory(const std::filesystem::path& path);

		/**
		 * \brief
		 * \param path
		 * \param flags
		 * \return
		 */
		static Ref<File> OpenFile(const std::filesystem::path& path, FileOpenFlags flags);

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		static Ref<Directory> OpenDirectory(const std::filesystem::path& path);

		/**
		 * \brief
		 * \param source
		 * \param destination
		 * \param failIfExists
		 */
		static void Copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool failIfExists = false);

		/**
		 * \brief
		 * \param source
		 * \param destination
		 */
		static void Move(const std::filesystem::path& source, const std::filesystem::path& destination);

		/**
		 * \brief
		 * \param path
		 */
		static void Remove(const std::filesystem::path& path);

		/**
		 * \brief
		 * \param protocol
		 * \param fileSystemDriver
		 */
		static void Mount(std::string protocol, FileSystemDriver* fileSystemDriver);

		/**
		 * \brief
		 * \param protocol
		 */
		static void UnMount(const std::string& protocol);
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGE_HPP
