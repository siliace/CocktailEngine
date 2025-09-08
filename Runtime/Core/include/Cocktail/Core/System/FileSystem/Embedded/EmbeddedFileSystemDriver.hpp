#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDFILESYSTEMDRIVER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDFILESYSTEMDRIVER_HPP

#include <cmrc/cmrc.hpp>

#include <Cocktail/Core/Export.hpp>
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
		bool IsFile(const Path& path) const override;

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		bool IsDirectory(const Path& path) const override;

		/**
		 * \brief 
		 * \param path 
		 */
		void CreateFile(const Path& path) override;

		/**
		 * \brief 
		 * \param path 
		 */
		void CreateDirectory(const Path& path) override;

		/**
		 * \brief 
		 * \param path 
		 * \param flags 
		 * \return 
		 */
		std::unique_ptr<File> OpenFile(const Path& path, const FileOpenFlags& flags) override;

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		std::unique_ptr<Directory> OpenDirectory(const Path& path) override;

		/**
		 * \brief 
		 * \param source 
		 * \param destination 
		 * \param failIfExists 
		 */
		void Copy(const Path& source, const Path& destination, bool failIfExists) override;

		/**
		 * \brief 
		 * \param source 
		 * \param destination 
		 */
		void Move(const Path& source, const Path& destination) override;

		/**
		 * \brief 
		 * \param path 
		 */
		void Remove(const Path& path) override;

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
