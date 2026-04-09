#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYITERATOR_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYITERATOR_HPP

#include <Cocktail/Core/System/FileSystem/Path.hpp>
#include <Cocktail/Core/System/FileSystem/PathInfo.hpp>

namespace Ck
{
    /**
     * \brief Iterator interface for enumerating directory entries
     *
     * DirectoryIterator provides a streaming interface to iterate over
     * the contents of a directory without loading all entries into memory.
     *
     * It is typically obtained from a Directory object and allows sequential
     * access to each entry along with optional metadata.
     *
     * \see Directory
     * \see Path
     * \see PathInfo
     */
    class DirectoryIterator
    {
    public:

        /**
         * \brief Destructor
         */
        virtual ~DirectoryIterator() = default;

        /**
         * \brief Advances the iterator to the next entry
         */
        virtual void Next() = 0;

        /**
         * \brief Gets the path of the current entry
         *
         * \return Path of the current directory entry
         */
        virtual const Path& GetPath() const = 0;

        /**
         * \brief Gets detailed information about the current entry
         *
         * Returns a PathInfo object containing metadata associated with
         * the current directory entry, such as file type, size, timestamps,
         * and permissions (depending on platform support).
         *
         * \return PathInfo structure describing the current directory entry
         */
        virtual PathInfo GetPathInfo() const = 0;

        /**
         * \brief Checks whether the iterator has reached the end
         *
         * \return True if no more entries are available
         */
        virtual bool IsEnd() const = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORYITERATOR_HPP
