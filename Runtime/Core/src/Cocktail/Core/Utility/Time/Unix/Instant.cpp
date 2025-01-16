#include <ctime>

#include <Cocktail/Core/Utility/Time/Instant.hpp>

namespace Ck
{
	Instant Instant::Now()
	{
		timespec timespec;
		clock_gettime(0, &timespec);

		return Instant::EpochSeconds(timespec.tv_sec, timespec.tv_nsec);
	}
}