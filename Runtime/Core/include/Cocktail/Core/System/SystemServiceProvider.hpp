#ifndef COCKTAIL_CORE_SYSTEM_SYSTEMSERVICEPROVIDER_HPP
#define COCKTAIL_CORE_SYSTEM_SYSTEMSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class COCKTAIL_CORE_API SystemServiceProvider : public Extends<SystemServiceProvider, ServiceProvider>
    {
    public:

		/**
		 * \brief Constructor
		 * \param application The application where register services 
		 */
		explicit SystemServiceProvider(const Ref<Application>& application);

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

#endif // COCKTAIL_CORE_SYSTEM_SYSTEMSERVICEPROVIDER_HPP
