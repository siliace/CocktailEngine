#include <Cocktail/Core/Utility/Worker/WorkerManager.hpp>

namespace Ck
{
	Worker* WorkerManager::CreateWorker(const std::string& name, Ref<WorkerJob> job)
	{
		auto worker = std::make_unique<Worker>(nullptr, std::move(job));
		if (!name.empty())
			worker->SetName(name);

		return mWorkers.emplace_back(std::move(worker)).get();
	}

	WorkerGroup* WorkerManager::CreateWorkerGroup(const std::string& name, Ref<TaskedJob> job, unsigned int workerCount)
	{
		return mWorkerGroups.emplace_back(
			std::make_unique<WorkerGroup>(name, std::move(job), workerCount)
		).get();
	}
}
