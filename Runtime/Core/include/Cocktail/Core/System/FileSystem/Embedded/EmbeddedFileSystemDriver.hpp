#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDFILESYSTEMDRIVER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDFILESYSTEMDRIVER_HPP

#include <cmrc/cmrc.hpp>

#include <Cocktail/Core/System/FileSystem/FileSystemDriver.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API EmbeddedFileSystemDriver : public FileSystemDriver
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
		std::shared_ptr<File> OpenFile(const std::filesystem::path& path, const FileOpenFlags& flags) override;

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		std::shared_ptr<Directory> OpenDirectory(const std::filesystem::path& path) override;

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
		 * \param fileSystem 
		 */
		void Register(cmrc::embedded_filesystem fileSystem);

	private:

		Array<cmrc::embedded_filesystem> mFileSystems;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDFILESYSTEMDRIVER_HPP
