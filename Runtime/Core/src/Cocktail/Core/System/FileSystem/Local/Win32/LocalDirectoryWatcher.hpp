#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALDIRECTORYWATCHER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALDIRECTORYWATCHER_HPP

#include <Cocktail/Core/System/FileSystem/Directory.hpp>
#include <Cocktail/Core/System/FileSystem/DirectoryWatcher.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class LocalDirectoryWatcher : public DirectoryWatcher
	{
	public:

		constexpr static std::size_t NotificationBufferSize = 32 * 1024;

		/**
		 * \brief 
		 * \param directory
		 * \param recursive 
		 */
		LocalDirectoryWatcher(Directory& directory, bool recursive);

		/**
		 * \brief 
		 */
		void PollEvents() override;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Path>& OnFileCreated() override;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Path>& OnFileModified() override;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Path, Path>& OnFileRenamed() override;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Path>& OnFileDeleted() override;

	private:

		Directory* mDirectory;
		bool mRecursive;
		Signal<Path> mOnFileCreated;
		Signal<Path> mOnFileModified;
		Signal<Path, Path> mOnFileRenamed;
		Signal<Path> mOnFileDeleted;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALDIRECTORYWATCHER_HPP
