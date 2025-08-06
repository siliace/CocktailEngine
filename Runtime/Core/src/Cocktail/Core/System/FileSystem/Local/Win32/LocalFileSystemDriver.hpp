#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMDRIVER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMDRIVER_HPP

#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemDriver.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief
	 */
	class LocalFileSystemDriver : public Ck::LocalFileSystemDriver
	{
	public:

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		bool IsFile(const std::filesystem::path& path) const override;

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		bool IsDirectory(const std::filesystem::path& path) const override;

		/**
		 * \brief
		 * \param path
		 */
		void CreateFile(const std::filesystem::path& path) override;

		/**
		 * \brief
		 * \param path
		 */
		void CreateDirectory(const std::filesystem::path& path) override;

		/**
		 * \brief
		 * \param path
		 * \param flags
		 * \return
		 */
		std::unique_ptr<File> OpenFile(const std::filesystem::path& path, const FileOpenFlags& flags) override;

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		std::unique_ptr<Directory> OpenDirectory(const std::filesystem::path& path) override;

		/**
		 * \brief
		 * \param source
		 * \param destination
		 * \param failIfExists
		 */
		void Copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool failIfExists) override;

		/**
		 * \brief
		 * \param source
		 * \param destination
		 */
		void Move(const std::filesystem::path& source, const std::filesystem::path& destination) override;

		/**
		 * \brief
		 * \param path
		 */
		void Remove(const std::filesystem::path& path) override;

		/**
		 * \brief 
		 * \param file 
		 * \param offset 
		 * \param length 
		 * \return 
		 */
		std::unique_ptr<FileLock> CreateLock(File& file, std::size_t offset, std::size_t length) override;

		/**
		 * \brief 
		 * \param directory 
		 * \param recursive 
		 * \return 
		 */
		std::unique_ptr<DirectoryWatcher> CreateWatcher(Directory& directory, bool recursive) override;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMDRIVER_HPP
