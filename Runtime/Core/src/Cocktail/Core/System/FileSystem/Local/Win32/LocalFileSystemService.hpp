#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMSERVICE_HPP

#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemService.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class LocalFileSystemService : public Ck::LocalFileSystemService
	{
	public:

	    LocalFileSystemService();

		/**
		 * \brief Get the path of the process current working directory
		 *
		 * \return The path of the working directory
		 */
		Path GetWorkingDirectory() const override;

		/**
		 * \brief Set the path of the process current working directory
		 *
		 * \param workingDirectory 
		 */
		void SetWorkingDirectory(const Path& workingDirectory) override;

		/**
		 * \brief 
		 * \return 
		 */
		Path GetTempDirectoryPath() const override;

        /**
         * \brief
         *
         * \return
         */
        Ck::LocalFileSystemDriver* GetRootDriver() const override;

        /**
         * \brief
         *
         * \param base
         *
         * \return
         */
		UniquePtr<Ck::LocalFileSystemDriver> CreateDriver(const Path& base) override;

	private:

	    UniquePtr<Ck::LocalFileSystemDriver> mRootDriver;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALFILESYSTEMSERVICE_HPP
