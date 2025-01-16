#ifndef COCKTAIL_CORE_LOG_LOGSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_LOG_LOGSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API LogServiceProvider : public Extends<LogServiceProvider, ServiceProvider>
	{
	public:

		/**
		 * \brief
		 * \param application
		 */
		explicit LogServiceProvider(const Ref<Application>& application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoRegister(const Ref<Application>& application) override;

		/**
		 * \brief 
		 * \param application 
		 */
		void DoBoot(const Ref<Application>& application) override;
	};
}

#endif // COCKTAIL_CORE_LOG_LOGSERVICEPROVIDER_HPP
