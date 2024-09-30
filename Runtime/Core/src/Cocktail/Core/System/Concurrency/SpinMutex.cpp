#include <thread>

#include <Cocktail/Core/System/Concurrency/SpinMutex.hpp>

namespace Ck
{
	SpinMutex::SpinMutex() noexcept:
		mState(true)
	{
		/// Nothing	
	}

	SpinMutex::~SpinMutex() noexcept = default;

	void SpinMutex::lock() noexcept
	{
		while (true)
		{
			while (!mState.load(std::memory_order_relaxed))
				std::this_thread::yield();

			if (try_lock())
				break;
		}
	}

	bool SpinMutex::try_lock() noexcept
	{
		return mState.exchange(false, std::memory_order_acquire);
	}

	void SpinMutex::unlock() noexcept
	{
		mState.store(true, std::memory_order_release);
	}
}
