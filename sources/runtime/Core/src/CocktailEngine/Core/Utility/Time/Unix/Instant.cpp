#include <ctime>

#include <CocktailEngine/Core/Utility/Time/Instant.hpp>

namespace Ck
{
    Instant Instant::Now()
    {
        timespec timespec;
        clock_gettime(CLOCK_REALTIME, &timespec);

        return Instant::EpochSeconds(static_cast<Uint64>(timespec.tv_sec), static_cast<Uint64>(timespec.tv_nsec));
    }
}
