#ifndef COCKTAILENGINE_CORE_LOG_LOGSERVICEPROVIDER_HPP
#define COCKTAILENGINE_CORE_LOG_LOGSERVICEPROVIDER_HPP

#include <CocktailEngine/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	class COCKTAILENGINE_CORE_API LogServiceProvider : public ServiceProvider
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

#endif // COCKTAILENGINE_CORE_LOG_LOGSERVICEPROVIDER_HPP
