#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_RUNNABLE_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_RUNNABLE_HPP

namespace Ck
{
	/**
	 * \brief Represents a runnable task that can be executed by a thread
	 *
	 * Implement this interface to define work that will run in a separate thread
	 * The thread lifecycle typically follows: Init() -> Run() -> Stop()
	 */
	class Runnable
	{
	public:

		/**
		 * \brief Default destructor
		 */
		virtual ~Runnable() = default;

		/**
		 * \brief Initializes resources before the thread starts running
		 *
		 * Called once before Run()
		 */
		virtual void Init() = 0;

		/**
		 * \brief Main execution method of the thread
		 *
		 * This method is executed on the thread It should typically contain
		 * the task's main loop or logic
		 *
		 * \return The return code of the thread
		 */
		virtual unsigned int Run() = 0;

		/**
		 * \brief Stops the thread gracefully
		 *
		 * Called to request a clean shutdown of the task
		 * This should set internal flags to exit Run()
		 */
		virtual void Stop() = 0;

		/**
		 * \brief Cleanup resources before the thread ends
		 *
		 * Called once after Run()
		 */
		virtual void Exit() = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_RUNNABLE_HPP
