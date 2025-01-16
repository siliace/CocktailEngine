#ifndef COCKTAIL_CORE_UTILITY_FILEUTILS_HPP
#define COCKTAIL_CORE_UTILITY_FILEUTILS_HPP

#include <filesystem>

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
		static std::vector<std::string> ReadFileLines(const std::filesystem::path& path);

		/**
		 * \brief 
		 * \param path 
		 * \param content 
		 */
		static void WriteFile(const std::filesystem::path& path, ByteArrayView content);

		/**
		 * \brief
		 * \param path
		 * \param lines
		 */
		static void WriteFileLines(const std::filesystem::path& path, const std::vector<std::string>& lines);

		/**
		 * \brief 
		 * \param path 
		 * \param lines 
		 */
		static void AppendFileLines(const std::filesystem::path& path, const std::vector<std::string>& lines);
	};
}

#endif // COCKTAIL_CORE_UTILITY_FILEUTILS_HPP
