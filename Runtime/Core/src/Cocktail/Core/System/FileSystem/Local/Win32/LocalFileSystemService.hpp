#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMSERVICE_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemService.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class LocalFileSystemService : public Inherit<LocalFileSystemService, Object, Ck::LocalFileSystemService>
	{
	public:

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

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMSERVICE_HPP
