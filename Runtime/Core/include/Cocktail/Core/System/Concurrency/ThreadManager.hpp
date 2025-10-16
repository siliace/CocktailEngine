#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADMANAGER_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADMANAGER_HPP

namespace Ck
{
	class Thread;
	class Runnable;

	/**
	 * \brief Interface for creating and managing threads
	 *
	 * Provides methods to spawn threads, access the current thread,
	 * and subscribe to thread lifecycle events
	 */
	class ThreadManager
	{
	public:

		/**
		 * \brief Default destructor
		 */
		virtual ~ThreadManager() = default;

		/**
		 * \brief Returns a pointer to the currently executing thread
		 * \return Pointer to the current thread
		 */
		virtual Thread* GetCurrentThread() = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADMANAGER_HPP
