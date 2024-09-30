#ifndef COCKTAIL_CORE_UTILITY_WORKER_WORKERSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_UTILITY_WORKER_WORKERSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API WorkerServiceProvider : public Extends<WorkerServiceProvider, ServiceProvider>
	{
	public:

		/**
		 * \brief 
		 * \param application 
		 */
		explicit WorkerServiceProvider(const Ref<Application>& application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoRegister(const Ref<Application>& application) override;
	};
}

#endif // COCKTAIL_CORE_UTILITY_WORKER_WORKERSERVICEPROVIDER_HPP
