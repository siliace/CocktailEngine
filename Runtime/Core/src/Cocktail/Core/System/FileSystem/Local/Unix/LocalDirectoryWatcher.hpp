#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORYWATCHER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORYWATCHER_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/FileSystem/Directory.hpp>
#include <Cocktail/Core/System/FileSystem/DirectoryWatcher.hpp>

namespace Ck::Detail::Unix
{
	/**
	 * \brief 
	 */
	class LocalDirectoryWatcher : public Inherit<LocalDirectoryWatcher, Object, DirectoryWatcher>
	{
	public:

		/**
		 * \brief 
		 * \param directory
		 * \param recursive 
		 */
		LocalDirectoryWatcher(const Ref<Directory>& directory, bool recursive);

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
		Signal<std::filesystem::path, std::filesystem::path>& OnFileRenamed() override;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<std::filesystem::path>& OnFileDeleted() override;

	private:

		Ref<Directory> mDirectory;
		bool mRecursive;
		Signal<std::filesystem::path> mOnFileCreated;
		Signal<std::filesystem::path, std::filesystem::path> mOnFileRenamed;
		Signal<std::filesystem::path> mOnFileDeleted;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORYWATCHER_HPP
