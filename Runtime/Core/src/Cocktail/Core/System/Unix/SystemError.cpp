#include <cstring>

#include <Cocktail/Core/System/SystemError.hpp>

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
            char buffer[1024];
            char* errorMessage = strerror_r(errorCode, buffer, 1024);
            return std::string(errorMessage, std::strlen(errorMessage));
        }
    };

    std::system_error SystemError::GetLastError()
    {
		return std::system_error(errno, GetSystemErrorCategory());
    }

    const std::error_category& SystemError::GetSystemErrorCategory()
    {
        static UnixErrorCategory errorCategory;
		return errorCategory;
    }
}