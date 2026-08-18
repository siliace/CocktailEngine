#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYDIRECTORY_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYDIRECTORY_HPP

#include <CocktailEngine/Core/Memory/SharedPtr.hpp>
#include <CocktailEngine/Core/System/FileSystem/Directory.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemNode.hpp>

namespace Ck::Detail
{
    /**
     * \brief Handle on a directory held in RAM by a MemoryFileSystemDriver
     *
     * \see MemoryFileSystemDriver
     */
    class MemoryDirectory final : public Directory
    {
    public:

        /**
         * \brief Constructor
         *
         * \param node The directory entry to expose
         */
        explicit MemoryDirectory(SharedPtr<MemoryFileSystemNode> node);

        /**
         * \brief Get the content of the directory
         *
         * \return The name of each entry the directory contains
         */
        Array<Path> GetContent() const override;

        const Path& GetPath() const override;

        /**
         * \brief Get the underlying system handle
         *
         * \return Always nullptr, an in-memory directory is not backed by a system object
         */
        void* GetSystemHandle() const override;

    private:

        SharedPtr<MemoryFileSystemNode> mNode; /*!< Entry the handle points to */
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYDIRECTORY_HPP
