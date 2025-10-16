#ifndef COCKTAIL_CORE_APPLICATION_SERVICECONTAINER_HPP
#define COCKTAIL_CORE_APPLICATION_SERVICECONTAINER_HPP

#include <memory>
#include <typeindex>
#include <unordered_map>

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/Application/Detail/InstanceServiceBinding.hpp>
#include <Cocktail/Core/Application/Detail/SingletonServiceBinding.hpp>
#include <Cocktail/Core/Utility/FunctionTraits.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	class COCKTAIL_CORE_API ServiceContainer
	{
	public:

		ServiceContainer() = default;
		ServiceContainer(const ServiceContainer&) = delete;
		ServiceContainer(ServiceContainer&&) noexcept= default;

		ServiceContainer& operator=(const ServiceContainer&) = delete;
		ServiceContainer& operator=(ServiceContainer&&) noexcept= default;

		/**
		 * \brief 
		 * \tparam Abstract 
		 * \tparam Concrete 
		 */
		template <typename Abstract, typename Concrete>
		void Alias()
		{
			Alias(typeid(Abstract), typeid(Concrete));
		}

		/**
		 * \brief 
		 * \param abstract 
		 * \param concrete 
		 */
		void Alias(const std::type_info& abstract, const std::type_info& concrete);

		/**
		 * \brief 
		 * \tparam Abstract 
		 * \param instance 
		 */
		template <typename Abstract>
		void Instance(Abstract* instance)
		{
			RegisterBinding(
				std::make_unique<Detail::InstanceServiceBinding<Abstract>>(instance)
			);
		}

		/**
		 * \brief
		 * \tparam Abstract
		 * \tparam Concrete
		 * \param lazy
		 */
		template <typename Abstract, typename Concrete = Abstract>
		void Singleton(bool lazy = true)
		{
			Singleton<Abstract>([]() -> std::unique_ptr<Abstract> {
				return std::make_unique<Concrete>();
			}, lazy);

			if constexpr (!std::is_same_v<Abstract, Concrete>)
				Alias<Concrete, Abstract>();
		}

		/**
		 * \brief
		 * \tparam Abstract
		 * \tparam Callable
		 * \param callable
		 * \param lazy
		 */
		template <typename Abstract, typename Callable>
		void Singleton(Callable&& callable, bool lazy = true)
		{
			static_assert(std::is_same_v<std::unique_ptr<Abstract>, FunctionReturnType<Callable>>);

			RegisterBinding(
				std::make_unique<Detail::SingletonServiceBinding<Abstract>>(
					this,
					MakeResolver<Abstract>(std::forward<Callable>(callable)),
					lazy
				)
			);
		}

		/**
		 * \brief 
		 * \tparam Abstract 
		 */
		template <typename Abstract>
		void Decorate(ServiceDecoratorFactory<Abstract> extender)
		{
			const std::type_info& abstract = typeid(Abstract);
			const std::type_info& concrete = GetConcrete(abstract);
			if (auto itBinding = mBindings.find(concrete); itBinding != mBindings.end())
			{
				auto binding = static_cast<Detail::ServiceBinding<Abstract>*>(itBinding->second.get());
				if (binding->IsCallable())
				{
					auto callableBinding = static_cast<Detail::CallableServiceBinding<Abstract>*>(binding);
					callableBinding->Decorate(std::move(extender));
				}
			}
		}

		template <typename T>
		T* Resolve()
		{
			const std::type_info& abstract = typeid(T);
			const std::type_info& concrete = GetConcrete(abstract);
			if (auto itBinding = mBindings.find(concrete); itBinding != mBindings.end())
			{
				auto binding = static_cast<Detail::ServiceBinding<T>*>(itBinding->second.get());
				return binding->Resolve();
			}

			return nullptr;
		}

		/**
		 * \brief Invoke a callable by resolving its parameters from the service container
		 * \param callable The callable to call
		 * \return The return value of the callable
		 */
		template <typename Callable>
		FunctionReturnType<Callable> Invoke(Callable&& callable)
		{
			auto resolver = [callback = std::forward<Callable>(callable)](ServiceContainer* self) -> FunctionReturnType<Callable> {
				if constexpr (FunctionTraits<Callable>::Arity > 0)
				{
					using ParameterResolverHelperType = typename FunctionTraits<Callable>::Arguments::template Apply<ParameterResolverHelper>;

					auto parameters = ParameterResolverHelperType::Resolve(self);

					return std::apply(callback, parameters);
				}
				else
				{
					return callback();
				}
			};

			return resolver(this);
		}

	protected:

		/**
		 * \brief 
		 */
		void Clear();

	private:

		template <typename T, typename... Others>
		struct ParameterResolverHelper
		{
			static auto Resolve(ServiceContainer* container) -> decltype(auto)
			{
				return std::tuple_cat(
					ParameterResolverHelper<T>::Resolve(container),
					ParameterResolverHelper<Others...>::Resolve(container)
				);
			}
		};

		template <typename T>
		struct ParameterResolverHelper<T>
		{
			using Resolved = std::remove_pointer_t<std::decay_t<T>>;

			static auto Resolve(ServiceContainer* container) -> decltype(auto)
			{
				return std::make_tuple(container->Resolve<Resolved>());
			}
		};

		/**
		 * \brief 
		 * \tparam T 
		 * \tparam Callable 
		 * \param callable 
		 * \return 
		 */
		template <typename T, typename Callable>
		static Detail::ServiceResolver<T> MakeResolver(Callable&& callable)
		{
			return [callback = std::forward<Callable>(callable)](ServiceContainer* self) -> std::unique_ptr<T> {
				if constexpr (FunctionTraits<Callable>::Arity > 0)
				{
					using ParameterResolverHelperType = typename FunctionTraits<Callable>::Arguments::template Apply<ParameterResolverHelper>;

					auto parameters = ParameterResolverHelperType::Resolve(self);

					return std::apply(callback, parameters);
				}
				else
				{
					return callback();
				}
			};
		}

		/**
		 * \brief 
		 * \param binding 
		 */
		void RegisterBinding(std::unique_ptr<Detail::ServiceBindingBase> binding);

		/**
		 * \brief 
		 * \param abstract 
		 * \return 
		 */
		const std::type_info& GetConcrete(const std::type_info& abstract) const;

		std::unordered_map<std::type_index, std::unique_ptr<Detail::ServiceBindingBase>> mBindings;
		std::unordered_map<std::type_index, std::reference_wrapper<const std::type_info>> mAliases;
	};
}

#endif // COCKTAIL_CORE_APPLICATION_SERVICECONTAINER_HPP
