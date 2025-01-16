#ifndef COCKTAIL_CORE_APPLICATION_SERVICECONTAINER_HPP
#define COCKTAIL_CORE_APPLICATION_SERVICECONTAINER_HPP

#include <typeindex>
#include <unordered_map>

#include <Cocktail/Core/Application/Detail/FactoryServiceBinding.hpp>
#include <Cocktail/Core/Application/Detail/InstanceServiceBinding.hpp>
#include <Cocktail/Core/Application/Detail/SingletonServiceBinding.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/Utility/FunctionTraits.hpp>
#include <Cocktail/Core/Utility/RefTraits.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	class COCKTAIL_CORE_API ServiceContainer : public Extends<ServiceContainer, Object>
	{
	public:

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
		void Instance(const Ref<Abstract>& instance)
		{
			RegisterBinding(
				Detail::InstanceServiceBinding<Abstract>::New(instance)
			);
		}

		/**
		 * \brief 
		 * \tparam Abstract 
		 * \tparam Concrete 
		 */
		template <typename Abstract, typename Concrete =  Abstract>
		void Factory()
		{
			Factory<Abstract>([]() -> Ref<Abstract> {
				return Concrete::New();
			});
		}

		/**
		 * \brief 
		 * \tparam Abstract 
		 * \tparam Callable 
		 * \param callable 
		 */
		template <typename Abstract, typename Callable>
		void Factory(Callable&& callable)
		{
			static_assert(std::is_same_v<Ref<Abstract>, FunctionReturnType<Callable>>);

			RegisterBinding(
				Detail::FactoryServiceBinding<Abstract>::New(
					Self(),
					MakeResolver<Abstract>(std::forward<Callable>(callable))
				)
			);
			
		}

		/**
		 * \brief
		 * \tparam Abstract
		 * \tparam Concrete
		 */
		template <typename Abstract, typename Concrete = Abstract>
		void Singleton()
		{
			Singleton<Abstract>([]() -> Ref<Abstract> {
				return Concrete::New();
			});
		}

		/**
		 * \brief
		 * \tparam Abstract
		 * \tparam Callable
		 * \param callable
		 */
		template <typename Abstract, typename Callable>
		void Singleton(Callable&& callable)
		{
			static_assert(std::is_same_v<Ref<Abstract>, FunctionReturnType<Callable>>);

			RegisterBinding(
				Detail::SingletonServiceBinding<Abstract>::New(
					Self(),
					MakeResolver<Abstract>(std::forward<Callable>(callable))
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
				auto binding = Detail::ServiceBinding<Abstract>::Cast(itBinding->second);
				if (binding->IsCallable())
				{
					auto callableBinding = Detail::CallableServiceBinding<Abstract>::Cast(binding);
					callableBinding->Decorate(std::move(extender));
				}
			}
		}

		template <typename T>
		Ref<T> Resolve()
		{
			const std::type_info& abstract = typeid(T);
			const std::type_info& concrete = GetConcrete(abstract);
			if (auto itBinding = mBindings.find(concrete); itBinding != mBindings.end())
			{
				auto binding = Detail::ServiceBinding<T>::Cast(itBinding->second);
				return binding->Resolve();
			}

			return nullptr;
		}

		/**
		 * \brief Invoke a callable by resolving this parameters from the service container
		 * \param callable The callable to call
		 * \return The return value of the callable
		 */
		template <typename Callable>
		FunctionReturnType<Callable> Invoke(Callable&& callable)
		{
			auto resolver = [callback = std::forward<Callable>(callable)](const Ref<ServiceContainer>& self) -> FunctionReturnType<Callable> {
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

			return resolver(Self());
		}

	private:

		template <typename T, typename... Others>
		struct ParameterResolverHelper
		{
			static auto Resolve(const Ref<ServiceContainer>& container) -> decltype(auto)
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
			using Resolved = RemoveRefType<std::decay_t<T>>;

			static auto Resolve(const Ref<ServiceContainer>& container) -> decltype(auto)
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
			return [callback = std::forward<Callable>(callable)](const Ref<ServiceContainer>& self) -> Ref<T> {
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
		void RegisterBinding(const Ref<Detail::ServiceBindingBase>& binding);

		/**
		 * \brief 
		 * \param abstract 
		 * \return 
		 */
		const std::type_info& GetConcrete(const std::type_info& abstract) const;

		std::unordered_map<std::type_index, Ref<Detail::ServiceBindingBase>> mBindings;
		std::unordered_map<std::type_index, std::reference_wrapper<const std::type_info>> mAliases;
	};
}

#endif // COCKTAIL_CORE_APPLICATION_SERVICECONTAINER_HPP
