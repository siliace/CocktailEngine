#ifndef COCKTAILENGINE_CORE_UTILITY_FILEUTILS_HPP
#define COCKTAILENGINE_CORE_UTILITY_FILEUTILS_HPP

#include <CocktailEngine/Core/IO/Input/Reader/BufferedReader.hpp>
#include <CocktailEngine/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <CocktailEngine/Core/IO/Input/Reader/LineReader.hpp>
#include <CocktailEngine/Core/IO/Input/Stream/FileInputStream.hpp>
#include <CocktailEngine/Core/IO/Output/Stream/FileOutputStream.hpp>
#include <CocktailEngine/Core/IO/Output/Writer/LineWriter.hpp>
#include <CocktailEngine/Core/IO/Output/Writer/StringWriter.hpp>
#include <CocktailEngine/Core/Utility/ByteArray.hpp>

namespace Ck
{
    class COCKTAILENGINE_CORE_API FileUtils
    {
    public:

        /**
         * \brief Selects the byte encoding used for text output.
         *
         * The selection applies independently to each write or append operation.
         * Options that include a byte-order mark add it to every non-empty encoded
         * fragment.
         */
        enum class EncodingOption
        {
            /** Select ASCII for ANSI-only content, otherwise UTF-16 with a byte-order mark. */
            Auto,

            /** Encode the content as ASCII without a byte-order mark. */
            Ascii,

            /** Encode the content as UTF-16 preceded by a byte-order mark. */
            Unicode,

            /** Encode the content as UTF-8 preceded by a byte-order mark. */
            UTF8,

            /** Encode the content as UTF-8 without a byte-order mark. */
            UTF8WithoutBOM
        };

        /**
         * \brief Ensures that a directory exists.
         *
         * Creates every missing parent directory recursively through \p driver. If
         * \p path already refers to a directory, this function has no effect.
         *
         * \param path Directory path to create.
         * \param driver File-system driver on which the directory is created.
         */
        static void MakeDirectories(const Path& path, FileSystemDriver& driver = LocalFileSystem::GetRootDriver());

        /**
         * \brief Reads the complete byte content of a file.
         *
         * The file size is sampled after opening, then the file is read repeatedly
         * until that many bytes have been received. If the driver reaches the end of
         * the file before fulfilling this snapshot, an I/O error is reported.
         *
         * \param path File path to read.
         * \param driver File-system driver on which the file is accessible.
         *
         * \return The file content, or an empty array when \p path is not a file or
         *         when the file is empty.
         *
         * \throws std::system_error When the driver reports an early end of file.
         */
        static ByteArray ReadFile(const Path& path, FileSystemDriver& driver = LocalFileSystem::GetRootDriver());

        /**
         * \brief Reads the complete text content of a file.
         *
         * \tparam TEncoding Encoding used to decode the file bytes.
         * \tparam TAllocator Allocator used by the returned string.
         *
         * \param path File path to read.
         * \param driver File-system driver on which the file is accessible.
         *
         * \return The decoded file content, or an empty string when \p path is not
         *         a file or when the file is empty.
         */
        template <typename TEncoding = Encoders::Text, typename TAllocator = HeapAllocator>
        static BasicString<TEncoding, TAllocator> ReadFileText(const Path& path, FileSystemDriver& driver = LocalFileSystem::GetRootDriver())
        {
            ByteArray content = ReadFile(path, driver);
            if (content.IsEmpty())
                return BasicString<TEncoding, TAllocator>::Empty;

            return Encoders::GetString<TEncoding, BasicString<TEncoding, TAllocator>>(content);
        }

        /**
         * \brief Reads the complete text content of a file as lines.
         *
         * \tparam TEncoding Encoding used to decode the file bytes.
         * \tparam TAllocator Allocator used by the returned strings and array.
         *
         * \param path File path to read.
         * \param ignoreEmptyLines True to omit empty lines from the result.
         * \param driver File-system driver on which the file is accessible.
         *
         * \return An array containing the lines in their file order, or an empty
         *         array when \p path is not a file or contains no retained lines.
         */
        template <typename TEncoding = Encoders::Text, typename TAllocator = HeapAllocator>
        static Array<BasicString<TEncoding, TAllocator>, TAllocator> ReadFileLines(const Path& path, bool ignoreEmptyLines = true,
                                                                                   FileSystemDriver& driver = LocalFileSystem::GetRootDriver())
        {
            if (!driver.IsFile(path))
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
         * \brief Replaces a file with byte content.
         *
         * Opens \p path with truncation before writing, so any existing content is
         * discarded even when \p content is empty. Missing parent directories are
         * created recursively through \p driver.
         *
         * \param path File path to replace.
         * \param content Byte content to write.
         * \param driver File-system driver on which the file is written.
         */
        static void WriteFile(const Path& path, ByteArrayView content, FileSystemDriver& driver = LocalFileSystem::GetRootDriver());

        /**
         * \brief Appends byte content to a file.
         *
         * Opens \p path in append mode and preserves any existing content. Missing
         * parent directories are created recursively through \p driver. Creation of
         * an absent file follows the append-open semantics of the selected driver.
         *
         * \param path File path to append to.
         * \param content Byte content to append.
         * \param driver File-system driver on which the file is written.
         */
        static void AppendFile(const Path& path, ByteArrayView content, FileSystemDriver& driver = LocalFileSystem::GetRootDriver());

        /**
         * \brief Replaces a file with encoded text content.
         *
         * The text is encoded according to \p encodingOption, then written with the
         * truncating semantics of the byte overload. Options that emit a byte-order
         * mark add it to the non-empty encoded content.
         *
         * \tparam TEncoding Encoding of \p content.
         *
         * \param path File path to replace.
         * \param content Text content to encode and write.
         * \param encodingOption Output encoding policy.
         * \param driver File-system driver on which the file is written.
         */
        template <typename TEncoding = Encoders::Text>
        static void WriteFile(const Path& path, BasicStringView<TEncoding> content, EncodingOption encodingOption = EncodingOption::UTF8WithoutBOM,
                              FileSystemDriver& driver = LocalFileSystem::GetRootDriver())
        {
            WriteFile(path, EncodeText<TEncoding>(content, encodingOption), driver);
        }

        /**
         * \brief Appends encoded text content to a file.
         *
         * The text is encoded according to \p encodingOption, then appended with
         * the byte overload. The existing file encoding is not detected or changed.
         * In particular, \ref EncodingOption::UTF8 and \ref EncodingOption::Unicode
         * emit a byte-order mark for every non-empty appended fragment; use
         * \ref EncodingOption::UTF8WithoutBOM when continuing a UTF-8 file.
         *
         * \tparam TEncoding Encoding of \p content.
         *
         * \param path File path to append to.
         * \param content Text content to encode and append.
         * \param encodingOption Output encoding policy for this fragment.
         * \param driver File-system driver on which the file is written.
         */
        template <typename TEncoding = Encoders::Text>
        static void AppendFile(const Path& path, BasicStringView<TEncoding> content, EncodingOption encodingOption = EncodingOption::UTF8WithoutBOM,
                               FileSystemDriver& driver = LocalFileSystem::GetRootDriver())
        {
            AppendFile(path, EncodeText<TEncoding>(content, encodingOption), driver);
        }

        /**
         * \brief Replaces a file with encoded lines.
         *
         * Each entry in \p lines is serialized through \ref LineWriter and therefore
         * receives a platform line terminator, including empty entries. The resulting
         * text is then written with the truncating semantics of \ref WriteFile.
         *
         * \tparam TEncoding Encoding of the input lines.
         * \tparam TAllocator Allocator used by the input strings and temporary text.
         * \tparam TArrayAllocator Allocator used by the input array.
         *
         * \param path File path to replace.
         * \param lines Lines to serialize.
         * \param encodingOption Output encoding policy.
         * \param driver File-system driver on which the file is written.
         */
        template <typename TEncoding = Encoders::Text, typename TAllocator = HeapAllocator, typename TArrayAllocator = HeapAllocator>
        static void WriteFileLines(const Path& path, const Array<BasicString<TEncoding, TAllocator>, TArrayAllocator>& lines,
                                   EncodingOption encodingOption = EncodingOption::UTF8WithoutBOM, FileSystemDriver& driver = LocalFileSystem::GetRootDriver())
        {
            StringWriter<TEncoding, TAllocator> writer;
            LineWriter<TEncoding> lineWriter(writer);

            lines.ForEach([&](const BasicString<TEncoding, TAllocator>& line) {
                lineWriter.WriteLine(line.GetData(), line.GetLength());
            });

            const BasicString<TEncoding, TAllocator> content = writer.ToString();
            WriteFile<TEncoding>(path, BasicStringView<TEncoding>(content), encodingOption, driver);
        }

        /**
         * \brief Appends encoded lines to a file.
         *
         * Each entry in \p lines is serialized through \ref LineWriter and therefore
         * receives a platform line terminator, including empty entries. No separator
         * is inserted before the first appended line; callers appending to arbitrary
         * text must ensure the existing file already ends with a line terminator.
         *
         * \tparam TEncoding Encoding of the input lines.
         * \tparam TAllocator Allocator used by the input strings and temporary text.
         * \tparam TArrayAllocator Allocator used by the input array.
         *
         * \param path File path to append to.
         * \param lines Lines to serialize and append.
         * \param encodingOption Output encoding policy for this fragment.
         * \param driver File-system driver on which the file is written.
         */
        template <typename TEncoding = Encoders::Text, typename TAllocator = HeapAllocator, typename TArrayAllocator = HeapAllocator>
        static void AppendFileLines(const Path& path, const Array<BasicString<TEncoding, TAllocator>, TArrayAllocator>& lines,
                                    EncodingOption encodingOption = EncodingOption::UTF8WithoutBOM, FileSystemDriver& driver = LocalFileSystem::GetRootDriver())
        {
            StringWriter<TEncoding, TAllocator> writer;
            LineWriter<TEncoding> lineWriter(writer);

            lines.ForEach([&](const BasicString<TEncoding, TAllocator>& line) {
                lineWriter.WriteLine(line.GetData(), line.GetLength());
            });

            const BasicString<TEncoding, TAllocator> content = writer.ToString();
            AppendFile<TEncoding>(path, BasicStringView<TEncoding>(content), encodingOption, driver);
        }

    private:

        /**
         * \brief Encodes one text fragment for a write or append operation.
         *
         * \tparam TEncoding Encoding of \p content.
         *
         * \param content Text fragment to encode.
         * \param encodingOption Output encoding policy.
         *
         * \return The encoded bytes, including a byte-order mark when selected for
         *         non-empty content.
         */
        template <typename TEncoding>
        static ByteArray EncodeText(BasicStringView<TEncoding> content, EncodingOption encodingOption)
        {
            using StringUtilsImpl = StringUtils<typename BasicStringView<TEncoding>::CharType, typename BasicStringView<TEncoding>::SizeType>;

            const bool isPureAscii = StringUtilsImpl::IsPureAnsi(content.GetData(), content.GetLength());
            if (encodingOption == EncodingOption::Auto)
                encodingOption = isPureAscii ? EncodingOption::Ascii : EncodingOption::Unicode;

            ByteArray binaryContent;
            if (!content.IsEmpty())
            {
                switch (encodingOption)
                {
                    case EncodingOption::Ascii:
                    {
                        binaryContent = Encoders::GetBytes<Encoders::Ascii>(content);
                    }
                    break;

                    case EncodingOption::Unicode:
                    {
                        static constexpr Utf16Char UnicodeBom = 0xfeff;
                        binaryContent.Append(reinterpret_cast<const Byte*>(&UnicodeBom), sizeof(Utf16Char));
                        binaryContent.Append(Encoders::GetBytes<Encoders::Utf16>(content));
                    }
                    break;

                    case EncodingOption::UTF8:
                    {
                        Utf8Char Utf8Bom[] = { static_cast<Utf8Char>(0xEF), static_cast<Utf8Char>(0xBB), static_cast<Utf8Char>(0xBF) };
                        binaryContent.Append(Utf8Bom, sizeof(Utf8Char) * 3);
                    }
                    case EncodingOption::UTF8WithoutBOM:
                    {
                        binaryContent.Append(Encoders::GetBytes<Encoders::Utf8>(content));
                    }
                    break;
                }
            }

            return binaryContent;
        }
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_FILEUTILS_HPP
