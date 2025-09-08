#ifndef COCKTAIL_CORE_UTILITY_FINDER_HPP
#define COCKTAIL_CORE_UTILITY_FINDER_HPP

#include <Cocktail/Core/System/FileSystem/Path.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Finder
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
		Array<Path> Get() const;

	private:

		/**
		 * \brief 
		 * \param source 
		 * \param depth 
		 * \return 
		 */
		Array<Path> Get(const Path& source, unsigned int depth) const;

		bool mIgnoreFiles = false;
		bool mIgnoreDirectories = false;
		unsigned int mDepth = 0;
		String mFileExtension;
		Array<Path> mSource;
	};

}
#endif // COCKTAIL_CORE_UTILITY_FINDER_HPP
