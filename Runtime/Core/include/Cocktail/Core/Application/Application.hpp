#ifndef COCKTAIL_CORE_APPLICATION_APPLICATION_HPP
#define COCKTAIL_CORE_APPLICATION_APPLICATION_HPP

#include <string>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Application/ServiceContainer.hpp>
#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>
#include <Cocktail/Core/Utility/Time/Instant.hpp>

#define COCKTAIL_REGISTER_SERVICE_PROVIDER(__ServiceProvider) \
	::Ck::Detail::ServiceProviderRegisterer<__ServiceProvider> COCKTAIL_CONCATENATE_STRING2(__ServiceProvider, Registerer)

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Application : public ServiceContainer, public Observable
	{
	public:
	
		/**
		 * \brief Destructor
		 */
		~Application() override;

		/**
		 * \brief Create and register a new ServiceProvider
		 * \tparam T The type of the ServiceProvider to create
		 * \tparam Args Types of the arguments to use to create the ServiceProvider
		 * \param args Arguments to use to create the ServiceProvider
		 * \return A pointer to the created ServiceProvider
		 */
		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<ServiceProvider, T>>>
		void RegisterServiceProvider(Args&&... args)
		{
			RegisterServiceProvider(std::make_unique<T>(this, std::forward<Args>(args)...));
		}

		/**
		 * \brief Register a new ServiceProvider
		 * \param serviceProvider The ServiceProvider to register
		 * \return A pointer to the registered ServiceProvider
		 */
		void RegisterServiceProvider(std::unique_ptr<ServiceProvider> serviceProvider);

		/**
		 * \brief 
		 * \return 
		 */
		bool IsBooted() const;

		/**
		 * \brief 
		 */
		void Boot();

		/**
		 * \brief
		 * \tparam T
		 * \param callback
		 */
		template <typename T, typename Callable>
		void BeforeBooted(Callable callback)
		{
			if (ServiceProvider* serviceProvider = FindServiceProvider<T>())
				Connect(serviceProvider->OnBoot(), callback);
		}

		/**
		 * \brief 
		 * \tparam T 
		 * \param callback 
		 */
		template <typename T, typename Callable>
		void AfterBooted(Callable callback)
		{
			if (ServiceProvider* serviceProvider = FindServiceProvider<T>())
			{
				Connect(serviceProvider->OnBooted(), callback);

				if (serviceProvider->IsBooted())
					std::invoke(callback, this, serviceProvider);
			}
		}

		/**
		 * \brief 
		 */
		void Terminate();

		/**
		 * \brief Get the duration since the application started
		 * \return The uptime
		 */
		Duration Uptime() const;

		/**
		 * \brief 
		 * \return 
		 */
		virtual const Array<std::string>& GetArgv() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual bool IsDebuggerPresent() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<Application*>& OnTerminate();

	protected:

		/**
		 * \brief
		 */
		Application();

		/**
		 * \brief 
		 * \tparam T 
		 * \return 
		 */
		template <typename T, typename = std::enable_if_t<std::is_base_of_v<ServiceProvider, T>>>
		ServiceProvider* FindServiceProvider() const
		{
			for (const std::unique_ptr<ServiceProvider>& serviceProvider : mServiceProviders)
			{
				if (typeid(*serviceProvider) == typeid(T))
					return serviceProvider.get();
			}
			return nullptr;
		}

	private:

		bool mBooted;
		Array<std::unique_ptr<ServiceProvider>> mServiceProviders;
		Signal<Application*> mOnTerminate;
		Instant mStart;
	};

	namespace Detail
	{
		/**
		 * \brief Helper class to register a service provider from everywhere
		 * \tparam T Type of the ServiceProvider to register
		 */
		template <typename T>
		class ServiceProviderRegisterer
		{
		public:

			ServiceProviderRegisterer()
			{
				ServiceFacadeBase::OnApplicationReady([](Application* application) {
					application->RegisterServiceProvider<T>();
				});
			}
		};
	}
}

#endif // COCKTAIL_CORE_APPLICATION_APPLICATION_HPP
