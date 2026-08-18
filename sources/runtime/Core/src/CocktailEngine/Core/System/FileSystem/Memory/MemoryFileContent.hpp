#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILECONTENT_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILECONTENT_HPP

#include <CocktailEngine/Core/Utility/ByteArray.hpp>

namespace Ck::Detail
{
    /**
     * \brief Content of a file living in RAM
     *
     * MemoryFileContent owns the bytes of an in-memory file. It keeps the logical
     * size of the file separated from the size of the underlying buffer so that the
     * buffer can grow geometrically: appending to a file stays amortized constant
     * instead of reallocating the whole content on every write.
     *
     * Reads and writes are expressed as absolute offsets, the cursor is owned by the
     * File handle rather than by the content itself, which allows several handles to
     * be opened on the same file.
     */
    class MemoryFileContent
    {
    public:

        /**
         * \brief Get the logical size of the file
         *
         * \return The number of meaningful bytes
         */
        Uint64 GetSize() const;

        /**
         * \brief Read bytes from the content
         *
         * Reading past the end of the file is not an error, it simply yields fewer
         * bytes than requested.
         *
         * \param offset Absolute offset to read from
         * \param buffer Destination buffer
         * \param length Maximum number of bytes to read
         *
         * \return The number of bytes actually read
         */
        Uint64 Read(Uint64 offset, void* buffer, Uint64 length) const;

        /**
         * \brief Write bytes into the content
         *
         * The file grows as needed. Writing past the end of the file leaves a hole
         * filled with zeros, like a sparse file would.
         *
         * \param offset Absolute offset to write to
         * \param buffer Source buffer
         * \param length Number of bytes to write
         */
        void Write(Uint64 offset, const void* buffer, Uint64 length);

        /**
         * \brief Change the logical size of the file
         *
         * Growing the file appends zeros, shrinking it discards the trailing bytes.
         *
         * \param size The new size of the file
         */
        void Resize(Uint64 size);

        /**
         * \brief Discard the content and release the underlying buffer
         */
        void Clear();

    private:

        /**
         * \brief Make sure the underlying buffer can hold a given number of bytes
         *
         * The buffer capacity doubles until it fits \p capacity, so that a sequence
         * of appends performs a logarithmic number of reallocations.
         *
         * \param capacity The number of bytes the buffer must be able to hold
         */
        void Reserve(Uint64 capacity);

        ByteArray mBuffer; /*!< Storage of the content, may be larger than the logical size */
        Uint64 mSize = 0; /*!< Logical size of the file */
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILECONTENT_HPP
