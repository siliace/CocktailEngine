#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLEWRITER_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLEWRITER_HPP

#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \brief Writer implementation targeting a system console file descriptor
     *
     * This class provides a concrete implementation of Writer that writes
     * directly to a console output using a low-level handle (e.g. stdout or stderr).
     */
    class ConsoleWriter : public Writer<>
    {
    public:

        /**
         * \brief Creates a writer bound to the standard output handle
         *
         * \return A ConsoleWriter instance associated with stdout
         */
        static ConsoleWriter FromOutputHandle();

        /**
         * \brief Creates a writer bound to the standard error handle
         *
         * \return A ConsoleWriter instance associated with stderr
         */
        static ConsoleWriter FromErrorHandle();

        /**
         * \brief Constructs a writer from a native console handle
         *
         * \param handle The underlying file descriptor used for writing
         */
        explicit ConsoleWriter(HANDLE handle);

        /**
         * \brief Writes a sequence of characters to the console
         *
         * \param text Pointer to the character buffer to write
         * \param length Number of characters to write
         */
        void Write(const CharType* text, SizeType length) override;

        /**
         * \brief Flushes any buffered output to the console
         *
         * Ensures that all pending data is effectively written.
         */
        void Flush() override;

    private:

        HANDLE mHandle; /*!< Underlying console handle */
    };
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_WIN32_CONSOLEWRITER_HPP
