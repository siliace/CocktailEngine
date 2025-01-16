#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGESERVICE_HPP

#include <string>
#include <unordered_map>

#include <Cocktail/Core/System/FileSystem/FileSystemDriver.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API StorageService
	{
	public:

		/**
		 * \brief 
		 */
		static constexpr std::string_view ProtocolSeparator = "://";

		/**
		 * \brief 
		 * \param defaultProtocol 
		 */
		explicit StorageService(std::string defaultProtocol = "file");

		StorageService(const StorageService&) = delete;
		StorageService(StorageService&&) noexcept = default;

		StorageService& operator=(const StorageService&) = delete;
		StorageService& operator=(StorageService&&) noexcept = default;

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		bool IsFile(const std::filesystem::path& path) const;

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		bool IsDirectory(const std::filesystem::path& path) const;

		/**
		 * \brief
		 * \param path
		 */
		void CreateFile(const std::filesystem::path& path) const;

		/**
		 * \brief
		 * \param path
		 */
		void CreateDirectory(const std::filesystem::path& path) const;

		/**
		 * \brief
		 * \param path
		 * \param flags
		 * \return
		 */
		Ref<File> OpenFile(const std::filesystem::path& path, FileOpenFlags flags) const;

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		Ref<Directory> OpenDirectory(const std::filesystem::path& path) const;

		/**
		 * \brief
		 * \param source
		 * \param destination
		 * \param failIfExists
		 */
		void Copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool failIfExists = false) const;

		/**
		 * \brief
		 * \param source
		 * \param destination
		 */
		void Move(const std::filesystem::path& source, const std::filesystem::path& destination) const;

		/**
		 * \brief
		 * \param path
		 */
		void Remove(const std::filesystem::path& path) const;

		/**
		 * \brief
		 * \param protocol
		 * \param fileSystemDriver
		 */
		void Mount(std::string protocol, FileSystemDriver* fileSystemDriver);

		/**
		 * \brief
		 * \param protocol
		 */
		void UnMount(const std::string& protocol);

		/**
		 * \brief 
		 * \return 
		 */
		const std::string& GetDefaultProtocol() const;

	private:

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		static std::pair<std::string, std::filesystem::path> SplitPathProtocol(const std::filesystem::path& path);

		/**
		 * \brief 
		 * \param protocol 
		 * \return 
		 */
		FileSystemDriver* ResolveProtocol(const std::string& protocol) const;

		std::string mDefaultProtocol;
		std::unordered_map<std::string, FileSystemDriver*> mDrivers;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_STORAGESERVICE_HPP
