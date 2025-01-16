#include <Cocktail/Core/Memory/RefCounter.hpp>

namespace Ck
{
	RefCounter::RefCounter() :
		RefCounter(1, 1)
	{
		/// Nothing
	}

	RefCounter::RefCounter(Uint64 strongCounter, Uint64 weakCounter) :
		mStrongCounter(strongCounter),
		mWeakCounter(weakCounter)
	{
		/// Nothing
	}

	void RefCounter::Acquire()
	{
		++mStrongCounter;
	}

	void RefCounter::WeakAcquire()
	{
		++mWeakCounter;
	}

	void RefCounter::Release()
	{
		--mStrongCounter;
		if (!mStrongCounter)
		{
			Destroy();
			WeakRelease();
		}
	}

	void RefCounter::WeakRelease()
	{
		--mWeakCounter;
		if (!mWeakCounter)
			Suicide();
	}

	Uint64 RefCounter::GetCounter() const
	{
		return mStrongCounter;
	}

	Uint64 RefCounter::GetWeakCounter() const
	{
		return mWeakCounter;
	}
}
