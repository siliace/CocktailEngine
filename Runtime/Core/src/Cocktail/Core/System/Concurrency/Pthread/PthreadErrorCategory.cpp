#include <cstring>

#include <Cocktail/Core/System/Concurrency/Pthread/PthreadErrorCategory.hpp>

namespace Ck::Detail::Pthread
{
    PthreadErrorCategory PthreadErrorCategory::Instance;

    const char* PthreadErrorCategory::name() const noexcept
    {
        return "pthread";
    }

    std::string PthreadErrorCategory::message(int error) const
    {
        char buf[1024];
        if (strerror_r(error, buf, sizeof(buf)) == nullptr)
            return buf;

        return "Unknown pthread error";
    }
}
