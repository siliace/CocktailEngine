#ifndef COCKTAILENGINE_CORE_APPLICATION_SERVICECONTAINER_HPP
#define COCKTAILENGINE_CORE_APPLICATION_SERVICECONTAINER_HPP

#include <typeindex>

#include <CocktailEngine/Core/HashMap.hpp>
#include <CocktailEngine/Core/HashSet.hpp>
#include <CocktailEngine/Core/Memory/Ptr.hpp>
#include <CocktailEngine/Core/Memory/SharedFromThis.hpp>
#include <CocktailEngine/Core/Memory/SharedPtr.hpp>
#include <CocktailEngine/Core/Memory/WeakPtr.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/Utility/FunctionTraits.hpp>

namespace Ck
{
    class ServiceContainer;

    /**
     * \brief Defines a transformation applied to an owned service instance
     *
     * A decorator receives ownership of the current instance and returns the
     * instance that subsequent decorators and consumers receive.
     *
     * \tparam T Service type transformed by the decorator
     */
    template <typename T>
    using ServiceDecoratorFactory = std::function<UniquePtr<T>(UniquePtr<T>)>;

    namespace Detail
    {
        /**
         * \brief Defines the factory used to create an owned service instance
         *
         * \tparam T Service type created by the resolver
         */
        template <typename T>
        using ServiceResolver = std::function<UniquePtr<T>(ServiceContainer*)>;

        /**
         * \brief Abstract base for bindings registered in a ServiceContainer
         *
         * A binding resolves one concrete service type and retains its supplier
         * bindings. Those strong dependency references ensure that consumers are
         * destroyed before the services they consume.
         */
        class COCKTAILENGINE_CORE_API ServiceBindingBase : public SharedFromThis<ServiceBindingBase>
        {
        public:

            /**
             * \brief Indicates whether this binding creates its service through a callable
             *
             * \return True when decorators can be applied to this binding, false for an existing instance
             */
            virtual bool IsCallable() const = 0;

            /**
             * \brief Gets the concrete service type resolved by this binding
             *
             * \return Runtime type information for the resolved service type
             */
            virtual const std::type_info& GetResolvedType() const = 0;

            /**
             * \brief Retains a supplier binding required by this binding
             *
             * \param dependency Binding that must outlive this consumer binding
             */
            void AddDependency(ServiceBindingBase* dependency);

        protected:

            /**
             * \brief Initializes dependency storage for a binding
             */
            ServiceBindingBase();

        private:

            HashSet<SharedPtr<ServiceBindingBase>> mDependencies; /*!< Supplier bindings retained by this consumer. */
        };

        /**
         * \brief Typed base for a binding that resolves services of type T
         *
         * \tparam T Concrete service type resolved by the binding
         */
        template <typename T>
        class ServiceBinding : public ServiceBindingBase
        {
        public:

            /**
             * \brief Resolves the service represented by this binding
             *
             * \return Resolved service instance, or nullptr when the binding represents a null instance
             */
            virtual T* Resolve() = 0;

            /**
             * \brief Gets the concrete type resolved by this typed binding
             *
             * \return Runtime type information for T
             */
            const std::type_info& GetResolvedType() const override
            {
                return typeid(T);
            }
        };

        /**
         * \brief Binding base for services created by a resolver callable
         *
         * Stores decorators that transform the owned service instance after the
         * resolver creates it.
         *
         * \tparam T Concrete service type created by the binding
         */
        template <typename T>
        class CallableServiceBinding : public ServiceBinding<T>
        {
        public:

            /**
             * \brief Adds a decorator applied after the service instance is created
             *
             * Decorators are applied in their registration order.
             *
             * \param extender Decorator factory to append
             */
            void Decorate(ServiceDecoratorFactory<T> extender)
            {
                mDecoratorFactories.push_back(extender);
            }

            /**
             * \brief Indicates that this binding creates its service through a callable
             *
             * \return Always true
             */
            bool IsCallable() const override
            {
                return true;
            }

        protected:

            /**
             * \brief Applies all registered decorators to an owned service instance
             *
             * \param instance Instance created by the resolver
             * \return Instance returned by the last decorator, or the original instance when none are registered
             */
            UniquePtr<T> InvokeDecorators(UniquePtr<T> instance)
            {
                for (const ServiceDecoratorFactory<T>& decoratorFactory : mDecoratorFactories)
                    instance = decoratorFactory(Move(instance));

                return instance;
            }

        private:

            Array<ServiceDecoratorFactory<T>> mDecoratorFactories; /*!< Decorators applied after resolving the service. */
        };

        /**
         * \brief Callable binding that creates and owns one service instance
         *
         * The service is either created by the constructor or deferred until the
         * first call to Resolve, depending on its lazy configuration.
         *
         * \tparam T Concrete service type owned by the binding
         */
        template <typename T>
        class SingletonServiceBinding : public CallableServiceBinding<T>
        {
        public:

            /**
             * \brief Creates a singleton binding
             *
             * \param owner Container used to resolve dependencies for the service factory
             * \param resolver Factory that creates the owned service instance
             * \param lazy When true, defers creation until the first resolution
             */
            SingletonServiceBinding(ServiceContainer* owner, ServiceResolver<T> resolver, bool lazy) :
                mOwner(owner),
                mResolver(Move(resolver)),
                mLazy(lazy)
            {
                if (!mLazy)
                    mInstance = CallableServiceBinding<T>::InvokeDecorators(mResolver(mOwner));
            }

            /**
             * \brief Resolves the singleton service instance
             *
             * Creates the instance through its resolver and decorators on the first
             * call when the binding is configured as lazy.
             *
             * \return Owned singleton service instance
             */
            T* Resolve() override
            {
                if (mLazy && !mInstance)
                {
                    LockGuard<Mutex> lg(mLock);
                    if (!mInstance)
                        mInstance = CallableServiceBinding<T>::InvokeDecorators(mResolver(mOwner));
                }

                return mInstance.Get();
            }

        private:

            ServiceContainer* mOwner; /*!< Container that owns this binding. */
            ServiceResolver<T> mResolver; /*!< Factory used to create the service. */
            bool mLazy; /*!< Whether service creation is deferred until resolution. */
            Mutex mLock; /*!< Serializes lazy instance creation. */
            UniquePtr<T> mInstance; /*!< Owned singleton instance. */
        };

        /**
         * \brief Non-owning binding for an existing service instance
         *
         * \tparam T Service type exposed by the binding
         */
        template <typename T>
        class InstanceServiceBinding : public ServiceBinding<T>
        {
        public:

            /**
             * \brief Creates a binding that exposes an existing instance
             *
             * \param instance Non-owning pointer to the service instance; may be null
             */
            explicit InstanceServiceBinding(T* instance) :
                mInstance(instance)
            {
                /// Nothing
            }

            /**
             * \brief Resolves the non-owning service instance
             *
             * \return Pointer supplied to the constructor, which may be null
             */
            T* Resolve() override
            {
                return mInstance;
            }

            /**
             * \brief Indicates that this binding does not create its service through a callable
             *
             * \return Always false
             */
            bool IsCallable() const override
            {
                return false;
            }

        private:

            T* mInstance; /*!< Non-owning service instance exposed by the binding. */
        };
    }

    /**
     * \brief Registers, resolves, and manages the lifetime of application services
     *
     * Services are identified by their C++ type and registered as instance
     * bindings or callable singleton bindings. A callable factory may declare service
     * pointer parameters; the container resolves those parameters when it invokes the
     * factory or an arbitrary callable through Invoke().
     *
     * The container also records dependencies between bindings. A consumer binding
     * retains its supplier bindings so that, during Clear(), consumers are destroyed
     * before the services on which they depend. Dependencies whose suppliers have not
     * yet been registered are tracked until their binding becomes available.
     */
    class COCKTAILENGINE_CORE_API ServiceContainer
    {
    public:

        /**
         * \brief Constructs an empty service container.
         */
        ServiceContainer() = default;

        /**
         * \brief Service containers cannot be copied.
         */
        ServiceContainer(const ServiceContainer&) = delete;

        /**
         * \brief Service containers cannot be moved.
         */
        ServiceContainer(ServiceContainer&&) noexcept = delete;

        /**
         * \brief Service containers cannot be copied.
         */
        ServiceContainer& operator=(const ServiceContainer&) = delete;

        /**
         * \brief Service containers cannot be moved.
         */
        ServiceContainer& operator=(ServiceContainer&&) noexcept = delete;

        /**
         * \brief Associates an abstract service type with a concrete implementation
         *
         * Resolving \p Abstract subsequently resolves the binding registered
         * for \p Concrete. Pending dependencies declared using \p Abstract are
         * migrated to the concrete service when the alias is registered.
         *
         * \tparam Abstract Abstract interface or base class to resolve
         * \tparam Concrete Concrete implementation of \p Abstract
         */
        template <typename Abstract, typename Concrete, typename = std::enable_if_t<std::is_base_of_v<Abstract, Concrete>>>
        void Alias()
        {
            if constexpr (!std::is_same_v<Abstract, Concrete>)
                Alias(typeid(Abstract), typeid(Concrete));
        }

        /**
         * \brief Associates an abstract service type with a concrete implementation
         *
         * This non-template overload supports runtime registration. It has
         * no effect when \p abstract and \p concrete denote the same type.
         *
         * \param abstract Type exposed to consumers
         * \param concrete Type whose binding resolves the abstract type
         */
        void Alias(const std::type_info& abstract, const std::type_info& concrete);

        /**
         * \brief Registers a non-owning binding for an existing service instance
         *
         * Resolve<Abstract>() returns \p instance until the binding is
         * replaced or the container is cleared. The caller remains responsible for
         * keeping the instance alive for the duration of its registration.
         *
         * \tparam Abstract Type exposed by the binding
         *
         * \param instance Existing instance to expose. May be null
         */
        template <typename Abstract>
        void Instance(Abstract* instance)
        {
            RegisterResolvableBinding(MakeShared<Detail::InstanceServiceBinding<Abstract>>(instance));
        }

        /**
         * \brief Registers a default-constructed singleton implementation
         *
         * The concrete service is constructed with MakeUnique<Concrete>()
         * and then exposed through an alias from \p Abstract to \p Concrete. The
         * instance is created on its first resolution unless \p lazy is false.
         *
         * \tparam Abstract Type exposed to consumers
         * \tparam Concrete Concrete implementation to construct
         *
         * \param lazy When true, defers construction until the first resolution
         */
        template <typename Abstract, typename Concrete = Abstract>
        void Singleton(bool lazy = true)
        {
            Singleton<Concrete>(
                []() -> UniquePtr<Concrete> {
                    return MakeUnique<Concrete>();
                },
                lazy);

            Alias<Abstract, Concrete>();
        }

        /**
         * \brief Registers a singleton created by a dependency-injected factory
         *
         * \p callable must return UniquePtr<Abstract>. Each parameter of
         * the callable is treated as a service pointer and resolved from this
         * container when the singleton is created. The binding retains the bindings
         * of its declared dependencies to preserve their destruction order.
         *
         * \tparam Abstract Type exposed by the singleton binding
         * \tparam Callable Factory type returning UniquePtr<Abstract>
         *
         * \param callable Factory used to create the singleton instance
         * \param lazy When true, defers construction until the first resolution
         */
        template <typename Abstract, typename Callable>
        void Singleton(Callable&& callable, bool lazy = true)
        {
            static_assert(std::is_same_v<UniquePtr<Abstract>, FunctionReturnType<Callable>>);

            using BindingType = Detail::SingletonServiceBinding<Abstract>;

            SharedPtr<BindingType> binding = MakeShared<BindingType>(this, MakeResolver<Abstract>(Forward<Callable>(callable)), lazy);

            using BindingDependencyHelperType = typename FunctionTraits<Callable>::Arguments::template Apply<BindingDependencyHelper>;
            BindingDependencyHelperType::Setup(this, binding.Get());

            RegisterResolvableBinding(Move(binding));
        }

        /**
         * \brief Adds a decorator to an existing callable binding
         *
         * The decorator is applied when the binding creates its service
         * instance. The call is ignored when no binding exists for \p Abstract or
         * when that binding is not callable.
         *
         * \tparam Abstract Type exposed by the binding to decorate
         *
         * \param extender Decorator factory to append to the binding
         */
        template <typename Abstract>
        void Decorate(ServiceDecoratorFactory<Abstract> extender)
        {
            const std::type_info& abstract = typeid(Abstract);
            const std::type_info& concrete = GetConcrete(abstract);
            mResolvingTable.TryGet(concrete).Then([&](Ptr<Detail::ServiceBindingBase> binding) {
                if (binding->IsCallable())
                {
                    auto callableBinding = static_cast<Detail::CallableServiceBinding<Abstract>*>(binding.Get());
                    callableBinding->Decorate(Move(extender));
                }
            });
        }

        /**
         * \brief Resolves the service registered for a type
         *
         * Aliases are followed before locating the binding. Resolving a
         * lazy singleton creates its instance on the first call.
         *
         * \tparam T Service type to resolve
         *
         * \return The resolved service instance, or nullptr when no binding exists
         */
        template <typename T>
        T* Resolve()
        {
            const std::type_info& abstract = typeid(T);
            const std::type_info& concrete = GetConcrete(abstract);
            return mResolvingTable.TryGet(concrete)
                .Map([](Ptr<Detail::ServiceBindingBase> binding) {
                    return static_cast<Detail::ServiceBinding<T>*>(binding.Get())->Resolve();
                })
                .GetOr(nullptr);
        }

        /**
         * \brief Invokes a callable after resolving its service pointer parameters
         *
         * This applies the same parameter-resolution rules as singleton
         * factories without registering a binding. A callable without parameters is
         * invoked directly.
         *
         * \tparam Callable Callable type to invoke
         * \param callable Callable receiving zero or more service pointers
         *
         * \return The value returned by \p callable
         */
        template <typename Callable>
        FunctionReturnType<Callable> Invoke(Callable&& callable)
        {
            auto resolver = [callback = Forward<Callable>(callable)](ServiceContainer* self) -> FunctionReturnType<Callable> {
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
         * \brief Removes all aliases, bindings, and pending dependency records
         *
         * Derived application types call this during teardown. Strong
         * dependency references ensure that suppliers survive until their consumer
         * bindings have been destroyed.
         */
        void Clear();

    private:

        /**
         * \brief Resolves a pack of factory parameter types into service pointers
         *
         * \tparam T First parameter type
         * \tparam Others Remaining parameter types
         */
        template <typename T, typename... Others>
        struct ParameterResolverHelper
        {
            static auto Resolve(ServiceContainer* container) -> decltype(auto)
            {
                return std::tuple_cat(ParameterResolverHelper<T>::Resolve(container), ParameterResolverHelper<Others...>::Resolve(container));
            }
        };

        /**
         * \brief Resolves one factory parameter into a single-element tuple
         *
         * \tparam T Factory parameter type; its pointer target identifies the
         *            service type to resolve
         */
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
         * \brief Registers all service types referenced by a factory signature
         *
         * \tparam Ts Factory parameter types whose pointer targets are dependency
         *             service types
         */
        template <typename... Ts>
        struct BindingDependencyHelper
        {
            static void Setup(ServiceContainer* container, Detail::ServiceBindingBase* binding)
            {
                (container->SetupDependency(binding, typeid(std::remove_pointer_t<std::decay_t<Ts>>)), ...);
            }
        };

        /**
         * \brief Wraps a callable in the resolver used by a callable binding
         *
         * The returned resolver resolves the callable parameters from its
         * owning container before invoking the original factory.
         *
         * \tparam T Service type produced by the factory
         *
         * \tparam Callable Factory type returning UniquePtr<T>
         * \param callable Factory to wrap
         *
         * \return Resolver compatible with a callable service binding
         */
        template <typename T, typename Callable>
        static Detail::ServiceResolver<T> MakeResolver(Callable&& callable)
        {
            return [callback = Forward<Callable>(callable)](ServiceContainer* self) -> UniquePtr<T> {
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
         * \brief Connects a binding to one of its declared dependencies
         *
         * The dependency type is first canonicalized through aliases. If
         * its binding is available, the consumer records a strong dependency
         * reference immediately. Otherwise, the consumer is stored as a weak
         * pending dependency until the supplier is registered.
         *
         * \param binding Consumer binding that declares the dependency
         * \param dependency Declared service type from the factory signature
         */
        void SetupDependency(Detail::ServiceBindingBase* binding, const std::type_info& dependency);

        /**
         * \brief Registers a binding that can resolve a concrete service type
         *
         * Any live consumers waiting for this service receive a strong
         * reference to \p binding before it is stored in the resolving table.
         *
         * \param binding Binding to register. Ownership is transferred to the
         *                resolving table
         */
        void RegisterResolvableBinding(SharedPtr<Detail::ServiceBindingBase> binding);

        /**
         * \brief Returns the concrete service type associated with an exposed type
         *
         * Aliases are followed recursively until an unaliased concrete
         * type is reached.
         *
         * \param abstract Type requested by a consumer
         *
         * \return The concrete type used as the binding-table key
         */
        const std::type_info& GetConcrete(const std::type_info& abstract) const;

        HashMap<std::type_index, SharedPtr<Detail::ServiceBindingBase>> mResolvingTable; /*!< Owns the resolvable binding registered for each concrete service type */
        HashMap<std::type_index, std::reference_wrapper<const std::type_info>> mAliases; /*!< Maps exposed abstract types to their concrete implementation types */
        HashMap<std::type_index, Array<WeakPtr<Detail::ServiceBindingBase>>> mPendingDependencies; /*!< Stores consumers waiting for each not-yet-registered dependency */
    };
}

#endif // COCKTAILENGINE_CORE_APPLICATION_SERVICECONTAINER_HPP
