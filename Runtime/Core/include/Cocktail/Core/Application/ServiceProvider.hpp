#ifndef COCKTAIL_CORE_APPLICATION_SERVICEPROVIDER_HPP
#define COCKTAIL_CORE_APPLICATION_SERVICEPROVIDER_HPP

#include <functional>

#include <Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp>
#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>

namespace Ck
{
	class Application;

    /**
     * \brief Base class responsible for registering and booting application services
     *
     * A ServiceProvider encapsulates the logic required to register services into
     * an \ref Application instance and perform any initialization required once
     * all services are available.
     *
     * The lifecycle of a ServiceProvider is split into two distinct phases:
     * - Registration: services are declared and bound to the application
     * - Booting      : services are fully initialized and may safely interact
     *
     * Service providers can emit signals before and after the boot phase, allowing
     * external systems to hook into the initialization process.
     */
	class COCKTAIL_CORE_API ServiceProvider : public Observable
	{
	public:

	    ServiceProvider() = delete;

	    /**
         * \brief Constructor
         *
         * Creates a ServiceProvider associated with the given application.
         * The provider is not registered nor booted upon construction.
         *
         * \param application The application in which services will be registered
         */
		explicit ServiceProvider(Application* application);

		ServiceProvider(const ServiceProvider&) = delete;
		ServiceProvider(ServiceProvider&& other) noexcept = default;

	    /**
         * \brief Destructor
         *
         * Destroys the ServiceProvider. Does not unregister services.
         */
		~ServiceProvider() override;

	    ServiceProvider& operator=(const ServiceProvider&) = delete;
		ServiceProvider& operator=(ServiceProvider&& other) noexcept = default;

	    /**
         * \brief Registers services in the associated application
         *
         * This function invokes \ref DoRegister and allows the provider to bind
         * services, singletons or factories to the application.
         *
         * Registration should not assume that other services are already initialized.
         */
		void Register();

	    /**
         * \brief Boots all registered services
         *
         * Executes the boot phase of the service provider. This function:
         * - emits the \ref OnBoot signal
         * - invokes \ref DoBoot
         * - emits the \ref OnBooted signal
         *
         * Booting is guaranteed to happen only once.
         */
		void Boot();

	    /**
         * \brief Indicates whether the provider has been booted
         *
         * \return \c true if the provider has completed its boot phase,
         *         \c false otherwise
         */
		bool IsBooted() const;

	    /**
         * \brief Signal emitted just before the provider is booted
         *
         * Allows listeners to react before services are initialized.
         *
         * \return Signal invoked with the application and provider instance
         */
		Signal<Application*, ServiceProvider*>& OnBoot();

	    /**
         * \brief Signal emitted after the provider has been booted
         *
         * Allows listeners to react once services are fully initialized.
         *
         * \return Signal invoked with the application and provider instance
         */
		Signal<Application*, ServiceProvider*>& OnBooted();

	protected:

	    /**
         * \brief Registers services into the application
         *
         * This function must be overridden by derived providers to declare
         * and bind services.
         *
         * \param application Target application
         */
		virtual void DoRegister(Application* application);

	    /**
         * \brief Boots registered services
         *
         * This function must be overridden by derived providers to perform
         * service initialization that depends on other registered services.
         *
         * \param application Target application
         */
		virtual void DoBoot(Application* application);

	private:

		Application* mApplication;
		bool mBooted;	
		Signal<Application*, ServiceProvider*> mOnBoot;
		Signal<Application*, ServiceProvider*> mOnBooted;
	};
}

#endif // COCKTAIL_CORE_APPLICATION_SERVICEPROVIDER_HPP
