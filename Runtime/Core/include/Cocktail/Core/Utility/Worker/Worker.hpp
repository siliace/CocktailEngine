#ifndef COCKTAIL_CORE_UTILITY_WORKER_WORKER_HPP
#define COCKTAIL_CORE_UTILITY_WORKER_WORKER_HPP

#include <thread>

#include <Cocktail/Core/Memory/Ref.hpp>
#include <Cocktail/Core/Utility/Worker/WorkerJob.hpp>

namespace Ck
{
	class WorkerGroup;

	/**
	 * \brief 
	 */
	class Worker
	{
	public:

		/**
		 * \brief 
		 * \param group 
		 * \param job 
		 */
		Worker(WorkerGroup* group, Ref<WorkerJob> job);

		/**
		 * \brief 
		 */
		~Worker();

		/**
		 * \brief 
		 */
		void Terminate();

		/**
		 * \brief 
		 * \return 
		 */
		WorkerGroup* GetGroup() const;

		/**
		 * \brief 
		 * \param affinityMask 
		 */
		void SetAffinityMask(unsigned int affinityMask);

		/**
		 * \brief 
		 * \return 
		 */
		const std::string& GetName() const;

		/**
		 * \brief 
		 * \param name 
		 */
		void SetName(std::string name);

	private:

		/**
		 * \brief 
		 */
		void Loop();

		WorkerGroup* mGroup;
		Ref<WorkerJob> mJob;
		std::string mName;
		std::thread mThread;
		std::atomic<bool> mRunning;
	};
}
#endif // COCKTAIL_CORE_UTILITY_WORKER_WORKER_HPP
