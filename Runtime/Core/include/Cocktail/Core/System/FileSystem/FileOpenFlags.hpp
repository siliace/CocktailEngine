#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILEOPENFLAGS_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILEOPENFLAGS_HPP

#include <Cocktail/Core/Flags.hpp>

namespace Ck
{
    /**
     * \brief Bitmask flags used to control file opening behavior
     *
     * FileOpenFlagBits defines low-level options that control how a file
     * is opened, including access mode, buffering, truncation behavior,
     * and sharing permissions.
     *
     * These flags can be combined using FileOpenFlags.
     */
    enum class FileOpenFlagBits
    {
        /**
         * \brief Enables read access to the file
         */
        Read = Bit(0),

        /**
         * \brief Enables write access to the file
         */
        Write = Bit(1),

        /**
         * \brief Appends all writes to the end of the file
         *
         * The file cursor is positioned at the end upon opening.
         */
        Append = Bit(2),

        /**
         * \brief Truncates the file on open
         *
         * The file content is cleared when the file is opened.
         */
        Truncate = Bit(3),

        /**
         * \brief Requires that the file already exists
         *
         * Opening will fail if the file does not exist, even if Write is enabled.
         */
        Existing = Bit(4),

        /**
         * \brief Disables system-level buffering for I/O operations
         */
        Unbuffered = Bit(5),

        /**
         * \brief Prevents other processes from writing to the file
         *
         * Depending on the platform, this may restrict concurrent write access
         * while the file is open.
         */
        Locked = Bit(6),
    };

    /**
     * \brief Combination of FileOpenFlagBits
     *
     * FileOpenFlags represents a bitmask of FileOpenFlagBits used to
     * configure file opening behavior.
     */
    using FileOpenFlags = Flags<FileOpenFlagBits>;
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILEOPENFLAGS_HPP
