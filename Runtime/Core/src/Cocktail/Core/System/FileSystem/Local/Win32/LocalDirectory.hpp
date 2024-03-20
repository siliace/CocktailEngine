#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALDIRECTORY_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALDIRECTORY_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/FileSystem/Directory.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
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
		HANDLE mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_LOCAL_WIN32_LOCALDIRECTORY_HPP
