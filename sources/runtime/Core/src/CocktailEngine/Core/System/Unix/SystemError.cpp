#include <cstring>

#include <CocktailEngine/Core/System/SystemError.hpp>

namespace Ck
{
    class UnixErrorCategory : public std::error_category
    {
    public:

        const char* name() const noexcept override
        {
            return "UnixErrorCategory";
        }

        std::string message(int errorCode) const override
        {
            if (char* error = strerror(errorCode); error != nullptr)
                return error;

            return "Unknown";
        }
    };

    unsigned int SystemError::GetLastErrorCode()
    {
        return errno;
    }

    std::system_error SystemError::GetLastError()
    {
        return std::system_error(static_cast<int>(GetLastErrorCode()), GetSystemErrorCategory());
    }

    const std::error_category& SystemError::GetSystemErrorCategory()
    {
        static UnixErrorCategory errorCategory;
        return errorCategory;
    }
}
