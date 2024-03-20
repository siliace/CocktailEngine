#include <Cocktail/Core/Utility/Worker/WakeupJob.hpp>

namespace Ck
{
	WakeupJob::WakeupJob(const Duration& period):
		mPeriod(period)
	{
		/// Nothing
	}

	void WakeupJob::Start(Worker* worker)
	{
		std::lock_guard lg(mInstantsLock);
		mStart[worker] = Instant::Now();
	}

	void WakeupJob::Cycle(Worker* worker)
	{
		{
			std::lock_guard lg(mInstantsLock);
			auto now = Instant::Now();
			if (mPeriod < Duration::Between(now, mStart[worker]))
				return;

			mStart[worker] = now;
		}

		WakeUp(worker);
	}

	void WakeupJob::Terminate(Worker* worker)
	{
		std::lock_guard lg(mInstantsLock);
		mStart.erase(worker);
	}

	void WakeupJob::Catch(Worker* worker, const std::exception& exception) const
	{

	}
}
