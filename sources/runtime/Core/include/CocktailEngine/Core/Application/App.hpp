#ifndef COCKTAILENGINE_CORE_APPLICATION_APP_HPP
#define COCKTAILENGINE_CORE_APPLICATION_APP_HPP

#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Application/ServiceFacade.hpp>

namespace Ck
{
    /**
     * \brief Static facade for the active Application instance
     *
     * Provides process-wide access to application lifecycle operations, runtime
     * information, and service-container operations. Except for callbacks registered
     * before boot, the facade is available only from the application's pre-boot phase
     * until it is terminated.
     */
    class COCKTAILENGINE_CORE_API App : public ServiceFacade<Application>
    {
    public:

        /**
         * \brief Registers a non-owning binding for an existing service instance
         *
         * \tparam Abstract Type exposed by the binding
         * \param instance Existing instance to expose; may be null
         * \see ServiceContainer::Instance
         */
        template <typename Abstract>
        static void Instance(Abstract* instance)
        {
            ResolveFacadeInstance()->Instance<Abstract>(instance);
        }

        /**
         * \brief Associates an abstract service type with a concrete implementation
         *
         * \tparam Abstract Type exposed to consumers
         * \tparam Concrete Concrete implementation that resolves the abstract type
         * \see ServiceContainer::Alias
         */
        template <typename Abstract, typename Concrete>
        static void Alias()
        {
            ResolveFacadeInstance()->Alias<Abstract, Concrete>();
        }

        /**
         * \brief Registers a default-constructed singleton service
         *
         * \tparam Abstract Type exposed to consumers
         * \tparam Concrete Concrete implementation to construct
         * \param lazy When true, defers construction until the first resolution
         * \see ServiceContainer::Singleton
         */
        template <typename Abstract, typename Concrete = Abstract>
        static void Singleton(bool lazy = true)
        {
            ResolveFacadeInstance()->Singleton<Abstract, Concrete>(lazy);
        }

        /**
         * \brief Registers a singleton created by a dependency-injected factory
         *
         * \tparam Abstract Type exposed by the singleton binding
         * \tparam Callable Factory type returning UniquePtr<Abstract>
         * \param callable Factory used to create the singleton instance
         * \param lazy When true, defers construction until the first resolution
         * \see ServiceContainer::Singleton
         */
        template <typename Abstract, typename Callable>
        static void Singleton(Callable&& callable, bool lazy = true)
        {
            ResolveFacadeInstance()->Singleton<Abstract>(Forward<Callable>(callable), lazy);
        }

        /**
         * \brief Adds a decorator to a callable service binding
         *
         * \tparam Abstract Type exposed by the binding to decorate
         * \param extender Decorator factory to append
         * \see ServiceContainer::Decorate
         */
        template <typename Abstract>
        static void Decorate(ServiceDecoratorFactory<Abstract> extender)
        {
            ResolveFacadeInstance()->Decorate<Abstract>(Move(extender));
        }

        /**
         * \brief Resolves a service from the active application
         *
         * \tparam T Service type to resolve
         * \return The resolved service instance, or nullptr when no binding exists
         * \see ServiceContainer::Resolve
         */
        template <typename T>
        static T* Resolve()
        {
            return ResolveFacadeInstance()->Resolve<T>();
        }

        /**
         * \brief Invokes a callable after resolving its service pointer parameters
         *
         * \tparam Callable Callable type to invoke
         * \param callable Callable receiving zero or more service pointers
         * \return The value returned by \p callable
         * \see ServiceContainer::Invoke
         */
        template <typename Callable>
        static FunctionReturnType<Callable> Invoke(Callable&& callable)
        {
            return ResolveFacadeInstance()->Invoke(Forward<Callable>(callable));
        }

        /**
         * \brief Creates and registers a service provider
         *
         * \tparam T Type of the service provider to create
         * \tparam Args Types of arguments forwarded to the provider constructor
         * \param args Arguments forwarded to the provider constructor
         * \see Application::RegisterServiceProvider
         */
        template <typename T, typename... Args>
        static void RegisterServiceProvider(Args&&... args)
        {
            ResolveFacadeInstance()->RegisterServiceProvider<T>(Forward<Args>(args)...);
        }

        /**
         * \brief Registers an existing service provider
         *
         * \param serviceProvider Provider instance whose ownership is transferred to the application
         * \see Application::RegisterServiceProvider
         */
        static void RegisterServiceProvider(UniquePtr<ServiceProvider> serviceProvider);

        /**
         * \brief Checks whether the active application has completed its boot sequence
         *
         * \return True after Application::Boot has been called, false otherwise
         */
        static bool IsBooted();

        /**
         * \brief Boots the active application and its registered service providers
         *
         * \see Application::Boot
         */
        static void Boot();

        /**
         * \brief Registers a callback invoked before service providers are booted
         *
         * When no application is active, the callback is queued for the next
         * application's pre-boot phase. Otherwise, registration is delegated to the
         * active application.
         *
         * \tparam Callable Type of the callback receiving an Application pointer
         * \param callback Callback to invoke during the application's pre-boot phase
         * \see Application::BeforeBoot
         */
        template <typename Callable>
        static void BeforeBoot(Callable&& callback)
        {
            if (Application* application = GetApplicationFacade())
                application->BeforeBoot(Forward<Callable>(callback));
            else
                Detail::ServiceFacadeBase::OnPreApplicationBoot(Forward<Callable>(callback));
        }

        /**
         * \brief Registers a callback invoked after all service providers are booted
         *
         * When no application is active, the callback is queued for the next
         * application's post-boot phase. Otherwise, registration is delegated to the
         * active application.
         *
         * \tparam Callable Type of the callback receiving an Application pointer
         * \param callback Callback to invoke during the application's post-boot phase
         * \see Application::AfterBooted
         */
        template <typename Callable>
        static void AfterBooted(Callable&& callback)
        {
            if (Application* application = GetApplicationFacade())
                application->AfterBooted(Forward<Callable>(callback));
            else
                Detail::ServiceFacadeBase::OnPostApplicationBoot(Forward<Callable>(callback));
        }

        /**
         * \brief Registers a callback invoked after a service provider is booted
         *
         * \tparam T Type of the service provider to observe
         * \tparam Callable Type of the callback receiving the application and provider
         * \param callback Callback to invoke after the provider has booted
         * \see Application::AfterServiceProviderBooted
         */
        template <typename T, typename Callable>
        static void AfterServiceProviderBooted(Callable&& callback)
        {
            ResolveFacadeInstance()->AfterServiceProviderBooted<T>(Forward<Callable>(callback));
        }

        /**
         * \brief Requests a graceful application exit
         *
         * \param exitCode Exit code returned to the operating system
         * \param callSite Optional string describing where the exit was requested
         * \see Application::Exit
         */
        static void Exit(unsigned int exitCode = 0, StringView callSite = CK_TEXT(""));

        /**
         * \brief Checks whether an exit has been requested
         *
         * \return True once Exit has been called, false otherwise
         * \see Application::IsExitRequested
         */
        static bool IsExitRequested();

        /**
         * \brief Gets the exit code carried by the exit request
         *
         * \return The code passed to Exit, or 0 if no exit was requested
         * \see Application::GetExitCode
         */
        static unsigned int GetExitCode();

        /**
         * \brief Gets the signal emitted when an exit is requested
         *
         * \return Reference to the exit request signal
         * \see Application::OnExitRequested
         */
        static Signal<Application*>& OnExitRequested();

        /**
         * \brief Terminates the process without unwinding
         *
         * \note The facade requires an active application. A crash handler running
         *       outside that lifetime must hold the Application instance directly.
         *
         * \param exitCode Exit code returned to the operating system
         * \see Application::Abort
         */
        [[noreturn]] static void Abort(unsigned int exitCode);

        /**
         * \brief Gracefully terminates the active application
         *
         * \note Do not call this from inside the main loop, because it destroys
         *       services while their consumers may still be alive.
         * \see Application::Terminate
         */
        static void Terminate();

        /**
         * \brief Gets the duration since the application started
         *
         * \return Application uptime
         * \see Application::Uptime
         */
        static Duration Uptime();

        /**
         * \brief Gets the command-line arguments
         *
         * \return Array of argument strings
         * \see Application::GetArgv
         */
        static const Array<String>& GetArgv();

        /**
         * \brief Retrieves an environment variable
         *
         * \param name Name of the environment variable
         * \return Value when it exists, otherwise an empty Optional
         * \see Application::GetEnvironmentVariable
         */
        static Optional<String> GetEnvironmentVariable(StringView name);

        /**
         * \brief Checks whether a debugger is attached to the process
         *
         * \return True when a debugger is present, false otherwise
         * \see Application::IsDebuggerPresent
         */
        static bool IsDebuggerPresent();

        /**
         * \brief Gets the path to the running executable
         *
         * \return Filesystem path to the running executable
         * \see Application::GetExecutablePath
         */
        static Path GetExecutablePath();

        /**
         * \brief Gets the full command line used to launch the application
         *
         * \return Command line used to launch the application
         * \see Application::GetCommandLine
         */
        static String GetCommandLine();

        /**
         * \brief Gets the signal emitted when the application is terminating
         *
         * \return Reference to the termination signal
         * \see Application::OnTerminate
         */
        static Signal<Application*>& OnTerminate();
    };
}

#endif // COCKTAILENGINE_CORE_APPLICATION_APP_HPP
