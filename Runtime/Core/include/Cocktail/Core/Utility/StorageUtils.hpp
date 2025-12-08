#ifndef COCKTAIL_CORE_UTILITY_STORAGEUTILS_HPP
#define COCKTAIL_CORE_UTILITY_STORAGEUTILS_HPP

#include <Cocktail/Core/System/FileSystem/Storage.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

namespace Ck
{
    class COCKTAIL_CORE_API StorageUtils
    {
    public:

        /**
         * \brief Ensure a directory exists
         *
         * This function will create recursively directories to ensure \p uri is a valid directory
         *
         * \param uri The location of the directory to create
         * \param storage The storage where the directory should be accessible
         */
        static void MakeDirectories(const URI& uri, StorageService* storage = Storage::ResolveFacadeInstance());

        /**
         * \brief Reads the whole content of a file
         *
         * \param uri The location of the file
         * \param storage The storage where the file is accessible
         *
         * \return A ByteArray filled with the content of the file
         */
        static ByteArray ReadFile(const URI& uri, StorageService* storage = Storage::ResolveFacadeInstance());

        /**
         * \brief Reads the whole content of a file as text
         *
         * \param uri The location of the file
         * \param storage The storage where the file is accessible
         *
         * \return A String filled with the content of file
         */
        template <typename TEncoding = Encoders::Text, typename TAllocator = HeapAllocator>
        static BasicString<TEncoding, TAllocator> ReadFileText(const URI& uri, StorageService* storage = Storage::ResolveFacadeInstance())
        {
            return FileUtils::ReadFileText<TEncoding, TAllocator>(uri.GetPath(), storage->ResolveDriver(uri.GetScheme()));
        }

        /**
         * \brief Reads the while content of a file line by line
         *
         * \param uri The location of the file
         * \param ignoreEmptyLines True to ignore empty lines
         * \param storage The storage where the file is accessible
         *
         * \return An array containing strings each filled with one line from the file
         */
        template <typename TEncoding = Encoders::Text, typename TAllocator = HeapAllocator>
        static Array<BasicString<TEncoding, TAllocator>, TAllocator> ReadFileLines(const URI& uri, bool ignoreEmptyLines = true, StorageService* storage = Storage::ResolveFacadeInstance())
        {
            return FileUtils::ReadFileLines<TEncoding, TAllocator>(uri.GetPath(), ignoreEmptyLines, storage->ResolveDriver(uri.GetScheme()));
        }

        /**
         * \brief Writes into a file
         *
         * \param uri The location of the file
         * \param content The data to write in the file
         * \param append Flag indicating whether append or truncate the file to write before opening it
         * \param storage The storage where the file is accessible
         */
        static void WriteFile(const URI& uri, ByteArrayView content, bool append, StorageService* storage = Storage::ResolveFacadeInstance());

        /**
         * \brief Writes an array of string into a file
         *
         * \param uri The location of the file
         * \param lines Strings to write to the file
         * \param append Flag indicating whether append or truncate the file to write before opening it
         * \param storage The storage where the file is accessible
         */
        template <typename TEncoding = Encoders::Text, typename TAllocator = HeapAllocator>
        static void WriteFileLines(const URI& uri, const Array<BasicString<TEncoding, TAllocator>>& lines, bool append, StorageService* storage = Storage::ResolveFacadeInstance())
        {
            return FileUtils::WriteFileLines<TEncoding, TAllocator>(uri.GetPath(), lines, append, storage->ResolveDriver(uri.GetScheme()));
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_STORAGEUTILS_HPP
