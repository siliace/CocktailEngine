#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMSERVICE_HPP

#include <Cocktail/Core/System/FileSystem/Path.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemDriver.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class LocalFileSystemService
	{
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~LocalFileSystemService() = default;

		/**
		 * \brief Get the path of the process current working directory
		 * \return The path of the working directory
		 */
		virtual Path GetWorkingDirectory() const = 0;

		/**
		 * \brief Set the path of the process current working directory
		 * \param workingDirectory 
		 */
		virtual void SetWorkingDirectory(const Path& workingDirectory) = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Path GetTempDirectoryPath() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual std::unique_ptr<LocalFileSystemDriver> CreateDriver(const Path& base = Path::Empty) = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMSERVICE_HPP
