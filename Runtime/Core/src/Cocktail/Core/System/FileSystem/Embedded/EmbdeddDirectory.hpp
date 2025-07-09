#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDIRECTORY_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDIRECTORY_HPP

#include <filesystem>

#include <cmrc/cmrc.hpp>

#include <Cocktail/Core/System/FileSystem/Directory.hpp>


namespace Ck
{
	/**
	 * \brief 
	 */
	class EmbeddedDirectory final : public Directory
	{
	public:

		/**
		 * \brief 
		 * \param fileSystem 
		 * \param path 
		 */
		EmbeddedDirectory(cmrc::embedded_filesystem fileSystem, std::filesystem::path path);

		/**
		 * \brief 
		 * \return 
		 */
		Array<std::filesystem::path> GetContent() const override;

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

		cmrc::embedded_filesystem mFileSystem;
		std::filesystem::path mPath;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDIRECTORY_HPP
