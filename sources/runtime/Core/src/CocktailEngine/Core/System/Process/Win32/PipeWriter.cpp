#include <CocktailEngine/Core/System/Process/Win32/PipeWriter.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>

namespace Ck::Detail::Win32
{
    PipeWriter::PipeWriter(HANDLE handle) :
        mHandle(handle)
    {
        assert(handle != INVALID_HANDLE_VALUE);
    }

    PipeWriter::~PipeWriter()
    {
        CloseHandle(mHandle);
    }

    void PipeWriter::Write(const CharType* text, SizeType length)
    {
        DWORD written;
        BOOL result = WriteFile(mHandle, text, length * sizeof(TextChar), &written, nullptr);
        if (result == FALSE)
            throw SystemError::GetLastError();
    }

    void PipeWriter::Flush()
    {
        FlushFileBuffers(mHandle);
    }

    HANDLE PipeWriter::GetSystemHandle() const
    {
        return mHandle;
    }
}
