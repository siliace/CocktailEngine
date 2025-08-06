#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_WIN_32FILELOCK_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_WIN_32FILELOCK_HPP

#include <Cocktail/Core/System/FileSystem/File.hpp>
#include <Cocktail/Core/System/FileSystem/FileLock.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class LocalFileLock : public FileLock
	{
	public:

		/**
		 * \brief 
		 * \param file
		 * \param offset
		 * \param length 
		 */
		LocalFileLock(File& file, std::size_t offset, std::size_t length);

		/**
		 * \brief 
		 */
		void Lock() override;

		/**
		 * \brief 
		 * \return 
		 */
		bool TryLock() override;

		/**
		 * \brief 
		 */
		void Unlock() override;

	private:

		File* mFile;
		std::size_t mOffset;
		std::size_t mLength;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_WIN_32FILELOCK_HPP
