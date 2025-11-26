#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILESYSTEMSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILESYSTEMSERVICE_HPP

#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemService.hpp>

namespace Ck::Detail::Unix
{
	/**
	 * \brief 
	 */
	class LocalFileSystemService : public Ck::LocalFileSystemService
	{
	public:

        /**
         * \brief Default constructor
         *
         * Creates a new instance of LocalFileSystemService and initialize its root driver
         */
        LocalFileSystemService();

		/**
		 * \brief Get the path of the current working directory
		 * \return The path of the working directory
		 */
		Path GetWorkingDirectory() const override;

		/**
		 * \brief Set the path of the process current working directory
		 * \param workingDirectory 
		 */
		void SetWorkingDirectory(const Path& workingDirectory) override;

		/**
		 * \brief
		 * \return
		 */
		Path GetTempDirectoryPath() const override;

	    /**
         * \brief Get the default "root" driver to the local file system
         *
         * \return The root driver
         */
	    Ck::LocalFileSystemDriver* GetRootDriver() const override;

	    /**
         * \brief Create a new LocalFileSystemDriver
         *
         * \param base The base path where the driver to create should point
         *
         * \return The created driver
         */
		UniquePtr<Ck::LocalFileSystemDriver> CreateDriver(const Path& base) override;

	private:

	    UniquePtr<Ck::LocalFileSystemDriver> mRootDriver;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILESYSTEMSERVICE_HPP
