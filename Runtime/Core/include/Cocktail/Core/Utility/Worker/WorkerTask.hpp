#ifndef COCKTAIL_CORE_UTILITY_WORKER_WORKERTASK_HPP
#define COCKTAIL_CORE_UTILITY_WORKER_WORKERTASK_HPP

#include <Cocktail/Core/Meta/Implements.hpp>
#include <Cocktail/Core/Meta/Interface.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class WorkerTask : public Implements<WorkerTask, Interface>
	{
	public:

		/**
		 * \brief 
		 */
		virtual void Process() = 0;
	};
}

#endif // COCKTAIL_CORE_UTILITY_WORKER_WORKERTASK_HPP
