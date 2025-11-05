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
		 * \brief
		 * \param base
		 * \return
		 */
		std::unique_ptr<Ck::LocalFileSystemDriver> CreateDriver(const Path& base) override;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALFILESYSTEMSERVICE_HPP
