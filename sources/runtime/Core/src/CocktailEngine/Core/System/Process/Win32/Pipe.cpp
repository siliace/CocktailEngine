#include <CocktailEngine/Core/System/Process/Win32/Pipe.hpp>
#include <CocktailEngine/Core/System/Process/Win32/PipeReader.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
    Pipe::Pipe()
    {
        SECURITY_ATTRIBUTES securityAttributes;
        Memory::Zero(securityAttributes);
        securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
        securityAttributes.bInheritHandle = TRUE;

        HANDLE readHandle = INVALID_HANDLE_VALUE;
        HANDLE writeHandle = INVALID_HANDLE_VALUE;
        BOOL result = CreatePipe(&readHandle, &writeHandle, &securityAttributes, 0);
        if (result == FALSE)
            throw SystemError::GetLastError();

        if (!SetHandleInformation(readHandle, HANDLE_FLAG_INHERIT, 0))
        {
            DWORD lastError = GetLastError();
            CloseHandle(readHandle);
            CloseHandle(writeHandle);
            throw SystemException(lastError, SystemError::GetSystemErrorCategory());
        }

        mReader = MakeUnique<PipeReader>(readHandle);
        mWriter = MakeUnique<PipeWriter>(writeHandle);
    }

    PipeReader& Pipe::GetReader() const
    {
        return *mReader;
    }

    PipeWriter& Pipe::GetWriter() const
    {
        return *mWriter;
    }
}
