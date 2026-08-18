#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORY_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORY_HPP

#include <dirent.h>

#include <CocktailEngine/Core/System/FileSystem/Directory.hpp>

namespace Ck::Detail::Unix
{
	/**
	 * \brief 
	 */
	class LocalDirectory : public Directory
	{
	public:

		/**
		 * \brief 
		 * \param path 
		 */
		explicit LocalDirectory(const Path& path);

		/**
		 * \brief 
		 */
		~LocalDirectory() override;

		/**
		 * \brief 
		 * \return 
		 */
		Array<Path> GetContent() const override;

		/**
		 * \brief 
		 * \return 
		 */
		const Path& GetPath() const override;

		/**
		 * \brief 
		 * \return 
		 */
		void* GetSystemHandle() const override;

	private:

		Path mPath;
		DIR* mHandle;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_UNIX_LOCALDIRECTORY_HPP
