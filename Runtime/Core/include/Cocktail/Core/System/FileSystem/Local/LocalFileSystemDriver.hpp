#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMDRIVER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMDRIVER_HPP

#include <Cocktail/Core/Meta/Implements.hpp>
#include <Cocktail/Core/System/FileSystem/FileSystemDriver.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class LocalFileSystemDriver : public FileSystemDriver
	{
	public:

		/**
		 * \brief
		 * \param file
		 * \param offset
		 * \param length
		 * \return
		 */
		virtual Ref<FileLock> CreateLock(const Ref<File>& file, std::size_t offset, std::size_t length) = 0;

		/**
		 * \brief
		 * \param directory
		 * \param recursive
		 * \return
		 */
		virtual Ref<DirectoryWatcher> CreateWatcher(const Ref<Directory>& directory, bool recursive) = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMDRIVER_HPP
