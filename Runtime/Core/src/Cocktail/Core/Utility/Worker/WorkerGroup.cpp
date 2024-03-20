#include <fmt/core.h>

#include <Cocktail/Core/Utility/Worker/Worker.hpp>
#include <Cocktail/Core/Utility/Worker/WorkerGroup.hpp>

namespace Ck
{
	WorkerGroup::WorkerGroup(std::string name, Ref<TaskedJob> job, unsigned int workerCount) :
		mName(std::move(name)),
		mJob(std::move(job))
	{
		mWorkers.reserve(workerCount);
		for (unsigned int i = 0; i < workerCount; i++)
		{
			std::unique_ptr<Worker> worker = std::make_unique<Worker>(this, mJob);
			if (!mName.empty())
				worker->SetName(fmt::format("{}-{}", mName, i));

			mWorkers.emplace_back(std::move(worker));
		}
	}

	WorkerGroup::~WorkerGroup()
	{
		while (!mTasks.IsEmpty())
			std::this_thread::yield();
	}

	std::unique_ptr<WorkerTask> WorkerGroup::PickTask()
	{
		return mTasks.Take();
	}
}
