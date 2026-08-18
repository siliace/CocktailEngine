#ifndef COCKTAILENGINE_MAIN_CRASHREPORT_HPP
#define COCKTAILENGINE_MAIN_CRASHREPORT_HPP

#include <CocktailEngine/Core/Cocktail.hpp>

namespace Ck::Main
{
    /**
     * \brief A report being written while the process is dying
     *
     * Every write goes straight to a file descriptor: no allocation, no lock, no
     * formatting library, not even a call to strlen. That is not a matter of taste. On
     * Unix this is written from a signal handler, where the only functions that may be
     * called are the async-signal-safe ones, and the log manager of the engine allocates
     * from an object pool, takes a mutex and resolves a facade that asserts on a
     * terminated application. A report written through it would deadlock or fault about
     * as often as it worked, which is worse than no report at all: it would be blamed on
     * the crash it was supposed to describe.
     *
     * Windows is far less restrictive, since an exception filter runs on a thread that is
     * merely executing code. The stricter of the two rules is the one followed, so that
     * what a report looks like does not depend on the platform reading it.
     *
     * Two descriptors, both optional, both receiving everything: the standard error a
     * developer is already watching, and a file that outlives the terminal.
     */
    class CrashReport
    {
    public:

        /**
         * \brief Value of a descriptor that is not open
         */
        static constexpr int NoDescriptor = -1;

        /**
         * \brief Constructor
         *
         * \param consoleDescriptor Console descriptor to write to, or NoDescriptor
         * \param fileDescriptor File descriptor to write to, or NoDescriptor
         */
        CrashReport(int consoleDescriptor, int fileDescriptor);

        /**
         * \brief Write a NUL terminated string
         *
         * \param text The text to write, ignored when null
         */
        void Write(const AnsiChar* text) const;

        /**
         * \brief Write a NUL terminated string followed by a line break
         *
         * \param text The text to write, ignored when null
         */
        void WriteLine(const AnsiChar* text) const;

        /**
         * \brief Write an unsigned integer in base ten
         *
         * \param value The value to write
         */
        void WriteUnsigned(Uint64 value) const;

        /**
         * \brief Write a signed integer in base ten
         *
         * Worth having on its own: the codes the platforms report a fault with are signed,
         * and several of the interesting ones are negative.
         *
         * \param value The value to write
         */
        void WriteSigned(Int64 value) const;

        /**
         * \brief Write an unsigned integer in base sixteen, prefixed with 0x
         *
         * \param value The value to write
         */
        void WriteHex(Uint64 value) const;

        /**
         * \brief Write a pointer in base sixteen
         *
         * \param pointer The pointer to write
         */
        void WritePointer(const void* pointer) const;

        /**
         * \brief Write a line break
         */
        void EndLine() const;

        /**
         * \brief Get the console descriptor of the report
         *
         * Handed to whatever writes into the report on its own, a stack walker being what
         * needs it.
         *
         * \return The descriptor, or NoDescriptor
         */
        int GetConsoleDescriptor() const;

        /**
         * \brief Get the file descriptor of the report
         *
         * \return The descriptor, or NoDescriptor
         */
        int GetFileDescriptor() const;

    private:

        void WriteRaw(const AnsiChar* data, Uint64 length) const;

        int mConsole;
        int mFile;
    };
}

#endif // COCKTAILENGINE_MAIN_CRASHREPORT_HPP
