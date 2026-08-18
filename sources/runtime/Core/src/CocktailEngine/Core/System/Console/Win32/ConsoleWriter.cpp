#include <CocktailEngine/Core/IO/Output/Stream/OutputStream.hpp>
#include <CocktailEngine/Core/System/Console/Win32/ConsoleWriter.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>

namespace Ck::Detail::Win32
{
    ConsoleWriter ConsoleWriter::FromOutputHandle()
    {
        return ConsoleWriter(GetStdHandle(STD_OUTPUT_HANDLE));
    }

    ConsoleWriter ConsoleWriter::FromErrorHandle()
    {
        return ConsoleWriter(GetStdHandle(STD_ERROR_HANDLE));
    }

    ConsoleWriter::ConsoleWriter(HANDLE handle) :
        mHandle(handle)
    {
        /// Nothing
    }

    void ConsoleWriter::Write(const TextChar* string, SizeType length)
    {
        if (WriteConsole(mHandle, string, length, nullptr, nullptr) == FALSE)
            ;//throw SystemError::GetLastError();
    }

    void ConsoleWriter::Flush()
    {
    }
}
