#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMDRIVER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMDRIVER_HPP

#include <CocktailEngine/Core/System/FileSystem/DirectoryWatcher.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileLock.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileSystemDriver.hpp>

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
		virtual UniquePtr<FileLock> CreateLock(File& file, std::size_t offset, std::size_t length) = 0;

		/**
		 * \brief
		 * \param directory
		 * \param recursive
		 * \return
		 */
		virtual UniquePtr<DirectoryWatcher> CreateWatcher(Directory& directory, bool recursive) = 0;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMDRIVER_HPP
