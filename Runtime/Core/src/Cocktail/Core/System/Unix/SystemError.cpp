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
            char buf[1024];
            if (strerror_r(errorCode, buf, sizeof(buf)) == nullptr) {
                return buf;
            }
            return "Unknown";
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