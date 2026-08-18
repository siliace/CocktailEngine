#include <system_error>

#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryDirectory.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryDirectoryIterator.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFile.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemDriver.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemNode.hpp>

namespace Ck
{
    namespace
    {
        /**
         * \brief Bring a path to the canonical form used as key in the entry table
         *
         * The virtual filesystem owns a single implicit root, so the root component
         * of the path is dropped and every separator is turned into '/'. Leading and
         * trailing separators are stripped as well, which makes "/a/b", "a/b/" and
         * "a\\b" designate the same entry.
         *
         * \param path The path to normalize
         *
         * \return The normalized path, empty when it designates the root
         */
        Path NormalizePath(const Path& path)
        {
            const Path generic = path.ToFormat(Path::Format::Generic);
            const String pathString = generic.ToString();

            Path::SizeType first = generic.GetRoot().GetLength();
            while (first < pathString.GetLength() && Path::IsSeparator(pathString[first]))
                first++;

            Path::SizeType last = pathString.GetLength();
            while (last > first && Path::IsSeparator(pathString[last - 1]))
                last--;

            if (first >= last)
                return Path::Empty;

            return Path::Parse(pathString.SubString(first, last - first), Path::Format::Generic);
        }

        /**
         * \brief Extract the name of the last element of a path
         *
         * \param path A normalized path
         *
         * \return The name of the entry, empty when \p path designates the root
         */
        String GetNodeName(const Path& path)
        {
            const String pathString = path.ToString();
            const String parentString = path.GetParent().ToString();

            Path::SizeType first = parentString.GetLength();
            while (first < pathString.GetLength() && Path::IsSeparator(pathString[first]))
                first++;

            Path::SizeType last = pathString.GetLength();
            while (last > first && Path::IsSeparator(pathString[last - 1]))
                last--;

            return first < last ? pathString.SubString(first, last - first) : String::Empty;
        }

        /**
         * \brief Build the normalized path of a child entry
         *
         * \param parent The normalized path of the parent directory
         * \param name The name of the child entry
         *
         * \return The normalized path of the child
         */
        Path MakeChildPath(const Path& parent, const String& name)
        {
            return NormalizePath(Path::Merge(parent, name));
        }

        [[noreturn]] void ThrowError(std::errc error)
        {
            throw std::system_error(std::make_error_code(error));
        }
    }

    MemoryFileSystemDriver::MemoryFileSystemDriver()
    {
        Clear();
    }

    MemoryFileSystemDriver::~MemoryFileSystemDriver() = default;

    MemoryFileSystemDriver::MemoryFileSystemDriver(MemoryFileSystemDriver&& other) noexcept = default;

    MemoryFileSystemDriver& MemoryFileSystemDriver::operator=(MemoryFileSystemDriver&& other) noexcept = default;

    bool MemoryFileSystemDriver::IsFile(const Path& path) const
    {
        const SharedPtr<Node> node = FindNode(path);
        return node.Get() != nullptr && node->Type == PathType::File;
    }

    bool MemoryFileSystemDriver::IsDirectory(const Path& path) const
    {
        const SharedPtr<Node> node = FindNode(path);
        return node.Get() != nullptr && node->Type == PathType::Directory;
    }

    void MemoryFileSystemDriver::CreateFile(const Path& path)
    {
        const Path normalized = NormalizePath(path);

        const SharedPtr<Node> node = FindNode(normalized);
        if (node.Get() != nullptr)
        {
            // Creating an already existing file is not an error, like the local driver
            if (node->Type != PathType::File)
                ThrowError(std::errc::is_a_directory);

            return;
        }

        CreateNode(normalized, PathType::File);
    }

    void MemoryFileSystemDriver::CreateDirectory(const Path& path)
    {
        const Path normalized = NormalizePath(path);
        if (FindNode(normalized).Get() != nullptr)
            ThrowError(std::errc::file_exists);

        CreateNode(normalized, PathType::Directory);
    }

    UniquePtr<DirectoryIterator> MemoryFileSystemDriver::CreateDirectoryIterator(const Path& path)
    {
        const Path normalized = NormalizePath(path);

        const SharedPtr<Node> node = FindNode(normalized);
        if (node.Get() == nullptr || node->Type != PathType::Directory)
            return nullptr;

        Detail::MemoryDirectoryIterator::EntryArray entries;
        entries.Reserve(node->Children.GetSize());

        for (const String& child : node->Children)
        {
            SharedPtr<Node> entry = FindNode(MakeChildPath(normalized, child));
            if (entry.Get() != nullptr)
                entries.Add(Move(entry));
        }

        node->LastAccessTime = Instant::Now();

        return MakeUnique<Detail::MemoryDirectoryIterator>(Move(entries));
    }

    UniquePtr<File> MemoryFileSystemDriver::OpenFile(const Path& path, const FileOpenFlags& flags)
    {
        if (!(flags & FileOpenFlagBits::Read) && !(flags & FileOpenFlagBits::Write))
            ThrowError(std::errc::invalid_argument);

        const Path normalized = NormalizePath(path);

        SharedPtr<Node> node = FindNode(normalized);
        if (node.Get() != nullptr && node->Type != PathType::File)
            ThrowError(std::errc::is_a_directory);

        if (node.Get() == nullptr)
        {
            const bool create = (flags & FileOpenFlagBits::Write) && !(flags & FileOpenFlagBits::Existing);
            if (!create)
                ThrowError(std::errc::no_such_file_or_directory);

            node = CreateNode(normalized, PathType::File);
        }
        else if ((flags & FileOpenFlagBits::Write) && (flags & FileOpenFlagBits::Truncate))
        {
            node->Content.Clear();
            node->LastChangeTime = Instant::Now();
        }

        return MakeUnique<Detail::MemoryFile>(Move(node), flags);
    }

    UniquePtr<Directory> MemoryFileSystemDriver::OpenDirectory(const Path& path)
    {
        return MakeUnique<Detail::MemoryDirectory>(RequireDirectory(path));
    }

    void MemoryFileSystemDriver::CopyFile(const Path& source, const Path& destination, const FileCopyOptions& options)
    {
        const Path normalizedSource = NormalizePath(source);
        const Path normalizedDestination = NormalizePath(destination);

        const SharedPtr<Node> sourceNode = RequireFile(normalizedSource);
        if (normalizedSource == normalizedDestination)
            return;

        const SharedPtr<Node> destinationNode = PrepareFileDestination(normalizedDestination, options.Overwrite);

        destinationNode->Content = sourceNode->Content;

        const Instant now = Instant::Now();
        if (options.PreserveMetadata)
        {
            destinationNode->CreationTime = sourceNode->CreationTime;
            destinationNode->LastChangeTime = sourceNode->LastChangeTime;
            destinationNode->LastAccessTime = sourceNode->LastAccessTime;
        }
        else
        {
            destinationNode->Touch(now);
        }

        sourceNode->LastAccessTime = now;
    }

    void MemoryFileSystemDriver::CopyDirectory(const Path& source, const Path& destination, const DirectoryCopyOptions& options)
    {
        const Path normalizedSource = NormalizePath(source);
        const Path normalizedDestination = NormalizePath(destination);

        const SharedPtr<Node> sourceNode = RequireDirectory(normalizedSource);
        if (normalizedSource == normalizedDestination)
            return;

        // Copying a directory into itself would recurse forever
        if (IsInSubTree(normalizedDestination, normalizedSource))
            ThrowError(std::errc::invalid_argument);

        SharedPtr<Node> destinationNode = FindNode(normalizedDestination);
        if (destinationNode.Get() == nullptr)
        {
            destinationNode = CreateNode(normalizedDestination, PathType::Directory);
        }
        else if (destinationNode->Type != PathType::Directory)
        {
            ThrowError(std::errc::not_a_directory);
        }

        // The recursion mutates the children of the source, walk a snapshot of them
        const Array<String> children = sourceNode->Children;
        for (const String& child : children)
        {
            const Path childSource = MakeChildPath(normalizedSource, child);
            const Path childDestination = MakeChildPath(normalizedDestination, child);

            const SharedPtr<Node> childNode = FindNode(childSource);
            if (childNode.Get() == nullptr)
                continue;

            if (childNode->Type == PathType::Directory)
            {
                if (!options.Recursive)
                    continue;

                CopyDirectory(childSource, childDestination, options);
            }
            else
            {
                if (options.OnlyStructure)
                    continue;

                CopyFile(childSource, childDestination, options);
            }
        }

        if (options.PreserveMetadata)
        {
            destinationNode->CreationTime = sourceNode->CreationTime;
            destinationNode->LastChangeTime = sourceNode->LastChangeTime;
            destinationNode->LastAccessTime = sourceNode->LastAccessTime;
        }
    }

    void MemoryFileSystemDriver::MoveFile(const Path& source, const Path& destination, const FileMoveOptions& options)
    {
        const Path normalizedSource = NormalizePath(source);
        const Path normalizedDestination = NormalizePath(destination);

        const SharedPtr<Node> sourceNode = RequireFile(normalizedSource);
        if (normalizedSource == normalizedDestination)
            return;

        const SharedPtr<Node> destinationNode = FindNode(normalizedDestination);
        if (destinationNode.Get() != nullptr)
        {
            if (destinationNode->Type != PathType::File)
                ThrowError(std::errc::is_a_directory);

            if (!options.Overwrite)
                ThrowError(std::errc::file_exists);
        }

        // Nothing must be unlinked before the destination is known to be reachable
        RequireDirectory(normalizedDestination.GetParent());

        if (destinationNode.Get() != nullptr)
            DetachNode(destinationNode);

        DetachNode(sourceNode);
        ReKeySubTree(sourceNode, normalizedDestination);
        LinkNode(sourceNode);
    }

    void MemoryFileSystemDriver::MoveDirectory(const Path& source, const Path& destination, const DirectoryMoveOptions& options)
    {
        const Path normalizedSource = NormalizePath(source);
        const Path normalizedDestination = NormalizePath(destination);

        const SharedPtr<Node> sourceNode = RequireDirectory(normalizedSource);
        if (normalizedSource.IsEmpty())
            ThrowError(std::errc::device_or_resource_busy);

        if (normalizedSource == normalizedDestination)
            return;

        // Moving a directory inside itself would detach it from the tree
        if (IsInSubTree(normalizedDestination, normalizedSource))
            ThrowError(std::errc::invalid_argument);

        const SharedPtr<Node> destinationNode = FindNode(normalizedDestination);
        if (destinationNode.Get() != nullptr)
        {
            if (destinationNode->Type != PathType::Directory)
                ThrowError(std::errc::not_a_directory);

            if (!options.Overwrite)
                ThrowError(std::errc::file_exists);

            if (!destinationNode->Children.IsEmpty())
                ThrowError(std::errc::directory_not_empty);
        }

        // Nothing must be unlinked before the destination is known to be reachable
        RequireDirectory(normalizedDestination.GetParent());

        if (destinationNode.Get() != nullptr)
            DetachNode(destinationNode);

        DetachNode(sourceNode);
        ReKeySubTree(sourceNode, normalizedDestination);
        LinkNode(sourceNode);
    }

    void MemoryFileSystemDriver::RemoveFile(const Path& path, const FileRemoveOptions& options)
    {
        const SharedPtr<Node> node = FindNode(path);
        if (node.Get() == nullptr)
        {
            if (options.IgnoreMissing)
                return;

            ThrowError(std::errc::no_such_file_or_directory);
        }

        if (node->Type != PathType::File)
            ThrowError(std::errc::is_a_directory);

        // An in-memory file has no read-only attribute, options.Force is a no-op
        DetachNode(node);
    }

    void MemoryFileSystemDriver::RemoveDirectory(const Path& path, const DirectoryRemoveOptions& options)
    {
        const Path normalized = NormalizePath(path);

        const SharedPtr<Node> node = FindNode(normalized);
        if (node.Get() == nullptr)
        {
            if (options.IgnoreMissing)
                return;

            ThrowError(std::errc::no_such_file_or_directory);
        }

        if (node->Type != PathType::Directory)
            ThrowError(std::errc::not_a_directory);

        // The root is the anchor of the whole tree, it can only be emptied
        if (normalized.IsEmpty())
            ThrowError(std::errc::device_or_resource_busy);

        if (!node->Children.IsEmpty() && !options.Recursive)
            ThrowError(std::errc::directory_not_empty);

        DropSubTree(node);
        DetachNode(node);
    }

    Path MemoryFileSystemDriver::MakeCanonical(const Path& path)
    {
        return TryMakeCanonical(path).GetOrThrow<std::system_error>(std::make_error_code(std::errc::no_such_file_or_directory));
    }

    Optional<Path> MemoryFileSystemDriver::TryMakeCanonical(const Path& path)
    {
        const Path normalized = NormalizePath(path);
        if (FindNode(normalized).Get() == nullptr)
            return Optional<Path>::Empty();

        // Root the result to make it explicit that it lives in the virtual filesystem
        String canonical;
        canonical.Append(CK_CHAR('/'));
        canonical.Append(normalized.ToString());

        return Optional<Path>::Of(Path::Parse(canonical, Path::Format::Generic));
    }

    PathInfo MemoryFileSystemDriver::GetPathInfo(const Path& path) const
    {
        const SharedPtr<Node> node = FindNode(path);
        if (node.Get() == nullptr)
            return {};

        return node->GetPathInfo();
    }

    void MemoryFileSystemDriver::Clear()
    {
        mNodes.Clear();

        SharedPtr<Node> root = MakeShared<Node>();
        root->Type = PathType::Directory;
        root->Touch(Instant::Now());

        mNodes.Put(Path::Empty, Move(root));
    }

    Uint64 MemoryFileSystemDriver::GetUsedSize() const
    {
        Uint64 usedSize = 0;
        mNodes.ForEach([&usedSize](const SharedPtr<Node>& node) {
            if (node->Type == PathType::File)
                usedSize += node->Content.GetSize();
        });

        return usedSize;
    }

    SharedPtr<Detail::MemoryFileSystemNode> MemoryFileSystemDriver::FindNode(const Path& path) const
    {
        if (const SharedPtr<Node>* node = mNodes.Get(NormalizePath(path)))
            return *node;

        return nullptr;
    }

    SharedPtr<Detail::MemoryFileSystemNode> MemoryFileSystemDriver::RequireDirectory(const Path& path) const
    {
        SharedPtr<Node> node = FindNode(path);
        if (node.Get() == nullptr)
            ThrowError(std::errc::no_such_file_or_directory);

        if (node->Type != PathType::Directory)
            ThrowError(std::errc::not_a_directory);

        return node;
    }

    SharedPtr<Detail::MemoryFileSystemNode> MemoryFileSystemDriver::RequireFile(const Path& path) const
    {
        SharedPtr<Node> node = FindNode(path);
        if (node.Get() == nullptr)
            ThrowError(std::errc::no_such_file_or_directory);

        if (node->Type != PathType::File)
            ThrowError(std::errc::is_a_directory);

        return node;
    }

    SharedPtr<Detail::MemoryFileSystemNode> MemoryFileSystemDriver::CreateNode(const Path& path, PathType type)
    {
        if (path.IsEmpty())
            ThrowError(std::errc::file_exists);

        // Parent directories are never created implicitly, like the local driver
        const SharedPtr<Node> parent = RequireDirectory(path.GetParent());

        const Instant now = Instant::Now();

        SharedPtr<Node> node = MakeShared<Node>();
        node->NodePath = path;
        node->Type = type;
        node->Touch(now);

        mNodes.Put(path, node);

        parent->Children.Add(GetNodeName(path));
        parent->LastChangeTime = now;

        return node;
    }

    SharedPtr<Detail::MemoryFileSystemNode> MemoryFileSystemDriver::PrepareFileDestination(const Path& path, bool overwrite)
    {
        const SharedPtr<Node> node = FindNode(path);
        if (node.Get() == nullptr)
            return CreateNode(path, PathType::File);

        if (node->Type != PathType::File)
            ThrowError(std::errc::is_a_directory);

        if (!overwrite)
            ThrowError(std::errc::file_exists);

        node->Content.Clear();

        return node;
    }

    void MemoryFileSystemDriver::DetachNode(const SharedPtr<Node>& node)
    {
        const Path path = node->NodePath;

        const SharedPtr<Node> parent = FindNode(path.GetParent());
        if (parent.Get() != nullptr && parent.Get() != node.Get())
        {
            parent->Children.Remove(GetNodeName(path));
            parent->LastChangeTime = Instant::Now();
        }

        mNodes.Remove(path);
    }

    void MemoryFileSystemDriver::LinkNode(const SharedPtr<Node>& node)
    {
        const SharedPtr<Node> parent = FindNode(node->NodePath.GetParent());
        if (parent.Get() == nullptr || parent.Get() == node.Get())
            return;

        parent->Children.Add(GetNodeName(node->NodePath));
        parent->LastChangeTime = Instant::Now();
    }

    void MemoryFileSystemDriver::ReKeySubTree(const SharedPtr<Node>& node, const Path& destination)
    {
        // The children are still reachable from the previous path, keep it around
        const Path origin = node->NodePath;

        mNodes.Remove(origin);

        node->NodePath = destination;
        mNodes.Put(destination, node);

        if (node->Type != PathType::Directory)
            return;

        const Array<String> children = node->Children;
        for (const String& child : children)
        {
            const SharedPtr<Node> childNode = FindNode(MakeChildPath(origin, child));
            if (childNode.Get() != nullptr)
                ReKeySubTree(childNode, MakeChildPath(destination, child));
        }
    }

    void MemoryFileSystemDriver::DropSubTree(const SharedPtr<Node>& node)
    {
        if (node->Type != PathType::Directory)
            return;

        const Array<String> children = node->Children;
        for (const String& child : children)
        {
            const SharedPtr<Node> childNode = FindNode(MakeChildPath(node->NodePath, child));
            if (childNode.Get() == nullptr)
                continue;

            DropSubTree(childNode);
            mNodes.Remove(childNode->NodePath);
        }

        node->Children.Clear();
    }

    bool MemoryFileSystemDriver::IsInSubTree(const Path& path, const Path& root)
    {
        // Everything lives under the root of the virtual filesystem
        if (root.IsEmpty())
            return true;

        if (path == root)
            return true;

        const String pathString = path.ToString();
        const String rootString = root.ToString();

        if (pathString.GetLength() <= rootString.GetLength())
            return false;

        if (!pathString.StartsWith(rootString))
            return false;

        // Guard against "ab" being reported as a descendant of "a"
        return Path::IsSeparator(pathString[rootString.GetLength()]);
    }
}
