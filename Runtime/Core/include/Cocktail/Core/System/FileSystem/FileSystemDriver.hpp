#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP

#include <Cocktail/Core/Memory/UniquePtr.hpp>
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
		virtual bool IsFile(const Path& path) const = 0;

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		virtual bool IsDirectory(const Path& path) const = 0;

		/**
		 * \brief
		 * \param path
		 */
		virtual void CreateFile(const Path& path) = 0;

		/**
		 * \brief 
		 * \param path 
		 */
		virtual void CreateDirectory(const Path& path) = 0;

		/**
		 * \brief
		 * \param path
		 * \param flags
		 * \return
		 */
		virtual UniquePtr<File> OpenFile(const Path& path, const FileOpenFlags& flags) = 0;

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		virtual UniquePtr<Directory> OpenDirectory(const Path& path) = 0;

		/**
		 * \brief
		 * \param source
		 * \param destination
		 * \param failIfExists
		 */
		virtual void Copy(const Path& source, const Path& destination, bool failIfExists = false) = 0;

		/**
		 * \brief 
		 * \param source 
		 * \param destination 
		 */
		virtual void Move(const Path& source, const Path& destination) = 0;

		/**
		 * \brief 
		 * \param path 
		 */
		virtual void Remove(const Path& path) = 0;

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		virtual Path MakeCanonical(const Path& path) = 0;

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		virtual Optional<Path> TryMakeCanonical(const Path& path) = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILESYSTEMDRIVER_HPP
