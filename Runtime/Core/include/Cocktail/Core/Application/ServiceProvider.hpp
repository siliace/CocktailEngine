#ifndef COCKTAIL_CORE_APPLICATION_SERVICEPROVIDER_HPP
#define COCKTAIL_CORE_APPLICATION_SERVICEPROVIDER_HPP

#include <functional>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>
#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>

namespace Ck
{
	class Application;

	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API ServiceProvider : public Extends<ServiceProvider, Object>, public Observable
	{
	public:

		/**
		 * \brief 
		 */
		ServiceProvider() = delete;

		/**
		 * \brief Constructor
		 * \param application The application where register services
		 */
		explicit ServiceProvider(const Ref<Application>& application);

		/**
		 * \brief 
		 */
		ServiceProvider(const ServiceProvider&) = delete;

		/**
		 * \brief 
		 * \param other 
		 */
		ServiceProvider(ServiceProvider&& other) noexcept = default;

		/**
		 * \brief Destructor
		 */
		~ServiceProvider();

		/**
		 * \brief 
		 */
		ServiceProvider& operator=(const ServiceProvider&) = delete;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		ServiceProvider& operator=(ServiceProvider&& other) noexcept = default;

		/**
		 * \brief Register services in the Application
		 */
		void Register();

		/**
		 * \brief Boot registered services
		 */
		void Boot();

		/**
		 * \brief Register a callback to invoke when the ServiceProvider is about to be booted
		 * \param callback The callback
		 */
		void OnBoot(const std::function<void(Ref<Application>, Ref<ServiceProvider>)>& callback);

		/**
		 * \brief Register a callback to invoke when the ServiceProvider has been booted
		 * \param callback The callback
		 */
		void OnBooted(const std::function<void(Ref<Application>, Ref<ServiceProvider>)>& callback);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		virtual void DoRegister(const Ref<Application>& application);

		/**
		 * \brief
		 * \param application
		 */
		virtual void DoBoot(const Ref<Application>& application);

	private:

		Ref<Application> mApplication;
		bool mBooted;	
		Signal<Ref<Application>, Ref<ServiceProvider>> mOnBoot;
		Signal<Ref<Application>, Ref<ServiceProvider>> mOnBooted;
	};
}

#endif // COCKTAIL_CORE_APPLICATION_SERVICEPROVIDER_HPP
