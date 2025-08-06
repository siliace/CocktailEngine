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

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALDIRECTORYWATCHER_HPP
