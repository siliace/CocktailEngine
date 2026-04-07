#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_FILEWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_FILEWRITER_HPP

#include <Cocktail/Core/IO/Output/Stream/FileOutputStream.hpp>
#include <Cocktail/Core/IO/Output/Writer/OutputStreamWriter.hpp>

namespace Ck
{
    /**
     * \brief Writer that writes text to a file
     *
     * FileWriter wraps a FileOutputStream and allows writing encoded text
     * directly to a file using the specified character encoding.
     *
     * \tparam TEncoding The character encoding to use (default: Encoders::Text)
     * \tparam TAllocator Allocator type for internal streams (default: SizedHeapAllocator<32>)
     */
    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
    class FileWriter : public OutputStreamWriter<TEncoding>
    {
    public:

        /**
         * \brief Construct a FileWriter for the given path
         *
         * Opens the file for writing. By default, it appends if the file exists;
         * setting truncate=true will overwrite the file.
         *
         * \param path Path of the file to write
         * \param truncate If true, truncate the file on open
         * \param driver File system driver to use (default: local file system)
         */
        explicit FileWriter(const Path& path, bool truncate = false, FileSystemDriver* driver = LocalFileSystem::GetRootDriver()) :
            mFileOutputStream(path, truncate, driver),
            OutputStreamWriter<TEncoding>(mFileOutputStream)
        {
            /// Nothing
        }

    private:

        FileOutputStream<TAllocator> mFileOutputStream; ///< Internal file output stream
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_FILEWRITER_HPP
