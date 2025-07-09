#ifndef COCKTAIL_CORE_APPLICATION_DETAIL_CALLABLESERVICEBINDING_HPP
#define COCKTAIL_CORE_APPLICATION_DETAIL_CALLABLESERVICEBINDING_HPP

#include <functional>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Application/Detail/ServiceBinding.hpp>

namespace Ck
{
	class ServiceContainer;

	template <typename T>
	using ServiceDecoratorFactory = std::function<std::unique_ptr<T>(std::unique_ptr<T>)>;

	namespace Detail
	{
		template <typename T>
		using ServiceResolver = std::function<std::unique_ptr<T>(ServiceContainer*)>;

		template <typename T>
		class CallableServiceBinding : public ServiceBinding<T>
		{
		public:

			/**
			 * \brief 
			 * \param extender 
			 */
			void Decorate(ServiceDecoratorFactory<T> extender)
			{
				mDecoratorFactories.push_back(extender);
			}

			/**
			 * \brief 
			 * \return 
			 */
			bool IsCallable() const override
			{
				return true;
			}

		protected:

			/**
			 * \brief
			 * \return
			 */
			std::unique_ptr<T> InvokeDecorators(std::unique_ptr<T> instance)
			{
				for (const ServiceDecoratorFactory<T>& decoratorFactory : mDecoratorFactories)
					instance = decoratorFactory(std::move(instance));

				return instance;
			}

		private:

			Array<ServiceDecoratorFactory<T>> mDecoratorFactories;
		};
	}
}

#endif // COCKTAIL_CORE_APPLICATION_DETAIL_CALLABLESERVICEBINDING_HPP
