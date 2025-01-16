#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORY_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORY_HPP

#include <dirent.h>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/FileSystem/Directory.hpp>

namespace Ck::Detail::Unix
{
	/**
	 * \brief 
	 */
	class LocalDirectory : public Inherit<LocalDirectory, Object, Directory>
	{
	public:

		/**
		 * \brief 
		 * \param path 
		 */
		explicit LocalDirectory(const std::filesystem::path& path);

		/**
		 * \brief 
		 */
		~LocalDirectory() override;

		/**
		 * \brief 
		 * \return 
		 */
		std::vector<std::filesystem::path> GetContent() const override;

		/**
		 * \brief 
		 * \return 
		 */
		const std::filesystem::path& GetPath() const override;

		/**
		 * \brief 
		 * \return 
		 */
		void* GetSystemHandle() const override;

	private:

		std::filesystem::path mPath;
		DIR* mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORY_HPP
