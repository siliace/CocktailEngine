#include <cerrno>
#include <cstring>

#include <CocktailEngine/Core/System/SystemError.hpp>

namespace Ck
{
    const SystemErrorCodeTranslator SystemErrorCodeTranslator::Instance;

    String SystemErrorCodeTranslator::TranslateErrorCode(unsigned int errorCode) const
    {
        char buffer[1024];

#if defined(__GLIBC__) && defined(_GNU_SOURCE)
        const char* message = strerror_r(static_cast<int>(errorCode), buffer, sizeof(buffer));
        if (message != nullptr)
            return String(reinterpret_cast<const Utf8Char*>(message));
#else
        if (strerror_r(static_cast<int>(errorCode), buffer, sizeof(buffer)) == 0)
            return String(reinterpret_cast<const Utf8Char*>(buffer));
#endif

        return String::Format(CK_TEXT("Unknown system error (%u)"), errorCode);
    }

    unsigned int SystemError::GetLastErrorCode()
    {
        return static_cast<unsigned int>(errno); /// Errno always hold a positive value or 0
    }
}
