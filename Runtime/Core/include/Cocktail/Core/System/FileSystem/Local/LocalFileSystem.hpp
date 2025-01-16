#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEM_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEM_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemDriver.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemService.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API LocalFileSystem : public ServiceFacade<LocalFileSystemService>
	{
	public:

		/**
		 * \brief Get the path of the current working directory
		 * \return The path of the working directory
		 */
		static std::filesystem::path GetWorkingDirectory();

		static void SetWorkingDirectory(const std::filesystem::path& workingDirectory);

		/**
		 * \brief
		 * \param location
		 * \param followLink
		 * \param lazyRootCreation
		 * \return
		 */
		static Ref<LocalFileSystemDriver> CreateLocalDriver(std::filesystem::path location = "", bool followLink = true, bool lazyRootCreation = false);
	};
}
#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEM_HPP
