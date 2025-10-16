#include <Cocktail/Core/System/Concurrency/Thread.hpp>

namespace Ck
{
	void Thread::SleepUntil(const Instant& instant, const Duration& duration)
	{
		// Assert the instant is not before now, otherwise we would wait forever
		assert(Instant::Now().IsBefore(instant));

		WaitUntil([&]() {
			return Instant::Now().IsBefore(instant);
		}, duration);
	}

	Thread* Thread::GetCurrentThread()
	{
		return ResolveFacadeInstance()->GetCurrentThread();
	}
}
