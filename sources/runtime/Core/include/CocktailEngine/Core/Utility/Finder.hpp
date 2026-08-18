#ifndef COCKTAILENGINE_CORE_UTILITY_FINDER_HPP
#define COCKTAILENGINE_CORE_UTILITY_FINDER_HPP

#include <CocktailEngine/Core/System/FileSystem/Path.hpp>
#include <CocktailEngine/Core/System/FileSystem/Local/LocalFileSystem.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_CORE_API Finder
	{
	public:

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		Finder& From(const Path& path);

		/**
		 * \brief 
		 * \param fileExtension 
		 * \return 
		 */
		Finder& WithExtension(String fileExtension);

		/**
		 * \brief 
		 * \param ignoreFiles 
		 * \return 
		 */
		Finder& IgnoreFiles(bool ignoreFiles = true);

		/**
		 * \brief 
		 * \param ignoreDirectories 
		 * \return 
		 */
		Finder& IgnoreDirectories(bool ignoreDirectories = true);

		/**
		 * \brief 
		 * \param recursive 
		 * \param depth 
		 * \return 
		 */
		Finder& Recursively(bool recursive, unsigned int depth = std::numeric_limits<unsigned int>::max());

		/**
		 * \brief 
		 * \return 
		 */
		Array<Path> Get(FileSystemDriver* fileSystemDriver = LocalFileSystem::GetRootDriver()) const;

	private:

		/**
		 * \brief 
		 * \param source 
		 * \param depth
		 * \param fileSystemDriver
		 * \return 
		 */
		Array<Path> Get(const Path& source, unsigned int depth, FileSystemDriver* fileSystemDriver) const;

		bool mIgnoreFiles = false;
		bool mIgnoreDirectories = false;
		unsigned int mDepth = 0;
		String mFileExtension;
		Array<Path> mSource;
	};

}
#endif // COCKTAILENGINE_CORE_UTILITY_FINDER_HPP
