#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLE_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLE_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/IO/Output/Writer/LineWriter.hpp>
#include <Cocktail/Core/System/Console/ConsoleService.hpp>

namespace Ck
{
    /**
     * \brief Static facade providing access to console services
     *
     * This class acts as a convenience wrapper over ConsoleService,
     * exposing a simplified static interface for common console
     * operations such as input, output, display control, and sound.
     *
     * The underlying implementation is provided by the active
     * ConsoleService instance.
     */
    class COCKTAIL_CORE_API Console : public ServiceFacade<ConsoleService>
    {
    public:

        /**
         * \brief Retrieves the standard output writer
         *
         * Shortcut for accessing the output stream of the underlying service.
         *
         * \return A LineWriter instance bound to the standard output
         */
        static LineWriter<> GetOutput();

        /**
         * \brief Retrieves the standard error writer
         *
         * Shortcut for accessing the error output stream of the underlying service.
         *
         * \return A LineWriter instance bound to the standard error
         */
        static LineWriter<> GetError();

        /**
         * \brief Retrieves the standard input reader
         *
         * Shortcut for accessing the input stream of the underlying service.
         *
         * \return A LineReader instance bound to the standard input
         */
        static LineReader<> GetInput();

        /**
         * \brief Clears the console display
         *
         * Delegates to the underlying ConsoleService implementation.
         */
        static void Clear();

        /**
         * \brief Sets the text rendering attributes in the console
         *
         * Depending on the underlying implementation, some parameters
         * may be ignored if not supported by the system.
         *
         * \param text The text (foreground) color
         * \param background The background color
         * \param style The text style
         */
        static void SetColors(ConsoleColor text, ConsoleColor background, ConsoleStyle style);

        /**
         * \brief Emits a default system beep sound
         *
         * Delegates to the underlying ConsoleService implementation.
         */
        static void Beep();

        /**
         * \brief Emits a beep sound with a specific frequency and duration
         *
         * \param frequency The frequency of the beep in Hertz
         * \param duration The duration of the beep
         */
        static void Beep(unsigned int frequency, const Duration& duration);
    };
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_CONSOLE_HPP
