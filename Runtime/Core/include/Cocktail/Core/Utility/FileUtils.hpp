#ifndef COCKTAIL_CORE_UTILITY_FILEUTILS_HPP
#define COCKTAIL_CORE_UTILITY_FILEUTILS_HPP

#include <filesystem>

#include <Cocktail/Core/Array.hpp>
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
		static void MakeDirectories(const std::filesystem::path& path);

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		static ByteArray ReadFile(const std::filesystem::path& path);

		/**
		 * \brief 
		 * \param path 
		 * \return 
		 */
		static Array<std::string> ReadFileLines(const std::filesystem::path& path);

		/**
		 * \brief 
		 * \param path 
		 * \param content
		 * \param append Flag indicating whether append or truncate the file to write before opening it
		 */
		static void WriteFile(const std::filesystem::path& path, ByteArrayView content, bool append);

		/**
		 * \brief Write an array of string into a file
		 * \param path The path of the file to write
		 * \param lines Strings to write to the file
		 * \param append Flag indicating whether append or truncate the file to write before opening it
		 */
		static void WriteFileLines(const std::filesystem::path& path, const Array<std::string>& lines, bool append);
	};
}

#endif // COCKTAIL_CORE_UTILITY_FILEUTILS_HPP
