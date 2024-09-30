#include <Cocktail/Core/Utility/Time/Instant.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck
{
	Instant Instant::Now()
	{
		LARGE_INTEGER counter, frequency;
		QueryPerformanceCounter(&counter);
		QueryPerformanceFrequency(&frequency);

		const Uint64 seconds = counter.QuadPart / frequency.QuadPart;
		const Uint64 nanoseconds = counter.QuadPart - seconds * frequency.QuadPart;
		return EpochSeconds(seconds, nanoseconds);
	}
}