#ifndef COCKTAIL_CORE_UTILITY_FINDER_HPP
#define COCKTAIL_CORE_UTILITY_FINDER_HPP

#include <filesystem>

#include <Cocktail/Core/Export.hpp>

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
		Finder& From(const std::filesystem::path& path);

		/**
		 * \brief 
		 * \param fileExtension 
		 * \return 
		 */
		Finder& WithExtension(const std::string& fileExtension);

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
		Array<std::filesystem::path> Get() const;

	private:

		/**
		 * \brief 
		 * \param source 
		 * \param depth 
		 * \return 
		 */
		Array<std::filesystem::path> Get(const std::filesystem::path& source, unsigned int depth) const;

		bool mIgnoreFiles = false;
		bool mIgnoreDirectories = false;
		unsigned int mDepth = 0;
		std::string mFileExtension;
		Array<std::filesystem::path> mSource;
	};

}
#endif // COCKTAIL_CORE_UTILITY_FINDER_HPP
