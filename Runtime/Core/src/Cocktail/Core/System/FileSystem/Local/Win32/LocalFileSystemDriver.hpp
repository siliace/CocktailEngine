#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMDRIVER_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMDRIVER_HPP

#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemDriver.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief
	 */
	class LocalFileSystemDriver : public Ck::LocalFileSystemDriver
	{
	public:

		/**
		 * \brief 
		 * \param base 
		 */
		explicit LocalFileSystemDriver(Path base = Path::Empty);

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		bool IsFile(const Path& path) const override;

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		bool IsDirectory(const Path& path) const override;

		/**
		 * \brief
		 * \param path
		 */
		void CreateFile(const Path& path) override;

		/**
		 * \brief
		 * \param path
		 */
		void CreateDirectory(const Path& path) override;

		/**
		 * \brief
		 * \param path
		 * \param flags
		 * \return
		 */
		std::unique_ptr<File> OpenFile(const Path& path, const FileOpenFlags& flags) override;

		/**
		 * \brief
		 * \param path
		 * \return
		 */
		std::unique_ptr<Directory> OpenDirectory(const Path& path) override;

		/**
		 * \brief
		 * \param source
		 * \param destination
		 * \param failIfExists
		 */
		void Copy(const Path& source, const Path& destination, bool failIfExists) override;

		/**
		 * \brief
		 * \param source
		 * \param destination
		 */
		void Move(const Path& source, const Path& destination) override;

		/**
		 * \brief
		 * \param path
		 */
		void Remove(const Path& path) override;

		/**
		 * \brief 
		 * \param file 
		 * \param offset 
		 * \param length 
		 * \return 
		 */
		std::unique_ptr<FileLock> CreateLock(File& file, std::size_t offset, std::size_t length) override;

		/**
		 * \brief 
		 * \param directory 
		 * \param recursive 
		 * \return 
		 */
		std::unique_ptr<DirectoryWatcher> CreateWatcher(Directory& directory, bool recursive) override;

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		Path MakeCanonical(const Path& path) override;

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		Optional<Path> TryMakeCanonical(const Path& path) override;

	private:

		Path mBase;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMDRIVER_HPP
