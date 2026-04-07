#ifndef COCKTAIL_CORE_IO_INPUT_STREAM_FILEINPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_INPUT_STREAM_FILEINPUTSTREAM_HPP

#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystem.hpp>

namespace Ck
{
    /**
     * \brief Input stream that reads bytes from a file
     *
     * FileInputStream wraps a file object and exposes it as an InputStream,
     * providing read access, cursor operations, and file size querying.
     *
     * \tparam TAllocator Allocator type (not used for the file itself, only for compatibility)
     */
    template <typename TAllocator = SizedHeapAllocator<32>>
    class FileInputStream : public InputStream<TAllocator>
    {
    public:

        /**
         * \brief Allocator type
         */
        using AllocatorType = TAllocator;

        /**
         * \brief Size type used for reading operations
         */
        using SizeType = typename InputStream<TAllocator>::SizeType;

        /**
         * \brief Constructor
         *
         * Opens a file for reading using the provided filesystem driver.
         *
         * \param path Path to the file to open
         * \param driver Filesystem driver to use (defaults to the local filesystem)
         */
        explicit FileInputStream(const Path& path, FileSystemDriver* driver = LocalFileSystem::GetRootDriver()) :
            mFile(driver->OpenFile(path, FileOpenFlagBits::Read))
        {
            /// Nothing to initialize
        }

        /**
         * \brief Check if the stream supports cursor operations
         *
         * \return Always true for FileInputStream
         */
        bool HasCursor() const override
        {
            return true;
        }

        /**
         * \brief Read bytes from the file
         *
         * Copies up to \p length bytes from the current file position into \p buffer.
         *
         * \param buffer Destination buffer
         * \param length Maximum number of bytes to read
         *
         * \return Number of bytes actually read
         */
        SizeType Read(Byte* buffer, SizeType length) override
        {
            return mFile->Read(buffer, length);
        }

        /**
         * \brief Move the read cursor to a specific position
         *
         * \param position Number of bytes to move relative to the current position
         *
         * \return The new cursor position
         */
        Uint64 Seek(Uint64 position) override
        {
            return mFile->SetCursor(FileCursorMode::Current, position);
        }

        /**
         * \brief Get the current cursor position
         *
         * \return Cursor position in bytes from the beginning of the file
         */
        Uint64 Tell() const override
        {
            return mFile->GetCursor();
        }

        /**
         * \brief Reset the read cursor to the beginning of the file
         */
        void Rewind() override
        {
            mFile->SetCursor(FileCursorMode::Begin, 0);
        }

        /**
         * \brief Get the total size of the file
         *
         * \return Size of the file in bytes
         */
        SizeType GetSize() const override
        {
            return mFile->GetSize();
        }

        /**
         * \brief Check if the end of the file has been reached
         *
         * \return True if the cursor is at the end of the file
         */
        bool IsEof() const override
        {
            return mFile->GetCursor() == mFile->GetSize();
        }

        /**
         * \brief Access the underlying File object
         *
         * \return Pointer to the file
         */
        File* GetFile() const
        {
            return mFile.Get();
        }

    private:

        UniquePtr<File> mFile; ///< Owned file object
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_STREAM_FILEINPUTSTREAM_HPP
