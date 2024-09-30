#ifndef COCKTAIL_CORE_APPLICATION_DETAIL_CALLABLESERVICEBINDING_HPP
#define COCKTAIL_CORE_APPLICATION_DETAIL_CALLABLESERVICEBINDING_HPP

#include <functional>

#include <Cocktail/Core/Application/Detail/ServiceBinding.hpp>

namespace Ck
{
	class ServiceContainer;

	template <typename T>
	using ServiceDecoratorFactory = std::function<Ref<T>(Ref<T>)>;

	namespace Detail
	{
		template <typename T>
		using ServiceResolver = std::function<Ref<T>(const Ref<ServiceContainer>&)>;

		template <typename T>
		class CallableServiceBinding : public Implements<CallableServiceBinding<T>, ServiceBinding<T>>
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
			Ref<T> InvokeDecorators(Ref<T> instance)
			{
				for (const ServiceDecoratorFactory<T>& decoratorFactory : mDecoratorFactories)
					instance = decoratorFactory(std::move(instance));

				return instance;
			}

		private:

			std::vector<ServiceDecoratorFactory<T>> mDecoratorFactories;
		};
	}
}

#endif // COCKTAIL_CORE_APPLICATION_DETAIL_CALLABLESERVICEBINDING_HPP
