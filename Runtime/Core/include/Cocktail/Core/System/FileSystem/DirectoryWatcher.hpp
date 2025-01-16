#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYWATCHER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYWATCHER_HPP

#include <filesystem>

#include <Cocktail/Core/Signal/Signal.hpp>
#include <Cocktail/Core/Meta/Implements.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class DirectoryWatcher : public Implements<Directory, Interface>
	{
	public:

		/**
		 * \brief 
		 */
		virtual void PollEvents() = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Signal<std::filesystem::path>& OnFileCreated() = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Signal<std::filesystem::path, std::filesystem::path>& OnFileRenamed() = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Signal<std::filesystem::path>& OnFileDeleted() = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYWATCHER_HPP
