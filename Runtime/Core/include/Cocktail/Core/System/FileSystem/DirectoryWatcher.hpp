#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYWATCHER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYWATCHER_HPP

#include <Cocktail/Core/Signal/Signal.hpp>
#include <Cocktail/Core/System/FileSystem/Path.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class DirectoryWatcher
	{
	public:

		/**
		 * \brief 
		 */
		virtual ~DirectoryWatcher() = default;

		/**
		 * \brief 
		 */
		virtual void PollEvents() = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Signal<Path>& OnFileCreated() = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Signal<Path>& OnFileModified() = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Signal<Path, Path>& OnFileRenamed() = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Signal<Path>& OnFileDeleted() = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYWATCHER_HPP
