#ifndef COCKTAIL_CORE_APPLICATION_APPLICATION_HPP
#define COCKTAIL_CORE_APPLICATION_APPLICATION_HPP

#include <Cocktail/Core/Application/ServiceContainer.hpp>
#include <Cocktail/Core/Application/ServiceProvider.hpp>
#include <Cocktail/Core/Signal/Observable.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Application : public Extends<Application, ServiceContainer>, public Observable
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
			RegisterServiceProvider(T::New(Self(), std::forward<Args>(args)...));
		}

		/**
		 * \brief Register a new ServiceProvider
		 * \param serviceProvider The ServiceProvider to register
		 * \return A pointer to the registered ServiceProvider
		 */
		void RegisterServiceProvider(const Ref<ServiceProvider>& serviceProvider);

		/**
		 * \brief 
		 * \return 
		 */
		bool IsBooted() const;

		/**
		 * \brief 
		 */
		virtual void Boot();

		/**
		 * \brief 
		 */
		void Terminate();

		/**
		 * \brief 
		 * \return 
		 */
		virtual const std::vector<std::string>& GetArgv() const = 0;

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

	private:

		bool mBooted;
		std::vector<Ref<ServiceProvider>> mServiceProviders;
		Signal<Application*> mOnTerminate;
	};
}

#endif // COCKTAIL_CORE_APPLICATION_APPLICATION_HPP
