#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILE_HPP

#include <CocktailEngine/Core/Memory/SharedPtr.hpp>
#include <CocktailEngine/Core/System/FileSystem/File.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileOpenFlags.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemNode.hpp>

namespace Ck::Detail
{
    /**
     * \brief Handle on a file held in RAM by a MemoryFileSystemDriver
     *
     * The handle owns the cursor and the access flags, the bytes themselves belong to
     * the entry it points to. Several handles may therefore be opened on the same
     * file, each with its own cursor.
     *
     * \see MemoryFileSystemDriver
     */
    class MemoryFile final : public File
    {
    public:

        /**
         * \brief Constructor
         *
         * \param node The file entry to read from and write to
         * \param flags The access mode the file was opened with
         */
        MemoryFile(SharedPtr<MemoryFileSystemNode> node, const FileOpenFlags& flags);

        unsigned int Read(void* buffer, unsigned int length) override;
        unsigned int Write(const void* buffer, unsigned int length) override;

        Uint64 GetCursor() const override;
        Uint64 SetCursor(FileCursorMode mode, Int64 offset) override;

        std::size_t GetSize() const override;
        const Path& GetPath() const override;

        void Flush() const override;

        /**
         * \brief Get the underlying system handle
         *
         * \return Always nullptr, an in-memory file is not backed by a system object
         */
        void* GetSystemHandle() const override;

    private:

        SharedPtr<MemoryFileSystemNode> mNode; /*!< Entry the handle points to */
        FileOpenFlags mFlags; /*!< Access mode the file was opened with */
        Uint64 mCursor; /*!< Position of the cursor, in bytes from the beginning */
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILE_HPP
