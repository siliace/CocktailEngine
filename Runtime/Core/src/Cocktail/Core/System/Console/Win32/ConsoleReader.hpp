#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLEREADER_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLEREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \brief Reader implementation for console input
     *
     * This class provides a concrete implementation of Reader that reads
     * character data from the standard input stream (console).
     *
     * Depending on the underlying system, some operations such as seeking
     * may not be supported.
     */
    class ConsoleReader : public Reader<>
    {
    public:

        /**
         * \brief Reads a single character from the console
         *
         * \param c Reference where the read character will be stored
         * \return True if a character was successfully read, false otherwise
         */
        bool Read(TextChar& c) override;

        /**
         * \brief Reads a sequence of characters from the console
         *
         * \param buffer Pointer to the buffer that will receive the data
         * \param length Maximum number of characters to read
         * \return The number of characters actually read
         */
        SizeType Read(TextChar* buffer, SizeType length) override;

        /**
         * \brief Indicates whether the stream supports cursor positioning
         *
         * Console input streams typically do not support seeking.
         *
         * \return True if cursor positioning is supported, false otherwise
         */
        bool HasCursor() const override;

        /**
         * \brief Moves the read position within the stream
         *
         * For console input, this operation is usually unsupported.
         *
         * \param position Target position in the stream
         * \return The new position, or an undefined value if unsupported
         */
        Uint64 Seek(Uint64 position) override;

        /**
         * \brief Resets the read position to the beginning of the stream
         *
         * For console input, this operation is typically unsupported.
         */
        void Rewind() override;

        /**
         * \brief Returns the current read position
         *
         * \return Current position in the stream, if applicable
         */
        Uint64 Tell() const override;

        /**
         * \brief Returns the total size of the input stream
         *
         * For console input, the size is generally unknown.
         *
         * \return The size of the stream, or an undefined value if not applicable
         */
        SizeType GetSize() const override;

        /**
         * \brief Indicates whether the end of the input stream has been reached
         *
         * \return True if end-of-file is reached, false otherwise
         */
        bool IsEof() const override;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLEREADER_HPP
