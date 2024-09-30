#ifndef COCKTAIL_CORE_UTILITY_WORKER_WORKERS_HPP
#define COCKTAIL_CORE_UTILITY_WORKER_WORKERS_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/Utility/Worker/WorkerManager.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Workers : public ServiceFacade<WorkerManager>
	{
	public:

		/**
		 * \brief
		 * \param name
		 * \param job
		 * \return
		 */
		static Worker* CreateWorker(const std::string& name, Ref<WorkerJob> job)
		{
			return ResolveFacadeInstance()->CreateWorker(name, std::move(job));
		}

		/**
		 * \brief
		 * \param name
		 * \param job
		 * \param workerCount
		 * \return
		 */
		static WorkerGroup* CreateWorkerGroup(const std::string& name, Ref<TaskedJob> job, unsigned int workerCount)
		{
			return ResolveFacadeInstance()->CreateWorkerGroup(name, std::move(job), workerCount);
		}
	};
}

#endif // COCKTAIL_CORE_UTILITY_WORKER_WORKERS_HPP
