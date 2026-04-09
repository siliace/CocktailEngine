#ifndef COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILECURSORMODE_HPP
#define COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILECURSORMODE_HPP

namespace Ck
{
    /**
     * \brief Defines reference positions for file cursor operations
     *
     * FileCursorMode specifies the base position used when moving
     * the file cursor via methods such as File::SetCursor().
     */
    enum class FileCursorMode
    {
        /**
         * \brief Beginning of the file
         *
         * The cursor position is set relative to the start of the file.
         */
        Begin,

        /**
         * \brief Current cursor position
         *
         * The cursor position is set relative to its current location.
         */
        Current,

        /**
         * \brief End of the file
         *
         * The cursor position is set relative to the end of the file.
         */
        End,
    };
}

#endif // COCKTAIL_CORE_SYSTEM_FILESYSTEM_FILECURSORMODE_HPP
