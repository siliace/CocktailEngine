#ifndef COCKTAIL_CORE_LOG_LOGSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_LOG_LOGSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API LogServiceProvider : public ServiceProvider
	{
	public:

		/**
		 * \brief
		 * \param application
		 */
		explicit LogServiceProvider(Application* application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoRegister(Application* application) override;

		/**
		 * \brief 
		 * \param application 
		 */
		void DoBoot(Application* application) override;
	};
}

#endif // COCKTAIL_CORE_LOG_LOGSERVICEPROVIDER_HPP
