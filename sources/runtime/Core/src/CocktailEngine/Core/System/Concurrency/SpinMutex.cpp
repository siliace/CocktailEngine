#include <CocktailEngine/Core/System/Concurrency/SpinMutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>

namespace Ck
{
	SpinMutex::SpinMutex() noexcept:
		mState(true)
	{
		/// Nothing	
	}

	SpinMutex::~SpinMutex() noexcept = default;

	void SpinMutex::Lock()
	{
		while (true)
		{
			while (!mState.load(std::memory_order_relaxed))
				Thread::Yield();

			if (TryLock())
				break;
		}
	}

	bool SpinMutex::TryLock() noexcept
	{
		return mState.exchange(false, std::memory_order_acquire);
	}

	void SpinMutex::Unlock() noexcept
	{
		mState.store(true, std::memory_order_release);
	}
}
