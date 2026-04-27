#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLESERVICE_HPP

#include <Cocktail/Core/IO/Input/Reader/LineReader.hpp>
#include <Cocktail/Core/IO/Output/Writer/LineWriter.hpp>
#include <Cocktail/Core/System/Console/ConsoleColor.hpp>
#include <Cocktail/Core/System/Console/ConsoleStyle.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

namespace Ck
{
    /**
     * \brief Interface providing access to system console input/output services
     *
     * This interface abstracts standard console operations such as writing,
     * reading, display management, and simple interactions like emitting a sound.
     */
    class ConsoleService
    {
    public:

        /**
         * \brief Default virtual destructor
         */
        virtual ~ConsoleService() = default;

        /**
         * \brief Retrieves the standard output stream
         *
         * Allows writing text to the console (equivalent to stdout).
         *
         * \return A LineWriter instance bound to the standard output
         */
        virtual LineWriter<> GetOutput() = 0;

        /**
         * \brief Retrieves the standard error stream
         *
         * Allows writing text to the error output (equivalent to stderr).
         *
         * \return A LineWriter instance bound to the error output
         */
        virtual LineWriter<> GetError() = 0;

        /**
         * \brief Retrieves the standard input stream
         *
         * Allows reading input from the console (equivalent to stdin).
         *
         * \return A reference to a LineReader instance bound to the standard input
         */
        virtual LineReader<> GetInput() = 0;

        /**
         * \brief Clears the console display
         *
         * Erases all visible content from the console screen.
         */
        virtual void Clear() = 0;

        /**
         * \brief Sets the text rendering attributes in the console
         *
         * Depending on the underlying implementation, some parameters
         * may be ignored if not supported by the system.
         *
         * \param text The text (foreground) color
         * \param background The background color
         * \param style The text style (default is ConsoleStyle::Normal)
         */
        virtual void SetColors(ConsoleColor text, ConsoleColor background, ConsoleStyle style) = 0;

        /**
         * \brief Emits a default system beep sound
         */
        virtual void Beep() const = 0;

        /**
         * \brief Emits a beep sound with a specific frequency and duration
         *
         * \param frequency The frequency of the beep in Hertz
         * \param duration The duration of the beep
         */
        virtual void Beep(unsigned int frequency, const Duration& duration) const = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLESERVICE_HPP
