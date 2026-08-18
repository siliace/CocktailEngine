#include <cerrno>

#include <CocktailEngine/Main/CrashReport.hpp>

#include "CocktailEngine/Core/Utility/StringUtils.hpp"

#ifdef COCKTAIL_OS_WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif

namespace Ck::Main
{
    namespace
    {
        /**
         * \brief Length of a NUL terminated string
         *
         * Written out rather than taken from the C library: strlen is not on the list of
         * functions a signal handler may call. It almost certainly would be safe, being
         * pure computation, but "almost certainly" is not what a crash handler is for.
         */
        Uint64 Length(const AnsiChar* text)
        {
            Uint64 length = 0;
            while (text[length])
                length++;

            return length;
        }

        /**
         * \brief Write a whole buffer to one descriptor
         *
         * A write can be partial and can be interrupted, and neither is an error worth
         * giving up on. Anything else ends the report on that descriptor: there is nowhere
         * left to report the failure to report.
         */
        void WriteTo(int descriptor, const AnsiChar* data, Uint64 length)
        {
            if (descriptor == CrashReport::NoDescriptor)
                return;

            Uint64 written = 0;
            while (written < length)
            {
#ifdef COCKTAIL_OS_WINDOWS
                const int result = _write(descriptor, data + written, static_cast<unsigned int>(length - written));
#else
                const ssize_t result = write(descriptor, data + written, static_cast<size_t>(length - written));

                if (result < 0 && errno == EINTR)
                    continue;
#endif

                if (result <= 0)
                    return;

                written += static_cast<Uint64>(result);
            }
        }
    }

    CrashReport::CrashReport(int consoleDescriptor, int fileDescriptor) :
        mConsole(consoleDescriptor),
        mFile(fileDescriptor)
    {
        /// Nothing
    }

    void CrashReport::Write(const AnsiChar* text) const
    {
        if (!text)
            return;

        WriteRaw(text, StringUtils<AnsiChar, unsigned int>::GetLength(text));
    }

    void CrashReport::WriteLine(const AnsiChar* text) const
    {
        Write(text);
        EndLine();
    }

    void CrashReport::WriteUnsigned(Uint64 value) const
    {
        // Twenty digits is what an unsigned 64 bit value takes at most, in base ten.
        AnsiChar buffer[20];
        Uint64 index = sizeof(buffer);

        do
        {
            buffer[--index] = static_cast<AnsiChar>('0' + (value % 10));
            value /= 10;
        } while (value && index);

        WriteRaw(buffer + index, sizeof(buffer) - index);
    }

    void CrashReport::WriteSigned(Int64 value) const
    {
        if (value < 0)
        {
            Write("-");

            // Negated as unsigned, so that the most negative value has somewhere to go.
            WriteUnsigned(~static_cast<Uint64>(value) + 1);
        }
        else
        {
            WriteUnsigned(static_cast<Uint64>(value));
        }
    }

    void CrashReport::WriteHex(Uint64 value) const
    {
        static const AnsiChar Digits[] = "0123456789abcdef";

        AnsiChar buffer[2 + 16];
        buffer[0] = '0';
        buffer[1] = 'x';

        // Fixed width on purpose: an address is easier to compare against a map file when
        // two of them line up.
        for (Uint64 nibble = 0; nibble < 16; nibble++)
            buffer[2 + nibble] = Digits[(value >> ((15 - nibble) * 4)) & 0xF];

        WriteRaw(buffer, sizeof(buffer));
    }

    void CrashReport::WritePointer(const void* pointer) const
    {
        WriteHex(reinterpret_cast<Uint64>(pointer));
    }

    void CrashReport::EndLine() const
    {
#ifdef COCKTAIL_OS_WINDOWS
        WriteRaw("\r\n", 2);
#else
        WriteRaw("\n", 1);
#endif
    }

    int CrashReport::GetConsoleDescriptor() const
    {
        return mConsole;
    }

    int CrashReport::GetFileDescriptor() const
    {
        return mFile;
    }

    void CrashReport::WriteRaw(const AnsiChar* data, Uint64 length) const
    {
        if (!length)
            return;

        WriteTo(mConsole, data, length);
        WriteTo(mFile, data, length);
    }
}
