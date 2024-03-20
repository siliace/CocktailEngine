#ifndef COCKTAIL_CORE_UTILITY_WORKER_TASKEDJOB_HPP
#define COCKTAIL_CORE_UTILITY_WORKER_TASKEDJOB_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/Concurrency/BlockingQueue.hpp>
#include <Cocktail/Core/Utility/Worker/WorkerJob.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class TaskedJob : public Inherit<TaskedJob, Object, WorkerJob>
	{
	public:

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
	};
}
#endif // COCKTAIL_CORE_UTILITY_WORKER_TASKEDJOB_HPP
