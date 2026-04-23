#ifndef COCKTAIL_CORE_APPLICATION_APPLICATION_HPP
#define COCKTAIL_CORE_APPLICATION_APPLICATION_HPP

#include <Cocktail/Core/Application/ServiceContainer.hpp>
#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Memory/Allocator/UseSystemAllocator.hpp>
#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/System/FileSystem/Path.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>
#include <Cocktail/Core/Utility/Time/Instant.hpp>

#define COCKTAIL_REGISTER_SERVICE_PROVIDER(__ServiceProvider) \
	::Ck::Detail::ServiceProviderRegisterer<__ServiceProvider> COCKTAIL_CONCATENATE_STRING2(__ServiceProvider, Registerer)

namespace Ck
{
    /**
     * \brief Core application class responsible for managing lifecycle and services.
     *
     * The Application class acts as the central entry point of the program.
     * It manages ServiceProviders, application lifecycle (boot, terminate),
     * and provides access to environment and runtime information.
     *
     * It also exposes signals to observe lifecycle events.
     */
	class COCKTAIL_CORE_API Application : public ServiceContainer, public Observable, public UseSystemAllocator
	{
	public:
	
		/**
	     * \brief Destructor
         *
         * Ensures proper cleanup of registered services and resources.
		 */
		~Application() override;

	    /**
         * \brief Create and register a new ServiceProvider
         *
         * Constructs a ServiceProvider of type \p T and registers it
         * into the application.
         *
         * \tparam T    Type of the ServiceProvider to create (must derive from ServiceProvider)
         * \tparam Args Types of the arguments used to construct the ServiceProvider
         *
         * \param args  Arguments forwarded to the ServiceProvider constructor
         */
		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<ServiceProvider, T>>>
		void RegisterServiceProvider(Args&&... args)
		{
			RegisterServiceProvider(MakeUnique<T>(this, std::forward<Args>(args)...));
		}

	    /**
         * \brief Register an existing ServiceProvider
         *
         * \param serviceProvider The ServiceProvider instance to register
         */
		void RegisterServiceProvider(UniquePtr<ServiceProvider> serviceProvider);

	    /**
         * \brief Check whether the application has been booted
         *
         * \return True if Boot() has been called, false otherwise
         */
		bool IsBooted() const;

	    /**
         * \brief Boot the application
         *
         * Initializes all registered ServiceProviders and triggers their boot sequence.
         */
		void Boot();

	    /**
         * \brief Register a callback executed before a ServiceProvider is booted
         *
         * The callback is connected to the ServiceProvider's "OnBoot" signal.
         *
         * \tparam T Type of the ServiceProvider
         * \tparam Callable Type of the callback function
         *
         * \param callback  Function to invoke before boot
         */
		template <typename T, typename Callable>
		void BeforeBooted(Callable callback)
		{
			if (ServiceProvider* serviceProvider = FindServiceProvider<T>())
				Connect(serviceProvider->OnBoot(), callback);
		}

	    /**
         * \brief Register a callback executed after a ServiceProvider is booted
         *
         * The callback is connected to the ServiceProvider's "OnBooted" signal.
         * If the ServiceProvider is already booted, the callback is invoked immediately.
         *
         * \tparam T Type of the ServiceProvider
         * \tparam Callable Type of the callback function
         *
         * \param callback  Function to invoke after boot
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
         * \brief Request application exit
         *
         * \param exitCode Exit code to return to the operating system
         * \param force If true, forces immediate exit
         * \param callSite Optional string describing where the exit was requested
         */
		virtual void Exit(unsigned int exitCode = 0, bool force = false, StringView callSite = CK_TEXT("")) = 0;

		/**
		 * \brief Gracefully terminate the application
		 *
	     * Disconnects all signals, destroys all registered ServiceProviders,
         * and disables all ServiceFacade instances.
		 */
		void Terminate();

	    /**
         * \brief Get the time elapsed since the application started
         *
         * \return Application uptime
         */
		Duration Uptime() const;

	    /**
         * \brief Get the command line arguments
         *
         * \return Array of argument strings
         */
		virtual const Array<String>& GetArgv() const = 0;

	    /**
         * \brief Retrieve an environment variable
         *
         * \param name Name of the environment variable
         *
         * \return The value if found, otherwise an empty Optional
         */
		virtual Optional<String> GetEnvironmentVariable(StringView name) = 0;

	    /**
         * \brief Check if a debugger is attached to the process
         *
         * \return True if a debugger is present, false otherwise
         */
		virtual bool IsDebuggerPresent() const = 0;

	    /**
         * \brief Get the path of the executable
         *
         * \return Filesystem path to the running executable
         */
		virtual Path GetExecutablePath() const = 0;

	    /**
         * \brief Get the full command line string
         *
         * \return Command line used to launch the application
         */
	    virtual String GetCommandLine() const = 0;

	    /**
         * \brief Signal emitted when the application is terminating
         *
         * \return Reference to the termination signal
         */
		Signal<Application*>& OnTerminate();

	protected:

	    /**
         * \brief Constructor
         *
         * Initializes internal state of the application.
         */
		Application();

	    /**
         * \brief Find a registered ServiceProvider by type
         *
         * \tparam T Type of the ServiceProvider to find
         *
         * \return Pointer to the ServiceProvider if found, nullptr otherwise
         */
		template <typename T, typename = std::enable_if_t<std::is_base_of_v<ServiceProvider, T>>>
		ServiceProvider* FindServiceProvider() const
		{
			for (const UniquePtr<ServiceProvider>& serviceProvider : mServiceProviders)
			{
				if (typeid(*serviceProvider) == typeid(T))
					return serviceProvider.Get();
			}
			return nullptr;
		}

	private:

		bool mBooted; /*!< Indicates whether the application has been booted */
		Array<UniquePtr<ServiceProvider>> mServiceProviders; /*!< Registered service providers */
		Signal<Application*> mOnTerminate; /*!< Signal emitted on application termination */
		Instant mStart; /*!< Time point when the application started */
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
