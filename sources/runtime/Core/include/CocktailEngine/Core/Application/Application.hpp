#ifndef COCKTAILENGINE_CORE_APPLICATION_APPLICATION_HPP
#define COCKTAILENGINE_CORE_APPLICATION_APPLICATION_HPP

#include <atomic>

#include <CocktailEngine/Core/Application/ServiceContainer.hpp>
#include <CocktailEngine/Core/Application/ServiceProvider.hpp>
#include <CocktailEngine/Core/Memory/Allocator/UseSystemAllocator.hpp>
#include <CocktailEngine/Core/Signal/Observable.hpp>
#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/System/FileSystem/Path.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>
#include <CocktailEngine/Core/Utility/Time/Instant.hpp>

#define COCKTAIL_REGISTER_SERVICE_PROVIDER(__ServiceProvider) ::Ck::Detail::ServiceProviderRegisterer<__ServiceProvider> COCKTAIL_CONCATENATE_STRING2(__ServiceProvider, Registerer)

namespace Ck
{
    namespace Detail
    {
        /**
         * \brief Coordinates the active application instance with static service facades
         *
         * This internal base exposes the application to ServiceFacade specializations
         * only while it is booted. It also stores one-shot callbacks for the phases
         * immediately before and after registered service providers are booted.
         */
        class COCKTAILENGINE_CORE_API ServiceFacadeBase
        {
        public:

            /**
             * \brief Gets the application currently available to service facades
             *
             * \return The active application, or nullptr before boot and after termination
             */
            static Application* GetApplicationFacade();

            /**
             * \brief Registers a callback invoked before service providers are booted
             *
             * The callback is invoked once during the next application boot, after
             * service registration has completed and before any provider is booted.
             *
             * \tparam Callable Type of the callback receiving an Application pointer
             * \param callable Callback to invoke during the pre-boot phase
             */
            template <typename Callable>
            static void OnPreApplicationBoot(Callable&& callable)
            {
                sPreBootConnections.Add(sOnApplicationPreBoot.Connect(callable));
            }

            /**
             * \brief Registers a callback invoked after service providers are booted
             *
             * The callback is invoked once during the next application boot, after
             * all registered providers have completed their boot phase.
             *
             * \tparam Callable Type of the callback receiving an Application pointer
             * \param callable Callback to invoke during the post-boot phase
             */
            template <typename Callable>
            static void OnPostApplicationBoot(Callable&& callable)
            {
                sPostBootConnections.Add(sOnApplicationPostBoot.Connect(callable));
            }

        private:

            friend Application;

            /**
             * \brief Makes an application available to service facades and emits pre-boot callbacks
             *
             * Called by Application::Boot after service registration and before service
             * providers enter their boot phase.
             *
             * \param application Application beginning its boot sequence
             */
            static void PreBoot(Application* application);

            /**
             * \brief Emits callbacks registered for the end of the boot sequence
             *
             * \param application Application whose service providers have been booted
             */
            static void PostBoot(Application* application);

            /**
             * \brief Ends the lifetime during which service facades can access the application
             */
            static void Terminate();

            static Application* sFacadeApplication; /*!< Application exposed through service facades. */
            static Array<Connection> sPreBootConnections; /*!< Connections retained until pre-boot callbacks are emitted. */
            static Signal<Application*> sOnApplicationPreBoot; /*!< Signal emitted before service providers are booted. */
            static Array<Connection> sPostBootConnections; /*!< Connections retained until post-boot callbacks are emitted. */
            static Signal<Application*> sOnApplicationPostBoot; /*!< Signal emitted after service providers are booted. */
        };
    }

    /**
     * \brief Core application class responsible for managing lifecycle and services.
     *
     * The Application class acts as the central entry point of the program.
     * It manages ServiceProviders, application lifecycle (boot, terminate),
     * and provides access to environment and runtime information.
     *
     * It also exposes signals to observe lifecycle events.
     */
    class COCKTAILENGINE_CORE_API Application : public ServiceContainer, public Observable, public UseSystemAllocator
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
            RegisterServiceProvider(MakeUnique<T>(this, Forward<Args>(args)...));
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
         * \brief Register a callback executed after the application is booted
         *
         * If the Application is already booted, the callback is invoked immediately.
         *
         * \tparam Callable Type of the callback function
         *
         * \param callback Function to invoke after boot
         */
        template <typename Callable>
        void BeforeBoot(Callable callback)
        {
            if (!mBooted)
            {
                Detail::ServiceFacadeBase::OnPreApplicationBoot(Forward<Callable>(callback));
            }
            else
            {
                std::invoke(callback, this);
            }
        }

        /**
         * \brief Register a callback executed after the application is booted
         *
         * If the Application is already booted, the callback is invoked immediately.
         *
         * \tparam Callable Type of the callback function
         *
         * \param callback Function to invoke after boot
         */
        template <typename Callable>
        void AfterBooted(Callable callback)
        {
            if (!mBooted)
            {
                Detail::ServiceFacadeBase::OnPostApplicationBoot(Forward<Callable>(callback));
            }
            else
            {
                std::invoke(callback, this);
            }
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
         * \param callback Function to invoke after boot
         */
        template <typename T, typename Callable>
        void AfterServiceProviderBooted(Callable callback)
        {
            if (ServiceProvider* serviceProvider = FindServiceProvider<T>())
            {
                if (!serviceProvider->IsBooted())
                {
                    Connect(serviceProvider->OnBooted(), callback);
                }
                else
                {
                    std::invoke(callback, this, serviceProvider);
                }
            }
        }

        /**
         * \brief Request the application to exit
         *
         * Records the request, remembers the exit code and emits OnExitRequested.
         * Nothing is destroyed here: the main loop observes the request through
         * IsExitRequested, leaves the entry point, and the shutdown happens by
         * unwinding its stack. That unwinding is what idles the graphic device and
         * destroys the windows in the order they were created in, which no teardown
         * done from here could reproduce.
         *
         * Requesting an exit is therefore not a platform matter, and this method is
         * not virtual: the two platforms cannot disagree on what it means. What a
         * platform decides is only how to kill a process that can no longer unwind,
         * which is Abort.
         *
         * The first request wins. A second one is ignored rather than overwriting
         * the exit code a loop may already have read.
         *
         * Safe to call from any thread, and before Boot: a request made before the
         * application is booted is seen by whoever boots it.
         *
         * \param exitCode Exit code to return to the operating system. Only values from
         *                 0 to 255 travel to a process status everywhere: Unix keeps
         *                 the low eight bits of it and Windows keeps all of them.
         * \param callSite Optional string describing where the exit was requested
         */
        void Exit(unsigned int exitCode = 0, StringView callSite = CK_TEXT(""));

        /**
         * \brief Check whether an exit has been requested
         *
         * This is what a main loop tests to know whether to run another iteration.
         *
         * \return True once Exit has been called, false otherwise
         */
        bool IsExitRequested() const;

        /**
         * \brief Get the exit code carried by the exit request
         *
         * \return The code passed to Exit, or 0 if no exit was requested
         */
        unsigned int GetExitCode() const;

        /**
         * \brief Signal emitted when an exit is requested
         *
         * Emitted once, from the thread that called Exit.
         *
         * \return Reference to the exit request signal
         */
        Signal<Application*>& OnExitRequested();

        /**
         * \brief Terminate the process without unwinding
         *
         * The only part of an exit a platform has a say in, for the cases where
         * unwinding is not possible any more: a crash handler, a watchdog.
         *
         * No destructor runs, no atexit handler runs, nothing is flushed. Prefer
         * Exit everywhere else.
         *
         * Never returns, on any platform. The platform primitives behind it do not
         * agree on that by themselves, so an implementation has to make it true rather
         * than leave a caller with code that runs on one platform and not the other.
         *
         * \param exitCode Exit code returned to the operating system
         */
        [[noreturn]] virtual void Abort(unsigned int exitCode) = 0;

        /**
         * \brief Gracefully terminate the application
         *
         * Disconnects all signals, destroys all registered ServiceProviders,
         * and disables all ServiceFacade instances.
         *
         * \note Never call this from inside the main loop. It destroys every service
         *       while the objects using them are still alive. It belongs after the
         *       entry point returned, which is where the engine already calls it.
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
        /**
         * \brief Requested exit code, negative while no exit has been requested
         *
         * The state and the code it carries are one value on purpose. Held apart, the
         * flag announcing the request would be published before the code it announces,
         * and a thread seeing the request could read a code that was not written yet.
         * Wider than the code so that "no request" has a value of its own.
         */
        std::atomic<Int64> mExitRequest;
        Array<UniquePtr<ServiceProvider>> mServiceProviders; /*!< Registered service providers */
        Signal<Application*> mOnExitRequested; /*!< Signal emitted when an exit is requested */
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
                ServiceFacadeBase::OnPostApplicationBoot([](Application* application) {
                    application->RegisterServiceProvider<T>();
                });
            }
        };
    }
}

#endif // COCKTAILENGINE_CORE_APPLICATION_APPLICATION_HPP
