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
	 * \brief 
	 */
	class COCKTAIL_CORE_API ServiceProvider : public Observable
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
		explicit ServiceProvider(Application* application);

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
		~ServiceProvider() override;

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
		 * \brief 
		 * \return 
		 */
		bool IsBooted() const;

		/**
		 * \brief Register a callback to invoke when the ServiceProvider is about to be booted
		 */
		Signal<Application*, ServiceProvider*>& OnBoot();

		/**
		 * \brief Register a callback to invoke when the ServiceProvider has been booted
		 */
		Signal<Application*, ServiceProvider*>& OnBooted();

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		virtual void DoRegister(Application* application);

		/**
		 * \brief
		 * \param application
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
