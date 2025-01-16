#include <Cocktail/Core/Utility/Time/Instant.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck
{
	Instant Instant::Now()
	{
		LARGE_INTEGER counter, frequency;
		if (QueryPerformanceFrequency(&frequency) == FALSE)
			throw SystemError::GetLastError();

		QueryPerformanceCounter(&counter);

		double timestamp = static_cast<double>(counter.QuadPart) * 1e9 / static_cast<double>(frequency.QuadPart);

		const Uint64 seconds = static_cast<Uint64>(timestamp / 1e9);
		const Uint64 nanoseconds = static_cast<Uint64>(timestamp - static_cast<double>(seconds) * 1e9);
		return EpochSeconds(seconds, nanoseconds);
	}
}
