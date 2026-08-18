#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILESYSTEMDRIVER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILESYSTEMDRIVER_HPP

#include <CocktailEngine/Core/Export.hpp>
#include <CocktailEngine/Core/HashMap.hpp>
#include <CocktailEngine/Core/Memory/SharedPtr.hpp>
#include <CocktailEngine/Core/System/FileSystem/FileSystemDriver.hpp>

namespace Ck
{
    namespace Detail
    {
        struct MemoryFileSystemNode;
    }

    /**
     * \class MemoryFileSystemDriver
     * \brief Filesystem driver holding a whole virtual filesystem in RAM
     *
     * MemoryFileSystemDriver implements the FileSystemDriver interface on top of an
     * in-memory tree of entries. Nothing is ever written to the underlying storage
     * medium, which makes it suitable for scratch data, caches, tests or as a
     * sandbox mounted next to the local filesystem.
     *
     * \par Path handling
     * The driver owns a single implicit root directory. Every path is interpreted
     * relative to that root, so \c "/textures/wood.png" and \c "textures/wood.png"
     * designate the same entry. Separators are normalized, meaning \c "a/b" and
     * \c "a\\b" are equivalent as well. MakeCanonical() returns the normalized path
     * prefixed by \c '/' to make it explicit that it is rooted in the virtual
     * filesystem and not in the local one.
     *
     * \par Semantics
     * Parent directories are never created implicitly: CreateFile(),
     * CreateDirectory() and the creating flavours of OpenFile() require the parent
     * directory to exist, exactly like the local driver does. Errors are reported by
     * throwing \c std::system_error carrying a portable \c std::errc code.
     *
     * \par Lifetime of the entries
     * A File or Directory handle keeps its entry alive. Removing an entry that is
     * still open unlinks it from the tree without invalidating the handles, which
     * mirrors the POSIX unlink behavior.
     *
     * \par Thread safety
     * This class is not thread-safe. External synchronisation is required when
     * concurrent reads and writes are needed.
     *
     * \see FileSystemDriver
     * \see StorageService
     */
    class COCKTAILENGINE_CORE_API MemoryFileSystemDriver : public FileSystemDriver
    {
    public:

        /**
         * \brief Constructor
         *
         * Creates a virtual filesystem containing nothing but an empty root directory.
         */
        MemoryFileSystemDriver();

        /**
         * \brief Destructor
         */
        ~MemoryFileSystemDriver() override;

        MemoryFileSystemDriver(const MemoryFileSystemDriver& other) = delete;
        MemoryFileSystemDriver(MemoryFileSystemDriver&& other) noexcept;
        MemoryFileSystemDriver& operator=(const MemoryFileSystemDriver& other) = delete;
        MemoryFileSystemDriver& operator=(MemoryFileSystemDriver&& other) noexcept;

        bool IsFile(const Path& path) const override;
        bool IsDirectory(const Path& path) const override;

        void CreateFile(const Path& path) override;
        void CreateDirectory(const Path& path) override;

        /**
         * \brief Create a directory iterator
         *
         * \param path The path of the directory
         *
         * \return Unique pointer to a DirectoryIterator object, or an empty pointer
         *         when \p path is not a directory
         */
        UniquePtr<DirectoryIterator> CreateDirectoryIterator(const Path& path) override;

        UniquePtr<File> OpenFile(const Path& path, const FileOpenFlags& flags) override;
        UniquePtr<Directory> OpenDirectory(const Path& path) override;

        void CopyFile(const Path& source, const Path& destination, const FileCopyOptions& options) override;
        void CopyDirectory(const Path& source, const Path& destination, const DirectoryCopyOptions& options) override;
        void MoveFile(const Path& source, const Path& destination, const FileMoveOptions& options) override;
        void MoveDirectory(const Path& source, const Path& destination, const DirectoryMoveOptions& options) override;
        void RemoveFile(const Path& path, const FileRemoveOptions& options) override;
        void RemoveDirectory(const Path& path, const DirectoryRemoveOptions& options) override;

        Path MakeCanonical(const Path& path) override;
        Optional<Path> TryMakeCanonical(const Path& path) override;

        PathInfo GetPathInfo(const Path& path) const override;

        /**
         * \brief Drop every entry and restore an empty root directory
         *
         * \note Handles opened on the discarded entries stay valid, they simply are
         *       not reachable through the driver anymore.
         */
        void Clear();

        /**
         * \brief Get the number of bytes held by the files of the virtual filesystem
         *
         * \return The cumulated size of every file
         */
        Uint64 GetUsedSize() const;

    private:

        using Node = Detail::MemoryFileSystemNode;

        /**
         * \brief Find the entry designated by a path
         *
         * \param path Path to resolve, in any format
         *
         * \return The entry, or an empty pointer when the path does not exist
         */
        SharedPtr<Node> FindNode(const Path& path) const;

        /**
         * \brief Find the entry designated by a path, requiring it to be a directory
         *
         * \param path Path to resolve, in any format
         *
         * \return The directory entry
         */
        SharedPtr<Node> RequireDirectory(const Path& path) const;

        /**
         * \brief Find the entry designated by a path, requiring it to be a file
         *
         * \param path Path to resolve, in any format
         *
         * \return The file entry
         */
        SharedPtr<Node> RequireFile(const Path& path) const;

        /**
         * \brief Create an entry and link it into its parent directory
         *
         * \param path Normalized path of the entry to create
         * \param type Kind of entry to create
         *
         * \return The created entry
         */
        SharedPtr<Node> CreateNode(const Path& path, PathType type);

        /**
         * \brief Get the file entry a copy or a move should write to
         *
         * \param path Normalized path of the destination
         * \param overwrite Whether an already existing destination may be replaced
         *
         * \return The truncated destination entry
         */
        SharedPtr<Node> PrepareFileDestination(const Path& path, bool overwrite);

        /**
         * \brief Unlink an entry from its parent directory and from the entry table
         *
         * \param node Entry to unlink
         */
        void DetachNode(const SharedPtr<Node>& node);

        /**
         * \brief Link an entry into the parent directory designated by its own path
         *
         * \param node Entry to link
         */
        void LinkNode(const SharedPtr<Node>& node);

        /**
         * \brief Move an entry and all its descendants to another path
         *
         * \param node Entry to relocate
         * \param destination Normalized path the entry must be reachable from
         */
        void ReKeySubTree(const SharedPtr<Node>& node, const Path& destination);

        /**
         * \brief Remove every descendant of an entry from the entry table
         *
         * \param node Entry whose descendants must be dropped
         */
        void DropSubTree(const SharedPtr<Node>& node);

        /**
         * \brief Tell whether a path is located inside a subtree
         *
         * \param path Normalized path to test
         * \param root Normalized path of the subtree root
         *
         * \return True when \p path is \p root or one of its descendants
         */
        static bool IsInSubTree(const Path& path, const Path& root);

        HashMap<Path, SharedPtr<Node>> mNodes; /*!< Every entry of the virtual filesystem, keyed by normalized path */
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_MEMORY_MEMORYFILESYSTEMDRIVER_HPP
