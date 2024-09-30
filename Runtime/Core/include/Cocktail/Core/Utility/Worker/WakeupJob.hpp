#ifndef COCKTAIL_CORE_UTILITY_WORKER_WAKEUPJOB_HPP
#define COCKTAIL_CORE_UTILITY_WORKER_WAKEUPJOB_HPP

#include <mutex>
#include <unordered_map>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>
#include <Cocktail/Core/Utility/Time/Instant.hpp>
#include <Cocktail/Core/Utility/Worker/WorkerJob.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API WakeupJob : public Inherit<WakeupJob, Object, WorkerJob>
	{
	public:

		/**
		 * \brief 
		 * \param period 
		 */
		explicit WakeupJob(const Duration& period);

		/**
		 * \brief 
		 * \param worker 
		 */
		void Start(Worker* worker) override;

		/**
		 * \brief 
		 * \param worker 
		 */
		void Cycle(Worker* worker) override;

		/**
		 * \brief 
		 * \param worker 
		 */
		void Terminate(Worker* worker) override;

		/**
		 * \brief 
		 * \param worker 
		 * \param exception 
		 */
		void Catch(Worker* worker, const std::exception& exception) const override;

	protected:

		/**
		 * \brief 
		 * \param worker 
		 */
		virtual void WakeUp(Worker* worker) = 0;

	private:

		Duration mPeriod;
		std::mutex mInstantsLock;
		std::unordered_map<Worker*, Instant> mStart;
	};
}

#endif // COCKTAIL_CORE_UTILITY_WORKER_WAKEUPJOB_HPP
