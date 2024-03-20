#include <Cocktail/Core/Utility/Worker/TaskedJob.hpp>
#include <Cocktail/Core/Utility/Worker/WorkerGroup.hpp>

namespace Ck
{
	void TaskedJob::Start(Worker* worker)
	{
	}

	void TaskedJob::Cycle(Worker* worker)
	{
		WorkerGroup* group = worker->GetGroup();
		std::unique_ptr<WorkerTask> task = group->PickTask();
		if (!task)
			return;

		task->Process();
	}

	void TaskedJob::Terminate(Worker* worker)
	{
	}

	void TaskedJob::Catch(Worker* worker, const std::exception& exception) const
	{
	}
}
