#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORY_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORY_HPP

#include <Cocktail/Core/System/FileSystem/Path.hpp>

namespace Ck
{
    /**
     * \brief Abstract representation of a directory in a file system
     *
     * Directory defines a generic interface for interacting with a directory
     * regardless of the underlying file system implementation (local, virtual,
     * remote, etc.). It provides access to the directory path, its contents,
     * and an optional system-specific handle.
     */
	class Directory
	{
	public:

	    /**
         * \brief Destructor
         */
	    virtual ~Directory() = default;

	    /**
         * \brief Get the content of the directory
         *
         * Retrieves the list of entries contained in the directory.
         * Each entry is represented as a Path object and may refer to
         * either a file or a subdirectory.
         *
         * \return An array of paths contained in the directory
         */
	    virtual Array<Path> GetContent() const = 0;

	    /**
         * \brief Get the directory path
         *
         * Returns the full path associated with this directory.
         *
         * \return Reference to the directory path
         */
	    virtual const Path& GetPath() const = 0;

	    /**
         * \brief Get the underlying system handle
         *
         * Provides access to the native system handle associated with
         * the directory (e.g., OS-specific directory handle).
         *
         * \return Pointer to the system-specific handle, or nullptr if not applicable
         *
         * \warning The returned pointer is implementation-defined and must not be
         *          used directly unless the caller knows the underlying platform.
         */
		virtual void* GetSystemHandle() const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_DIRECTORY_HPP
