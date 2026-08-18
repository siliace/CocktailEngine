#ifndef COCKTAILENGINE_CORE_SYSTEM_WIN32_FILETIME_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WIN32_FILETIME_HPP

#include <CocktailEngine/Core/System/Win32/Windows.hpp>
#include <CocktailEngine/Core/Utility/Time/Instant.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \brief Number of 100ns periods between 1st january 1601 and 1st january 1970
     *
     * A FILETIME counts from 1601, an Instant counts from 1970, and this is the
     * distance between the two origins.
     */
    static constexpr Uint64 FiletimeDurationToEpoch = 116'444'736'000'000'000ULL;

    /**
     * \brief Number of FILETIME ticks in a second, a tick being 100ns
     */
    static constexpr Uint64 FiletimeTicksPerSecond = 10'000'000ULL;

    /**
     * \brief Number of nanoseconds in a FILETIME tick
     */
    static constexpr Uint64 FiletimeTickNanoseconds = 100ULL;

    /**
     * \brief Converts a Win32 FILETIME to an Instant
     *
     * Both describe an absolute point in time, they only disagree on the origin they
     * count from and on their resolution. This is the single place where that is
     * reconciled, so that every Instant the engine produces on Windows counts from
     * the same origin as on the other platforms.
     *
     * \param fileTime UTC file time to convert. A FILETIME holding local time has to
     *        be converted with FileTimeToLocalFileTime beforehand
     *
     * \return The matching Instant, truncated to the 100ns resolution of a FILETIME
     *
     * \remark A FILETIME earlier than the epoch cannot be represented and underflows.
     *         Nothing in the engine produces one: file times come from the file system
     *         and the system clock, both of which sit well past 1970
     */
    inline Instant FiletimeToInstant(const FILETIME& fileTime)
    {
        ULARGE_INTEGER ticks;
        ticks.HighPart = fileTime.dwHighDateTime;
        ticks.LowPart = fileTime.dwLowDateTime;

        /// Number of 100ns periods since 1st january 1970
        const Uint64 epochTicks = ticks.QuadPart - FiletimeDurationToEpoch;

        return Instant::EpochSeconds(epochTicks / FiletimeTicksPerSecond, epochTicks % FiletimeTicksPerSecond * FiletimeTickNanoseconds);
    }
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WIN32_FILETIME_HPP
