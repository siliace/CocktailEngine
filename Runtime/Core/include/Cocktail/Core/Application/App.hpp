#ifndef COCKTAIL_CORE_APPLICATION_APP_HPP
#define COCKTAIL_CORE_APPLICATION_APP_HPP

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Application/ServiceFacade.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API App : public ServiceFacade<Application>
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
			return ResolveFacadeInstance()->Invoke(std::forward<Callable>(callable));
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
			ResolveFacadeInstance()->RegisterServiceProvider<T>(std::forward<Args>(args)...);
		}

		/**
		 * \brief Register a new ServiceProvider
		 * \param serviceProvider The ServiceProvider to register
		 * \return A pointer to the registered ServiceProvider
		 */
		static void RegisterServiceProvider(std::unique_ptr<ServiceProvider> serviceProvider);

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
		static void BeforeBooted(Callable&& callback)
		{
			ResolveFacadeInstance()->BeforeBooted<T>(std::forward<Callable>(callback));
		}

		/**
		 * \brief
		 * \tparam T
		 * \param callback
		 */
		template <typename T, typename Callable>
		static void AfterBooted(Callable&& callback)
		{
			ResolveFacadeInstance()->AfterBooted<T>(std::forward<Callable>(callback));
		}

		/**
		 * \brief 
		 * \param exitCode 
		 * \param force 
		 * \param callSite 
		 */
		static void Exit(unsigned int exitCode = 0, bool force = false, std::string_view callSite = "");

		/**
		 * \brief Get the duration since the application started
		 * \return The uptime
		 */
		static Duration Uptime();

		/**
		 * \brief
		 * \return
		 */
		static const Array<std::string>& GetArgv();

		/**
		 * \brief
		 * \param name
		 * \return
		 */
		static std::string GetEnvironmentVariable(std::string_view name);

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

#endif // COCKTAIL_CORE_APPLICATION_APP_HPP
