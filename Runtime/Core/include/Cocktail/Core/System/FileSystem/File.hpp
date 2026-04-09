#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILE_HPP

#include <Cocktail/Core/System/FileSystem/FileCursorMode.hpp>
#include <Cocktail/Core/System/FileSystem/Path.hpp>

namespace Ck
{
    /**
     * \brief Abstract interface for file manipulation
     *
     * File provides a platform-independent interface to perform
     * basic file operations such as reading, writing, seeking,
     * and retrieving file metadata.
     *
     * Implementations may wrap operating system file handles or
     * custom virtual file systems.
     */
    class File
    {
    public:

        /**
         * \brief Destructor
         */
        virtual ~File() = default;

        /**
         * \brief Reads data from the file
         *
         * Attempts to read up to \p length bytes from the file into
         * the provided buffer, starting at the current cursor position.
         *
         * \param buffer Destination buffer to store the read data
         * \param length Maximum number of bytes to read
         *
         * \return Number of bytes actually read
         */
        virtual unsigned int Read(void* buffer, unsigned int length) = 0;

        /**
         * \brief Writes data to the file
         *
         * Writes up to \p length bytes from the provided buffer into
         * the file, starting at the current cursor position.
         *
         * \param buffer Source buffer containing the data to write
         * \param length Number of bytes to write
         *
         * \return Number of bytes actually written
         */
        virtual unsigned int Write(const void* buffer, unsigned int length) = 0;

        /**
         * \brief Gets the current cursor position
         *
         * \return Current position of the file cursor in bytes from the beginning
         */
        virtual Uint64 GetCursor() const = 0;

        /**
         * \brief Sets the cursor position
         *
         * Moves the file cursor according to the specified mode and offset.
         *
         * \param mode Reference position (e.g., beginning, current, end)
         * \param offset Offset in bytes relative to the reference position
         *
         * \return New cursor position in bytes from the beginning
         */
        virtual Uint64 SetCursor(FileCursorMode mode, Int64 offset) = 0;

        /**
         * \brief Gets the size of the file
         *
         * \return Total size of the file in bytes
         */
        virtual std::size_t GetSize() const = 0;

        /**
         * \brief Gets the file path
         *
         * \return Reference to the path associated with this file
         */
        virtual const Path& GetPath() const = 0;

        /**
         * \brief Flushes pending writes to the storage medium
         *
         * Ensures that any buffered data is written to the underlying
         * storage device.
         */
        virtual void Flush() const = 0;

        /**
         * \brief Gets the underlying system handle
         *
         * Returns a pointer to the native file handle used by the
         * implementation (e.g., FILE*, file descriptor, HANDLE).
         *
         * \return Opaque pointer to the system-specific file handle
         */
        virtual void* GetSystemHandle() const = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILE_HPP
