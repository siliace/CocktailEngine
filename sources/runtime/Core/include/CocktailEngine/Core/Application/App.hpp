#ifndef COCKTAILENGINE_CORE_APPLICATION_APP_HPP
#define COCKTAILENGINE_CORE_APPLICATION_APP_HPP

#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Application/ServiceFacade.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_CORE_API App : public ServiceFacade<Application>
	{
	public:

		/**
		 * \brief
		 * \tparam Abstract
		 * \param instance
		 */
		template <typename Abstract>
		static void Instance(Abstract* instance)
		{
			ResolveFacadeInstance()->Instance<Abstract>(instance);
		}

		/**
		 * \brief Register a shared binding in the container
		 * \tparam Abstract The abstract type to bind
		 * \tparam Concrete The concrete implementation to resolve
		 */
		template <typename Abstract, typename Concrete = Abstract>
		static void Singleton()
		{
			ResolveFacadeInstance()->Singleton<Abstract, Concrete>();
		}

		/**
		 * \brief Register a shared binding in the container
		 * \tparam Concrete The concrete type to bind
		 * \tparam Args Arguments of the callable
		 * \tparam Callable The type of the callable to invoke to create the concrete instance
		 * \param callable The callback to invoke to instantiate the binding
		 */
		template <typename Concrete, typename... Args, typename Callable>
		static void Singleton(Callable&& callable)
		{
			ResolveFacadeInstance()->Singleton<Concrete, Args..., Callable>(callable);
		}

		/**
		 * \brief
		 * \tparam T
		 * \return
		 */
		template <typename T>
		static T* Resolve()
		{
			return ResolveFacadeInstance()->Resolve<T>();
		}

		/**
		 * \brief Invoke a callable by resolving this parameters from the service container
		 * \param callable The callable to call
		 * \return The return value of the callable
		 */
		template <typename Callable>
		static FunctionReturnType<Callable> Invoke(Callable&& callable)
		{
			return ResolveFacadeInstance()->Invoke(Forward<Callable>(callable));
		}

		/**
		 * \brief Create and register a new service provider
		 * \tparam T The type of the service provider to create
		 * \tparam Args Types of the arguments to use to create the service provider
		 * \param args Arguments to use to create the service provider
		 * \return A pointer to the created service provider
		 */
		template <typename T, typename... Args>
		static void RegisterServiceProvider(Args&&... args)
		{
			ResolveFacadeInstance()->RegisterServiceProvider<T>(Forward<Args>(args)...);
		}

		/**
		 * \brief Register a new ServiceProvider
		 * \param serviceProvider The ServiceProvider to register
		 * \return A pointer to the registered ServiceProvider
		 */
		static void RegisterServiceProvider(UniquePtr<ServiceProvider> serviceProvider);

		/**
		 * \brief 
		 * \return 
		 */
		static bool IsBooted();

		/**
		 * \brief 
		 */
		static void Boot();

		/**
		 * \brief
		 * \tparam T
		 * \param callback
		 */
		template <typename T, typename Callable>
		static void AfterBooted(Callable&& callback)
		{
			ResolveFacadeInstance()->AfterBooted<T>(Forward<Callable>(callback));
		}

		/**
		 * \brief Request the application to exit
		 * \param exitCode Exit code to return to the operating system
		 * \param callSite Optional string describing where the exit was requested
		 * \see Application::Exit
		 */
		static void Exit(unsigned int exitCode = 0, StringView callSite = CK_TEXT(""));

		/**
		 * \brief Check whether an exit has been requested
		 * \return True once Exit has been called, false otherwise
		 */
		static bool IsExitRequested();

		/**
		 * \brief Get the exit code carried by the exit request
		 * \return The code passed to Exit, or 0 if no exit was requested
		 */
		static unsigned int GetExitCode();

		/**
		 * \brief Signal emitted when an exit is requested
		 * \return Reference to the exit request signal
		 */
		static Signal<Application*>& OnExitRequested();

		/**
		 * \brief Terminate the process without unwinding
		 *
		 * \note Like every facade, this needs a booted application. A crash handler
		 *       running outside that window has to hold the Application itself.
		 *
		 * \param exitCode Exit code returned to the operating system
		 * \see Application::Abort
		 */
		[[noreturn]] static void Abort(unsigned int exitCode);

		/**
		 * \brief Get the duration since the application started
		 * \return The uptime
		 */
		static Duration Uptime();

		/**
		 * \brief
		 * \return
		 */
		static const Array<String>& GetArgv();

		/**
		 * \brief
		 * \param name
		 * \return
		 */
		static Optional<String> GetEnvironmentVariable(StringView name);

		/**
		 * \brief
		 * \return
		 */
		static bool IsDebuggerPresent();

		/**
		 * \brief
		 * \return
		 */
		static Signal<Application*>& OnTerminate();
	};
}

#endif // COCKTAILENGINE_CORE_APPLICATION_APP_HPP
