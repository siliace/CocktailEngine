#include <algorithm>
#include <cstring>

#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Clipboard/Win32/ClipboardService.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
    std::size_t ClipboardService::Get(void* data, std::size_t length, std::size_t offset)
    {
        if (IsClipboardFormatAvailable(CF_TEXT) == FALSE)
            throw SystemError::GetLastError();

        if (OpenClipboard(nullptr) == FALSE)
            throw SystemError::GetLastError();
            
        HANDLE clipboardHandle = GetClipboardData(CF_TEXT);

		char* clipboardData = static_cast<char*>(GlobalLock(clipboardHandle));

		std::size_t read = std::min(std::strlen(clipboardData) - offset, length);
        std::memcpy(data, clipboardData + offset, read);

        GlobalUnlock(clipboardHandle);
        CloseClipboard();

        return read;    
    }

    std::size_t ClipboardService::GetSize() const
    {
        if (IsClipboardFormatAvailable(CF_TEXT) == FALSE)
            throw SystemError::GetLastError();

        if (OpenClipboard(nullptr) == FALSE)
            throw SystemError::GetLastError();

        HANDLE clipboardHandle = GetClipboardData(CF_TEXT);

        char* clipboardData = static_cast<char*>(GlobalLock(clipboardHandle));
        return std::strlen(clipboardData);
    }

    void ClipboardService::Set(const void* data, std::size_t length)
    {
        if (!OpenClipboard(nullptr))
            throw SystemError::GetLastError();

        HANDLE memoryHandle = GlobalAlloc(GMEM_MOVEABLE, length);

        if (memoryHandle == INVALID_HANDLE_VALUE)
			throw SystemError::GetLastError();

		memcpy(GlobalLock(memoryHandle), data, length);
		GlobalUnlock(memoryHandle);

		SetClipboardData(CF_TEXT, memoryHandle);
        CloseClipboard();
    }

    void ClipboardService::Clear()
    {
		if (!OpenClipboard(nullptr))
			throw SystemError::GetLastError();

		if (!EmptyClipboard())
			throw SystemError::GetLastError();
    }
}
