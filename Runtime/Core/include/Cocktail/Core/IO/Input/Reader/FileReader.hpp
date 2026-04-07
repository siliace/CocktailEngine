#ifndef COCKTAIL_CORE_IO_INPUT_READER_FILEREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_FILEREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>

namespace Ck
{
    /**
     * \brief Reader for files on disk
     *
     * FileReader provides an easy way to read text from a file using a specified
     * encoding. It internally uses a FileInputStream and InputStreamReader to
     * handle low-level file access and optional transcoding.
     *
     * This class supports any encoding supported by InputStreamReader, such as
     * ASCII, UTF-8, UTF-16, or UTF-32.
     *
     * \tparam TEncoding The character encoding used to read the file
     * \tparam TAllocator The allocator used internally for buffering
     */
    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
    class FileReader : public InputStreamReader<TEncoding, TAllocator>
    {
    public:

        /**
         * \brief Character type associated with the encoding
         */
        using CharType = typename InputStreamReader<TEncoding, FileInputStream<TAllocator>>::CharType;

        /**
         * \brief Size type associated with the encoding
         */
        using SizeType = typename InputStreamReader<TEncoding, FileInputStream<TAllocator>>::SizeType;

        /**
         * \brief Encoding type used by this reader
         */
        using EncodingType = typename InputStreamReader<TEncoding, FileInputStream<TAllocator>>::EncodingType;

        /**
         * \brief Constructor
         *
         * Opens the file at the specified \p path using the optional \p driver.
         * If no driver is provided, the local file system is used by default.
         *
         * \param path Path to the file to read
         * \param driver Optional file system driver
         */
        explicit FileReader(const Path& path, FileSystemDriver* driver = LocalFileSystem::GetRootDriver()) :
            mFileInputStream(path, driver),
            InputStreamReader<TEncoding, TAllocator>(mFileInputStream)
        {
            /// Nothing to do
        }

        /**
         * \brief Get the underlying file object
         *
         * Allows direct access to the File instance managed by FileInputStream.
         *
         * \return Pointer to the File object, or nullptr if the file is not open
         */
        File* GetFile() const
        {
            return mFileInputStream->GetFile();
        }

    private:

        /** \brief File input stream used internally to read raw bytes */
        FileInputStream<TAllocator> mFileInputStream;
    };

    /** \brief FileReader using ASCII encoding */
    using AsciiFileReader = FileReader<Encoders::Ascii>;

    /** \brief FileReader using UTF-8 encoding */
    using Utf8FileReader = FileReader<Encoders::Utf8>;

    /** \brief FileReader using UTF-16 encoding */
    using Utf16FileReader = FileReader<Encoders::Utf16>;

    /** \brief FileReader using UTF-32 encoding */
    using Utf32FileReader = FileReader<Encoders::Utf32>;
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_FILEREADER_HPP
