#ifndef COCKTAILENGINE_CORE_SYSTEM_SYSTEMSERVICEPROVIDER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_SYSTEMSERVICEPROVIDER_HPP

#include <CocktailEngine/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class COCKTAILENGINE_CORE_API SystemServiceProvider : public ServiceProvider
    {
    public:

		/**
		 * \brief Constructor
		 * \param application The application where register services 
		 */
		explicit SystemServiceProvider(Application* application);

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

#endif // COCKTAILENGINE_CORE_SYSTEM_SYSTEMSERVICEPROVIDER_HPP
