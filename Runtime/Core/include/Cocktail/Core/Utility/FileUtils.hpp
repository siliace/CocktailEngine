#ifndef COCKTAIL_CORE_UTILITY_FILEUTILS_HPP
#define COCKTAIL_CORE_UTILITY_FILEUTILS_HPP

#include <Cocktail/Core/System/FileSystem/URI.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API FileUtils
	{
	public:

		/**
		 * \brief 
		 * \param path
		 */
		static void MakeDirectories(const URI& uri);

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		static ByteArray ReadFile(const URI& uri);

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		static Array<String> ReadFileLines(const URI& uri);

		/**
		 * \brief 
		 * \param path 
		 * \param content
		 * \param append Flag indicating whether append or truncate the file to write before opening it
		 */
		static void WriteFile(const URI& uri, ByteArrayView content, bool append);

		/**
		 * \brief Write an array of string into a file
		 * \param path The path of the file to write
		 * \param lines Strings to write to the file
		 * \param append Flag indicating whether append or truncate the file to write before opening it
		 */
		static void WriteFileLines(const URI& uri, const Array<String>& lines, bool append);
	};
}

#endif // COCKTAIL_CORE_UTILITY_FILEUTILS_HPP
