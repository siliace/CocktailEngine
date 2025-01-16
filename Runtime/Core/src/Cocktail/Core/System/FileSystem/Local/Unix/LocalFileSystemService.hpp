#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILESYSTEMSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILESYSTEMSERVICE_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemService.hpp>

namespace Ck::Detail::Unix
{
	/**
	 * \brief 
	 */
	class LocalFileSystemService : public Inherit<LocalFileSystemService, Object, Ck::LocalFileSystemService>
	{
	public:

		/**
		 * \brief Get the path of the current working directory
		 * \return The path of the working directory
		 */
		std::filesystem::path GetWorkingDirectory() const override;

		void SetWorkingDirectory(const std::filesystem::path& workingDirectory) override;

		/**
		 * \brief 
		 * \param location 
		 * \param followLink 
		 * \param lazyRootCreation 
		 * \return 
		 */
		Ref<Ck::LocalFileSystemDriver> CreateLocalDriver(std::filesystem::path location, bool followLink, bool lazyRootCreation) override;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILESYSTEMSERVICE_HPP
