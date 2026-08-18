#ifndef COCKTAILENGINE_CORE_UTILITY_STORAGEUTILS_HPP
#define COCKTAILENGINE_CORE_UTILITY_STORAGEUTILS_HPP

#include <CocktailEngine/Core/System/FileSystem/Storage.hpp>
#include <CocktailEngine/Core/Utility/FileUtils.hpp>

namespace Ck
{
    class COCKTAILENGINE_CORE_API StorageUtils
    {
    public:

        /**
         * \brief Ensures that a directory identified by a URI exists.
         *
         * Missing parent directories are created recursively through the driver
         * resolved from the URI scheme.
         *
         * \param uri URI of the directory to create.
         * \param storage Storage service used to resolve the URI scheme.
         */
        static void MakeDirectories(const URI& uri, StorageService* storage = Storage::ResolveFacadeInstance());

        /**
         * \brief Reads the complete byte content of a URI-addressed file.
         *
         * \param uri URI of the file to read.
         * \param storage Storage service used to resolve the URI scheme.
         *
         * \return The file content, or an empty array when the URI path is not a
         *         file or when the file is empty.
         */
        static ByteArray ReadFile(const URI& uri, StorageService* storage = Storage::ResolveFacadeInstance());

        /**
         * \brief Reads the complete text content of a URI-addressed file.
         *
         * \tparam TEncoding Encoding used to decode the file bytes.
         * \tparam TAllocator Allocator used by the returned string.
         *
         * \param uri URI of the file to read.
         * \param storage Storage service used to resolve the URI scheme.
         *
         * \return The decoded file content, or an empty string when the URI path is
         *         not a file or when the file is empty.
         */
        template <typename TEncoding = Encoders::Text, typename TAllocator = HeapAllocator>
        static BasicString<TEncoding, TAllocator> ReadFileText(const URI& uri, StorageService* storage = Storage::ResolveFacadeInstance())
        {
            return FileUtils::ReadFileText<TEncoding, TAllocator>(uri.GetPath(), storage->ResolveDriver(uri));
        }

        /**
         * \brief Reads the complete text content of a URI-addressed file as lines.
         *
         * \tparam TEncoding Encoding used to decode the file bytes.
         * \tparam TAllocator Allocator used by the returned strings and array.
         *
         * \param uri URI of the file to read.
         * \param ignoreEmptyLines True to omit empty lines from the result.
         * \param storage Storage service used to resolve the URI scheme.
         *
         * \return The file lines in their file order, or an empty array when the URI
         *         path is not a file or contains no retained lines.
         */
        template <typename TEncoding = Encoders::Text, typename TAllocator = HeapAllocator>
        static Array<BasicString<TEncoding, TAllocator>, TAllocator> ReadFileLines(const URI& uri, bool ignoreEmptyLines = true,
                                                                                   StorageService* storage = Storage::ResolveFacadeInstance())
        {
            return FileUtils::ReadFileLines<TEncoding, TAllocator>(uri.GetPath(), ignoreEmptyLines, storage->ResolveDriver(uri));
        }

        /**
         * \brief Replaces a URI-addressed file with byte content.
         *
         * Resolves the driver from the URI scheme, then delegates to the truncating
         * byte-write semantics of \ref FileUtils::WriteFile. Existing content is
         * discarded even when \p content is empty.
         *
         * \param uri URI of the file to replace.
         * \param content Byte content to write.
         * \param storage Storage service used to resolve the URI scheme.
         */
        static void WriteFile(const URI& uri, ByteArrayView content, StorageService* storage = Storage::ResolveFacadeInstance());

        /**
         * \brief Appends byte content to a URI-addressed file.
         *
         * Resolves the driver from the URI scheme, then delegates to
         * \ref FileUtils::AppendFile without truncating the existing content.
         *
         * \param uri URI of the file to append to.
         * \param content Byte content to append.
         * \param storage Storage service used to resolve the URI scheme.
         */
        static void AppendFile(const URI& uri, ByteArrayView content, StorageService* storage = Storage::ResolveFacadeInstance());

        /**
         * \brief Replaces a URI-addressed file with encoded text content.
         *
         * Resolves the driver from the URI scheme, then delegates to the truncating
         * text-write semantics of \ref FileUtils::WriteFile.
         *
         * \tparam TEncoding Encoding of \p content.
         *
         * \param uri URI of the file to replace.
         * \param content Text content to encode and write.
         * \param encodingOption Output encoding policy.
         * \param storage Storage service used to resolve the URI scheme.
         */
        template <typename TEncoding = Encoders::Text>
        static void WriteFile(const URI& uri, BasicStringView<TEncoding> content, FileUtils::EncodingOption encodingOption = FileUtils::EncodingOption::UTF8WithoutBOM,
                              StorageService* storage = Storage::ResolveFacadeInstance())
        {
            FileUtils::WriteFile<TEncoding>(uri.GetPath(), content, encodingOption, storage->ResolveDriver(uri));
        }

        /**
         * \brief Appends encoded text content to a URI-addressed file.
         *
         * Resolves the driver from the URI scheme, then delegates to
         * \ref FileUtils::AppendFile. The existing file encoding is not detected;
         * the selected policy applies to the appended fragment.
         *
         * \tparam TEncoding Encoding of \p content.
         *
         * \param uri URI of the file to append to.
         * \param content Text content to encode and append.
         * \param encodingOption Output encoding policy for this fragment.
         * \param storage Storage service used to resolve the URI scheme.
         */
        template <typename TEncoding = Encoders::Text>
        static void AppendFile(const URI& uri, BasicStringView<TEncoding> content, FileUtils::EncodingOption encodingOption = FileUtils::EncodingOption::UTF8WithoutBOM,
                               StorageService* storage = Storage::ResolveFacadeInstance())
        {
            FileUtils::AppendFile<TEncoding>(uri.GetPath(), content, encodingOption, storage->ResolveDriver(uri));
        }

        /**
         * \brief Replaces a URI-addressed file with encoded lines.
         *
         * Resolves the driver from the URI scheme, then delegates to
         * \ref FileUtils::WriteFileLines. Each input line receives a platform line
         * terminator and the existing file content is discarded.
         *
         * \tparam TEncoding Encoding of the input lines.
         * \tparam TAllocator Allocator used by the input strings and temporary text.
         * \tparam TArrayAllocator Allocator used by the input array.
         *
         * \param uri URI of the file to replace.
         * \param lines Lines to serialize.
         * \param encodingOption Output encoding policy.
         * \param storage Storage service used to resolve the URI scheme.
         */
        template <typename TEncoding = Encoders::Text, typename TAllocator = HeapAllocator, typename TArrayAllocator = HeapAllocator>
        static void WriteFileLines(const URI& uri, const Array<BasicString<TEncoding, TAllocator>, TArrayAllocator>& lines,
                                   FileUtils::EncodingOption encodingOption = FileUtils::EncodingOption::UTF8WithoutBOM, StorageService* storage = Storage::ResolveFacadeInstance())
        {
            FileUtils::WriteFileLines<TEncoding, TAllocator, TArrayAllocator>(uri.GetPath(), lines, encodingOption, storage->ResolveDriver(uri));
        }

        /**
         * \brief Appends encoded lines to a URI-addressed file.
         *
         * Resolves the driver from the URI scheme, then delegates to
         * \ref FileUtils::AppendFileLines without truncating the existing content.
         * No separator is inserted before the first appended line.
         *
         * \tparam TEncoding Encoding of the input lines.
         * \tparam TAllocator Allocator used by the input strings and temporary text.
         * \tparam TArrayAllocator Allocator used by the input array.
         *
         * \param uri URI of the file to append to.
         * \param lines Lines to serialize and append.
         * \param encodingOption Output encoding policy for this fragment.
         * \param storage Storage service used to resolve the URI scheme.
         */
        template <typename TEncoding = Encoders::Text, typename TAllocator = HeapAllocator, typename TArrayAllocator = HeapAllocator>
        static void AppendFileLines(const URI& uri, const Array<BasicString<TEncoding, TAllocator>, TArrayAllocator>& lines,
                                    FileUtils::EncodingOption encodingOption = FileUtils::EncodingOption::UTF8WithoutBOM,
                                    StorageService* storage = Storage::ResolveFacadeInstance())
        {
            FileUtils::AppendFileLines<TEncoding, TAllocator, TArrayAllocator>(uri.GetPath(), lines, encodingOption, storage->ResolveDriver(uri));
        }
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_STORAGEUTILS_HPP
