#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMSERVICE_HPP

#include <filesystem>

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
		virtual std::filesystem::path GetWorkingDirectory() const = 0;

		/**
		 * \brief Set the path of the process current working directory
		 * \param workingDirectory 
		 */
		virtual void SetWorkingDirectory(const std::filesystem::path& workingDirectory) = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_LOCALFILESYSTEMSERVICE_HPP
