#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEM_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEM_HPP

#include <CocktailEngine/Core/Application/ServiceFacade.hpp>
#include <CocktailEngine/Core/System/FileSystem/Local/LocalFileSystemDriver.hpp>
#include <CocktailEngine/Core/System/FileSystem/Local/LocalFileSystemService.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_CORE_API LocalFileSystem : public ServiceFacade<LocalFileSystemService>
	{
	public:

		/**
		 * \brief Get the path of the current working directory
		 * \return The path of the working directory
		 */
		static Path GetWorkingDirectory();

		/**
		 * \brief Set the path of the process current working directory
		 * \param workingDirectory 
		 */
		static void SetWorkingDirectory(const Path& workingDirectory);

		/**
		 * \brief
		 * \return
		 */
	    static Path GetTempDirectoryPath();

	    /**
         * \brief Get the default "root" driver to the local file system
         *
         * \return The root driver
         */
	    static LocalFileSystemDriver* GetRootDriver();

	    /**
         * \brief Create a new LocalFileSystemDriver
         *
         * \param base The base path where the driver to create should point
         *
         * \return The created driver
         */
		static UniquePtr<LocalFileSystemDriver> CreateDriver(const Path& base = Path::Empty);
	};
}
#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEM_HPP
