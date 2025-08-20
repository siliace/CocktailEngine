#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORYWATCHER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORYWATCHER_HPP

#include <Cocktail/Core/System/FileSystem/Directory.hpp>
#include <Cocktail/Core/System/FileSystem/DirectoryWatcher.hpp>

namespace Ck::Detail::Unix
{
	/**
	 * \brief 
	 */
	class LocalDirectoryWatcher : public DirectoryWatcher
	{
	public:

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
		Signal<std::filesystem::path>& OnFileCreated() override;

		/**
		 * \brief
		 * \return
		 */
		Signal<std::filesystem::path>& OnFileModified() override;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<std::filesystem::path, std::filesystem::path>& OnFileRenamed() override;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<std::filesystem::path>& OnFileDeleted() override;

	private:

		Directory* mDirectory;
		bool mRecursive;
		Signal<std::filesystem::path> mOnFileCreated;
		Signal<std::filesystem::path> mOnFileModified;
		Signal<std::filesystem::path, std::filesystem::path> mOnFileRenamed;
		Signal<std::filesystem::path> mOnFileDeleted;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORYWATCHER_HPP
