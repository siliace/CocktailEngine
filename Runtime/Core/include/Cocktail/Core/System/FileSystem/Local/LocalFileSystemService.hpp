#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMSERVICE_HPP

#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemDriver.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class LocalFileSystemService : public Implements<LocalFileSystemService, Interface>
	{
	public:

#ifdef COCKTAIL_OS_WINDOWS
		static constexpr std::string_view RootLocation;
#else
		static constexpr std::string_view RootLocation = "/";
#endif

		/**
		 * \brief
		 * \param location
		 * \param followLink
		 * \param lazyRootCreation
		 * \return
		 */
		virtual Ref<LocalFileSystemDriver> CreateLocalDriver(std::filesystem::path location = RootLocation, bool followLink = true, bool lazyRootCreation = false) = 0;
	};
}
#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMSERVICE_HPP
