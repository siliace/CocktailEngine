#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYWATCHER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYWATCHER_HPP

#include <Cocktail/Core/Signal/Signal.hpp>
#include <Cocktail/Core/System/FileSystem/Path.hpp>

namespace Ck
{
    /**
     * \brief Watches a directory and notifies about file system changes
     *
     * DirectoryWatcher provides an interface to monitor a directory for
     * changes such as file creation, modification, deletion, and renaming.
     *
     * Events are collected via PollEvents() and dispatched
     * through signals that users can subscribe to.
     */
	class DirectoryWatcher
	{
	public:

	    /**
         * \brief Destructor
         */
		virtual ~DirectoryWatcher() = default;

	    /**
         * \brief Polls the file system for new events
         *
         * Processes pending file system changes and triggers the corresponding
         * signals. This method must be called regularly to ensure events are
         * dispatched.
         */
		virtual void PollEvents() = 0;

	    /**
         * \brief Signal emitted when a file is created
         *
         * The signal provides the path of the created file.
         *
         * \return Signal triggered on file creation
         */
		virtual Signal<Path>& OnFileCreated() = 0;

	    /**
         * \brief Signal emitted when a file is modified
         *
         * The signal provides the path of the modified file.
         *
         * \return Signal triggered on file modification
         */
		virtual Signal<Path>& OnFileModified() = 0;

	    /**
         * \brief Signal emitted when a file is renamed
         *
         * The signal provides the old path and the new path of the file.
         *
         * \return Signal triggered on file rename
         */
		virtual Signal<Path, Path>& OnFileRenamed() = 0;

	    /**
         * \brief Signal emitted when a file is deleted
         *
         * The signal provides the path of the deleted file.
         *
         * \return Signal triggered on file deletion
         */
		virtual Signal<Path>& OnFileDeleted() = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYWATCHER_HPP
