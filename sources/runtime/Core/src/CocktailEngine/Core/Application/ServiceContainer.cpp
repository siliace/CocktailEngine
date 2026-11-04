#include <CocktailEngine/Core/Application/ServiceContainer.hpp>

#include <utility>

#include "CocktailEngine/Core/Memory/Allocator/SizedLinearAllocator.hpp"

namespace Ck
{
    namespace Detail
    {
        void ServiceBindingBase::AddDependency(ServiceBindingBase* dependency)
        {
            assert(dependency != nullptr);

            if (dependency != this)
                mDependencies.Add(dependency->AsShared());
        }

        ServiceBindingBase::ServiceBindingBase()
        {
            mDependencies.Reserve(2);
        }
    }

    void ServiceContainer::Alias(const std::type_info& abstract, const std::type_info& concrete)
    {
        const TypeKey abstractType(abstract);
        const TypeKey concreteType(concrete);

        if (abstractType == concreteType)
            return;

        if (const std::reference_wrapper<const std::type_info>* existing = mAliases.Get(abstractType))
        {
            if (existing->get() == concrete)
                return;

            throw LogicException(CK_TEXT("A service alias cannot be retargeted"));
        }

        for (TypeKey currentType = concreteType;;)
        {
            if (currentType == abstractType)
                throw LogicException(CK_TEXT("A service alias cannot form a cycle"));

            const std::reference_wrapper<const std::type_info>* next = mAliases.Get(currentType);
            if (!next)
                break;

            currentType = TypeKey(next->get());
        }

        mAliases.Put(abstractType, std::cref(concrete));

        if (const Array<WeakPtr<Detail::ServiceBindingBase>>* pendingDependencies = mPendingDependencies.Get(abstractType))
        {
            Array<WeakPtr<Detail::ServiceBindingBase>> waitingBindings(*pendingDependencies);
            mPendingDependencies.Remove(abstractType);

            waitingBindings.ForEach([&](const WeakPtr<Detail::ServiceBindingBase>& pendingBinding) {
                if (SharedPtr<Detail::ServiceBindingBase> resolvedPendingBinding = pendingBinding.ToStrong())
                    SetupDependency(resolvedPendingBinding.Get(), concrete);
            });
        }
    }

    void ServiceContainer::Clear()
    {
        mAliases.Clear();
        mResolvingTable.Clear();
        mPendingDependencies.Clear();
        mUpcasts.Clear();
    }

    void ServiceContainer::SetupDependency(Detail::ServiceBindingBase* binding, const std::type_info& dependency)
    {
        const TypeKey dependencyType(GetConcrete(dependency));
        if (BindingRecord* dependencyBinding = mResolvingTable.Get(dependencyType))
        {
            binding->AddDependency(dependencyBinding->Binding.Get());
            return;
        }

        mPendingDependencies[dependencyType].Add(binding->AsWeak());
    }

    void ServiceContainer::RegisterResolvableBinding(SharedPtr<Binding> binding, BindingResolver resolver)
    {
        const TypeKey resolvedType(binding->GetResolvedType());

        if (const Array<WeakPtr<Detail::ServiceBindingBase>>* pendingBindings = mPendingDependencies.Get(resolvedType))
        {
            Array<WeakPtr<Detail::ServiceBindingBase>> waitingBindings(*pendingBindings);
            mPendingDependencies.Remove(resolvedType);

            waitingBindings.ForEach([&](const WeakPtr<Detail::ServiceBindingBase>& pendingBinding) {
                if (SharedPtr<Detail::ServiceBindingBase> resolvedPendingBinding = pendingBinding.ToStrong())
                    resolvedPendingBinding->AddDependency(binding.Get());
            });
        }

        mResolvingTable.Put(resolvedType, BindingRecord{ Move(binding), resolver });
    }

    const std::type_info& ServiceContainer::GetConcrete(const std::type_info& abstract) const
    {
        const std::type_info* concrete = &abstract;
        while (const std::reference_wrapper<const std::type_info>* alias = mAliases.Get(TypeKey(*concrete)))
            concrete = &alias->get();

        return *concrete;
    }

    void* ServiceContainer::Upcast(void* instance, const std::type_info& concrete, const std::type_info& requested) const
    {
        if (!instance || requested == concrete)
            return instance;

        // Collect the alias chain from the requested type down to the concrete type.
        // Each registered thunk converts a concrete-side pointer to one of its direct
        // bases, so the chain must be applied starting from the concrete pointer and
        // moving up towards the requested base. We therefore record the hops first and
        // replay them in reverse. Composing the per-hop adjustments reproduces the full
        // offset a direct static_cast between the statically-known types would apply.
        Array<std::pair<std::type_index, std::type_index>, LinearAllocator<16>> hops; // (abstractSide, concreteSide)
        std::type_index current(requested);
        while (current != std::type_index(concrete))
        {
            const std::reference_wrapper<const std::type_info>* next = mAliases.Get(current);
            if (!next)
                return instance; // No path recorded; return the unadjusted pointer.

            const std::type_index nextKey(next->get());
            hops.Add({ current, nextKey });
            current = nextKey;
        }

        for (unsigned int i = hops.GetSize(); i > 0; --i)
        {
            const std::pair<std::type_index, std::type_index>& hop = hops.At(i - 1);
            const std::type_index& abstractSide = hop.first;
            const std::type_index& concreteSide = hop.second;

            if (const UpcastThunk* thunk = mUpcasts.Get(UpcastKey{ concreteSide, abstractSide }))
                instance = (*thunk)(instance);
        }

        return instance;
    }
}
