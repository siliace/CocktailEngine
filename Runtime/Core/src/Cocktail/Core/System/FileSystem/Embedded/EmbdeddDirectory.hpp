#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDIRECTORY_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDIRECTORY_HPP

#include <filesystem>

#include <cmrc/cmrc.hpp>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/FileSystem/Directory.hpp>


namespace Ck
{
	/**
	 * \brief 
	 */
	class EmbeddedDirectory final : public Inherit<EmbeddedDirectory, Object, Directory>
	{
	public:

		/**
		 * \brief 
		 * \param fileSystem 
		 * \param path 
		 */
		EmbeddedDirectory(cmrc::embedded_filesystem fileSystem, std::filesystem::path path) :
			mFileSystem(fileSystem),
			mPath(std::move(path))
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \return 
		 */
		std::vector<std::filesystem::path> GetContent() const override
		{
			std::vector<std::filesystem::path> childrens;
			cmrc::directory_iterator iterable = mFileSystem.iterate_directory(mPath.string());

			childrens.reserve(std::distance(iterable.begin(), iterable.end()));
			for (auto it = iterable.begin(); it != iterable.end(); ++it)
				childrens.push_back(mPath / (*it).filename());

			return childrens;
		}

		/**
		 * \brief 
		 * \return 
		 */
		const std::filesystem::path& GetPath() const override
		{
			return mPath;
		}

		/**
		 * \brief 
		 * \return 
		 */
		void* GetSystemHandle() const override
		{
			return nullptr;
		}

	private:

		cmrc::embedded_filesystem mFileSystem;
		std::filesystem::path mPath;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_EMBEDDED_EMBEDDEDIRECTORY_HPP
