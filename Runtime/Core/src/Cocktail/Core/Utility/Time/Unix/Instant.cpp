#include <ctime>

#include <Cocktail/Core/Utility/Time/Instant.hpp>

namespace Ck
{
	Instant Instant::Now()
	{
		timespec timespec;
		clock_gettime(CLOCK_REALTIME, &timespec);

		return Instant::EpochSeconds(timespec.tv_sec, timespec.tv_nsec);
	}
}