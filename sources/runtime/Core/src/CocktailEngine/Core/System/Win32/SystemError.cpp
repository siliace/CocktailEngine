#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck
{
    const SystemErrorCodeTranslator SystemErrorCodeTranslator::Instance;

    String SystemErrorCodeTranslator::TranslateErrorCode(unsigned int errorCode) const
    {
        if (errorCode == 0)
            return String::Empty;

        LPWSTR messageBuffer = nullptr;
        const DWORD size = ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, errorCode,
                                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, nullptr);

        if (size == 0 || messageBuffer == nullptr)
            return String::Format(CK_TEXT("Unknown system error (%u)"), errorCode);

        String error(messageBuffer, size);
        ::LocalFree(messageBuffer);

        return error;
    }

    unsigned int SystemError::GetLastErrorCode()
    {
        return ::GetLastError();
    }
}
