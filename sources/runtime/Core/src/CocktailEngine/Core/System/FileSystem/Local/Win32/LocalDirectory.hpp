#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALDIRECTORY_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALDIRECTORY_HPP

#include <CocktailEngine/Core/System/FileSystem/Directory.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
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
		HANDLE mHandle;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALDIRECTORY_HPP
