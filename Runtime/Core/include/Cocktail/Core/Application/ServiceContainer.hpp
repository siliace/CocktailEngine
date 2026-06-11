#ifndef COCKTAIL_CORE_APPLICATION_SERVICECONTAINER_HPP
#define COCKTAIL_CORE_APPLICATION_SERVICECONTAINER_HPP

#include <typeindex>

#include <Cocktail/Core/Application/Detail/InstanceServiceBinding.hpp>
#include <Cocktail/Core/Application/Detail/SingletonServiceBinding.hpp>
#include <Cocktail/Core/Application/Detail/ThreadLocalSingletonServiceBinding.hpp>
#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/HashMap.hpp>
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
		template <typename Abstract, typename Concrete, typename = std::enable_if_t<std::is_base_of_v<Abstract, Concrete>>>
		void Alias()
		{
		    if constexpr (!std::is_same_v<Abstract, Concrete>)
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
				MakeUnique<Detail::InstanceServiceBinding<Abstract>>(instance)
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
			Singleton<Concrete>([]() -> UniquePtr<Concrete> {
				return MakeUnique<Concrete>();
			}, lazy);

		    Alias<Abstract, Concrete>();
		}

	    /**
         * \brief
         * \tparam Abstract
         * \tparam Concrete
         */
	    template <typename Abstract, typename Concrete = Abstract>
        void ThreadLocalSingleton()
		{
		    ThreadLocalSingleton<Concrete>([]() -> UniquePtr<Concrete> {
                return MakeUnique<Concrete>();
            });

		    Alias<Abstract, Concrete>();
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
			static_assert(std::is_same_v<UniquePtr<Abstract>, FunctionReturnType<Callable>>);

			RegisterBinding(
				MakeUnique<Detail::SingletonServiceBinding<Abstract>>(
					this,
					MakeResolver<Abstract>(std::forward<Callable>(callable)),
					lazy
				)
			);
		}

	    /**
         * \brief
         * \tparam Abstract
         * \tparam Callable
         * \param callable
         */
	    template <typename Abstract, typename Callable>
        void ThreadLocalSingleton(Callable&& callable)
		{
		    static_assert(std::is_same_v<UniquePtr<Abstract>, FunctionReturnType<Callable>>);

		    RegisterBinding(
                MakeUnique<Detail::ThreadLocalSingletonServiceBinding<Abstract>>(
                    this,
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
		    mBindings.TryGet(concrete).Then([&](const UniquePtr<Detail::ServiceBindingBase>& binding) {
                if (binding->IsCallable())
                {
                    auto callableBinding = static_cast<Detail::CallableServiceBinding<Abstract>*>(binding);
                    callableBinding->Decorate(std::move(extender));
                }
		    });
		}

		template <typename T>
		T* Resolve()
		{
			const std::type_info& abstract = typeid(T);
			const std::type_info& concrete = GetConcrete(abstract);
		    return mBindings.TryGet(concrete).Map([](const UniquePtr<Detail::ServiceBindingBase>& binding) {
		        return static_cast<Detail::ServiceBinding<T>*>(binding.Get())->Resolve();
		    }).GetOr(nullptr);
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
			return [callback = std::forward<Callable>(callable)](ServiceContainer* self) -> UniquePtr<T> {
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
		void RegisterBinding(UniquePtr<Detail::ServiceBindingBase>&& binding);

		/**
		 * \brief 
		 * \param abstract 
		 * \return 
		 */
		const std::type_info& GetConcrete(const std::type_info& abstract) const;

		HashMap<std::type_index, UniquePtr<Detail::ServiceBindingBase>> mBindings;
		HashMap<std::type_index, std::reference_wrapper<const std::type_info>> mAliases;
	};
}

#endif // COCKTAIL_CORE_APPLICATION_SERVICECONTAINER_HPP
