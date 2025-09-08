#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP

#include <Cocktail/Core/System/FileSystem/Directory.hpp>
#include <Cocktail/Core/System/FileSystem/File.hpp>
#include <Cocktail/Core/System/FileSystem/FileOpenFlags.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class FileSystemDriver
	{
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~FileSystemDriver() = default;

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		virtual bool IsFile(const std::filesystem::path& path) const = 0;

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		virtual bool IsDirectory(const std::filesystem::path& path) const = 0;

		/**
		 * \brief
		 * \param path
		 */
		virtual void CreateFile(const std::filesystem::path& path) = 0;

		/**
		 * \brief 
		 * \param path 
		 */
		virtual void CreateDirectory(const std::filesystem::path& path) = 0;

		/**
		 * \brief
		 * \param path
		 * \param flags
		 * \return
		 */
		virtual std::unique_ptr<File> OpenFile(const std::filesystem::path& path, const FileOpenFlags& flags) = 0;

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		virtual std::unique_ptr<Directory> OpenDirectory(const std::filesystem::path& path) = 0;

		/**
		 * \brief
		 * \param source
		 * \param destination
		 * \param failIfExists
		 */
		virtual void Copy(const std::filesystem::path& source, const std::filesystem::path& destination, bool failIfExists = false) = 0;

		/**
		 * \brief 
		 * \param source 
		 * \param destination 
		 */
		virtual void Move(const std::filesystem::path& source, const std::filesystem::path& destination) = 0;

		/**
		 * \brief 
		 * \param path 
		 */
		virtual void Remove(const std::filesystem::path& path) = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP
