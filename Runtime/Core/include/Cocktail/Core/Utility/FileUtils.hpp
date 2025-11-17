#ifndef COCKTAIL_CORE_UTILITY_FILEUTILS_HPP
#define COCKTAIL_CORE_UTILITY_FILEUTILS_HPP

#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>
#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/LineReader.hpp>
#include <Cocktail/Core/System/FileSystem/URI.hpp>
#include <Cocktail/Core/System/FileSystem/Storage.hpp>
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
		 * \param uri
		 */
		static void MakeDirectories(const URI& uri);

		/**
		 * \brief 
		 * \param uri
		 * \return 
		 */
		static ByteArray ReadFile(const URI& uri);

		/**
		 * \brief
		 * \param uri
		 * \return
		 */
		static String ReadFileText(const URI& uri);

		/**
		 * \brief 
		 * \param uri
		 * \param ignoreEmptyLines
		 * \return
		 */
	    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
	    static Array<BasicString<TEncoding, TAllocator>, TAllocator> ReadFileLines(const URI& uri, bool ignoreEmptyLines = true)
	    {
	        if (!Storage::IsFile(uri))
	            return {};

	        std::unique_ptr<File> file = Storage::OpenFile(uri, FileOpenFlagBits::Read | FileOpenFlagBits::Existing);
	        FileInputStream inputStream(*file);

	        InputStreamReader<TEncoding> inputStreamReader(inputStream);
	        BufferedReader<TEncoding> bufferedReader(inputStreamReader);
	        LineReader<TEncoding> lineReader(bufferedReader);

	        BasicString<TEncoding, TAllocator> line;
	        Array<BasicString<TEncoding, TAllocator>, TAllocator> lines;
	        while (lineReader.ReadLine(line, ignoreEmptyLines))
	            lines.Add(line);

	        return lines;
	    }

		/**
		 * \brief 
		 * \param uri
		 * \param content
		 * \param append Flag indicating whether append or truncate the file to write before opening it
		 */
		static void WriteFile(const URI& uri, ByteArrayView content, bool append);

		/**
		 * \brief Write an array of string into a file
		 * \param uri The path of the file to write
		 * \param lines Strings to write to the file
		 * \param append Flag indicating whether append or truncate the file to write before opening it
		 */
		static void WriteFileLines(const URI& uri, const Array<String>& lines, bool append);
	};
}

#endif // COCKTAIL_CORE_UTILITY_FILEUTILS_HPP
