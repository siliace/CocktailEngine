#ifndef COCKTAIL_CORE_UTILIY_WORKER_WORKERGROUP_HPP
#define COCKTAIL_CORE_UTILIY_WORKER_WORKERGROUP_HPP

#include <string>

#include <Cocktail/Core/System/Concurrency/BlockingQueue.hpp>
#include <Cocktail/Core/Utility/Worker/CallableTask.hpp>
#include <Cocktail/Core/Utility/Worker/TaskedJob.hpp>
#include <Cocktail/Core/Utility/Worker/Worker.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class WorkerGroup
	{
	public:

		/**
		 * \brief 
		 * \param name 
		 * \param job 
		 * \param workerCount 
		 */
		WorkerGroup(std::string name, Ref<TaskedJob> job, unsigned int workerCount);

		/**
		 * \brief 
		 */
		~WorkerGroup();

		/**
		 * \brief 
		 * \tparam Callable 
		 * \tparam Args 
		 * \param callable 
		 * \param args 
		 * \return 
		 */
		template <typename Callable, typename... Args>
		std::future<std::invoke_result_t<Callable, Args...>> Dispatch(Callable&& callable, Args&&... args)
		{
			auto task = std::make_unique<CallableTask<Callable, Args...>>(std::forward<Callable>(callable), std::forward<Args>(args)...);
			auto future = task->ToFuture();

			mTasks.Push(std::move(task));

			return future;
		}

	private:

		friend class TaskedJob;

		/**
		 * \brief 
		 * \return 
		 */
		std::unique_ptr<WorkerTask> PickTask();

		std::string mName;
		Ref<TaskedJob> mJob;
		std::vector<std::unique_ptr<Worker>> mWorkers;
		BlockingQueue<std::unique_ptr<WorkerTask>> mTasks;
	};
}

#endif // COCKTAIL_CORE_UTILIY_WORKER_WORKERGROUP_HPP
