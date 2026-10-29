#include <CocktailEngine/Core/System/Process/Win32/PipeReader.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>

namespace Ck::Detail::Win32
{
    PipeReader::PipeReader(HANDLE handle) :
        mHandle(handle),
        mEof(false)
    {
        assert(handle != INVALID_HANDLE_VALUE);
    }

    PipeReader::~PipeReader()
    {
        CloseHandle(mHandle);
    }

    Reader<>::SizeType PipeReader::Read(CharType* text, SizeType length)
    {
        DWORD read = 0;
        BOOL result = ReadFile(mHandle, text, sizeof(TextChar) * length, &read, nullptr);
        if (result == FALSE)
        {
            DWORD lastError = ::GetLastError();
            if (lastError != ERROR_BROKEN_PIPE)
                throw SystemException(lastError, SystemError::GetSystemErrorCategory());

            mEof = true;
            return 0;
        }

        return read / sizeof(TextChar);
    }

    bool PipeReader::HasCursor() const
    {
        return false;
    }

    Uint64 PipeReader::Seek(Uint64)
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    void PipeReader::Rewind()
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    Uint64 PipeReader::Tell() const
    {
        ExceptionUtils::ThrowNotImplemented();
    }

    Reader<>::SizeType PipeReader::GetSize() const
    {
        LARGE_INTEGER size;
        if (GetFileSizeEx(mHandle, &size) == FALSE)
            throw SystemError::GetLastError();

        return size.QuadPart;
    }

    bool PipeReader::IsEof() const
    {
        return mEof;
    }

    HANDLE PipeReader::GetSystemHandle() const
    {
        return mHandle;
    }
}
