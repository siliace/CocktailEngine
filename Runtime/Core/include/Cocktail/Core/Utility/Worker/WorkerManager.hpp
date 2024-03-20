#ifndef COCKTAIL_CORE_UTILITY_WORKER_WORKERMANAGER_HPP
#define COCKTAIL_CORE_UTILITY_WORKER_WORKERMANAGER_HPP

#include <string>
#include <vector>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Core/Utility/Worker/WorkerGroup.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class WorkerManager : public Extends<WorkerManager, Object>
	{
	public:

		/**
		 * \brief 
		 * \param name 
		 * \param job 
		 * \return 
		 */
		Worker* CreateWorker(const std::string& name, Ref<WorkerJob> job);

		/**
		 * \brief 
		 * \param name 
		 * \param job 
		 * \param workerCount 
		 * \return 
		 */
		WorkerGroup* CreateWorkerGroup(const std::string& name, Ref<TaskedJob> job, unsigned int workerCount);

	private:

		std::vector<std::unique_ptr<Worker>> mWorkers;
		std::vector<std::unique_ptr<WorkerGroup>> mWorkerGroups;
	};
}

#endif // COCKTAIL_CORE_UTILITY_WORKER_WORKERMANAGER_HPP
