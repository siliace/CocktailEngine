#include <CocktailEngine/Core/System/Win32/Filetime.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>
#include <CocktailEngine/Core/Utility/Time/Instant.hpp>

namespace Ck
{
    Instant Instant::Now()
    {
        /// The system clock rather than a performance counter: an Instant counts from
        /// the epoch, which a counter started at boot cannot express. GetSystemTime
        /// PreciseAsFileTime is the precise variant, the plain one being quantised to
        /// the scheduler tick, around 15 ms, which is coarser than the millisecond
        /// Clock and Chronometer report in. It needs Windows 8.
        FILETIME fileTime;
        GetSystemTimePreciseAsFileTime(&fileTime);

        return Detail::Win32::FiletimeToInstant(fileTime);
    }
}
