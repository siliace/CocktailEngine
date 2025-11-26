#ifndef COCKTAIL_CORE_UTILITY_FILEUTILS_HPP
#define COCKTAIL_CORE_UTILITY_FILEUTILS_HPP

#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/LineReader.hpp>
#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>
#include <Cocktail/Core/IO/Output/Writer/FileWriter.hpp>
#include <Cocktail/Core/IO/Output/Writer/LineWriter.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API FileUtils
	{
	public:

	    /**
         * \brief Ensure a directory exists
         *
         * This function will create recursively directories to ensure \p uri is a valid directory
         *
         * \param path The path of the directory to create
         * \param driver The file system driver where the directory should be accessible
         */
		static void MakeDirectories(const Path& path, FileSystemDriver* driver = LocalFileSystem::GetRootDriver());

	    /**
         * \brief Reads the whole content of a file
         *
         * \param path The path on the file system of the file
         * \param driver The file system driver where the file is accessible
         *
         * \return A ByteArray filled with the content of the file
         */
		static ByteArray ReadFile(const Path& path, FileSystemDriver* driver = LocalFileSystem::GetRootDriver());

		/**
		 * \brief Reads the whole content of a file as text
	    *
         * \param path The path on the file system of the file
         * \param driver The file system driver where the file is accessible
		 *
         * \return A String filled with the content of file
		 */
	    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
		static BasicString<TEncoding, TAllocator> ReadFileText(const Path& path, FileSystemDriver* driver = LocalFileSystem::GetRootDriver())
	    {
	        ByteArray content = ReadFile(path, driver);
	        if (content.IsEmpty())
	            return BasicString<TEncoding, TAllocator>::Empty;

	        return Encoders::GetString<TEncoding, BasicString<TEncoding, TAllocator>>(content);
	    }

		/**
         * \brief Reads the while content of a file line by line
		 *
         * \param path The path on the file system of the file
         * \param ignoreEmptyLines True to ignore empty lines
         * \param driver The file system driver where the file is accessible
		 *
         * \return An array containing strings each filled with one line from the file
		 */
	    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
	    static Array<BasicString<TEncoding, TAllocator>, TAllocator> ReadFileLines(const Path& path, bool ignoreEmptyLines = true, FileSystemDriver* driver = LocalFileSystem::GetRootDriver())
	    {
	        if (!driver->IsFile(path))
	            return {};

	        FileInputStream inputStream(path, driver);
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
		 * \brief Writes into a file
		 *
         * \param path The path on the file system of the file
		 * \param content The data to write in the file
		 * \param append Flag indicating whether append or truncate the file to write before opening it
         * \param driver The file system driver where the file is accessible
		 */
		static void WriteFile(const Path& path, ByteArrayView content, bool append, FileSystemDriver* driver = LocalFileSystem::GetRootDriver());

		/**
		 * \brief Writes an array of string into a file
		 *
         * \param path The path on the file system of the file
		 * \param lines Strings to write to the file
		 * \param append Flag indicating whether append or truncate the file to write before opening it
         * \param driver The file system driver where the file is accessible
		 */
	    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
		static void WriteFileLines(const Path& path, const Array<BasicString<TEncoding, TAllocator>>& lines, bool append, FileSystemDriver* driver = LocalFileSystem::GetRootDriver())
	    {
	        if (!driver->IsFile(path))
	        {
	            MakeDirectories(path.GetParent());
	            driver->CreateFile(path);
	        }

	        FileWriter<TEncoding> writer(path, !append);
	        LineWriter<TEncoding> lineWriter(writer);

	        lines.ForEach([&](const String& line) {
                lineWriter.WriteLine(line.GetData(), line.GetLength());
            });
	    }
	};
}

#endif // COCKTAIL_CORE_UTILITY_FILEUTILS_HPP
