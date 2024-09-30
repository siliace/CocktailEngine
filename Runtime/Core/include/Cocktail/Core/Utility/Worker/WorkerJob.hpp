#ifndef COCKTAIL_CORE_UTILITY_WORKER_WORKERJOB_HPP
#define COCKTAIL_CORE_UTILITY_WORKER_WORKERJOB_HPP

#include <exception>

#include <Cocktail/Core/Meta/Implements.hpp>
#include <Cocktail/Core/Meta/Interface.hpp>

namespace Ck
{
	class Worker;

	/**
	 * \brief 
	 */
	class WorkerJob : public Implements<WorkerJob, Interface>
	{
	public:

		/**
		 * \brief 
		 * \param worker 
		 */
		virtual void Start(Worker* worker) = 0;

		/**
		 * \brief 
		 * \param worker 
		 */
		virtual void Cycle(Worker* worker) = 0;

		/**
		 * \brief 
		 * \param worker 
		 */
		virtual void Terminate(Worker* worker) = 0;

		/**
		 * \brief 
		 * \param worker 
		 * \param exception 
		 */
		virtual void Catch(Worker* worker, const std::exception& exception) const = 0;
	};
}

#endif // COCKTAIL_CORE_UTILITY_WORKER_WORKERJOB_HPP
