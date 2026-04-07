#ifndef COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystem.hpp>

namespace Ck
{
    /**
     * \brief Output stream that writes bytes to a file
     *
     * FileOutputStream provides an interface to write raw bytes to a file
     * using a File abstraction. It supports optional truncation of the file
     * on opening and ensures all writes are directed to the underlying file.
     *
     * \tparam TAllocator Allocator type used for size types and internal management
     */
    template <typename TAllocator = SizedHeapAllocator<32>>
    class FileOutputStream : public OutputStream<TAllocator>
    {
    public:

        /**
         * \brief Allocator type used for size calculations
         */
        using AllocatorType = TAllocator;

        /**
         * \brief Size type used for write operations
         */
        using SizeType = typename AllocatorType::SizeType;

        /**
         * \brief Constructor
         *
         * Opens a file for writing. Optionally truncates the file if it already exists.
         *
         * \param path Path to the file to open
         * \param truncate True to truncate the file if it exists, false to preserve existing content
         * \param driver File system driver used to open the file (defaults to local file system)
         */
        explicit FileOutputStream(const Path& path, bool truncate = false, FileSystemDriver* driver = LocalFileSystem::GetRootDriver()) :
            mFile(driver->OpenFile(path, truncate ? FileOpenFlagBits::Write | FileOpenFlagBits::Truncate : FileOpenFlagBits::Write))
        {
            /// Nothing to initialize
        }

        /**
         * \brief Write bytes to the file
         *
         * Writes up to \p length bytes from \p data to the underlying file.
         *
         * \param data Pointer to the source bytes
         * \param length Number of bytes to write
         *
         * \return Number of bytes actually written
         */
        SizeType Write(const Byte* data, SizeType length) override
        {
            return mFile->Write(data, length);
        }

        /**
         * \brief Flush the file
         *
         * Ensures that all pending writes are physically written to the file.
         */
        void Flush() override
        {
            mFile->Flush();
        }

        /**
         * \brief Get the underlying file object
         *
         * \return Pointer to the File object used internally
         */
        File* GetFile() const
        {
            return mFile.Get();
        }

    private:

        UniquePtr<File> mFile; ///< Internal file object
    };
}

#endif // COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP
