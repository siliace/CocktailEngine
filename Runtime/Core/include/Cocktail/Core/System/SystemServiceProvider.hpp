#ifndef COCKTAIL_CORE_SYSTEM_SYSTEMSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_SYSTEM_SYSTEMSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class COCKTAIL_CORE_API SystemServiceProvider : public ServiceProvider
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

#endif // COCKTAIL_CORE_SYSTEM_SYSTEMSERVICEPROVIDER_HPP
