#include <CocktailEngine/Core/Application/ServiceContainer.hpp>

#include <utility>

#include "CocktailEngine/Core/Memory/Allocator/SizedLinearAllocator.hpp"

namespace Ck
{
    namespace Detail
    {
        void ServiceBindingBase::AddDependency(ServiceBindingBase* dependency)
        {
            mDependencies.Add(dependency->AsShared());
        }

        ServiceBindingBase::ServiceBindingBase()
        {
            mDependencies.Reserve(2);
        }
    }

    void ServiceContainer::Alias(const std::type_info& abstract, const std::type_info& concrete)
    {
        if (abstract == concrete)
            return;

        mAliases.Put(abstract, concrete);

        mPendingDependencies.TryGet(abstract).Then([&](const Array<WeakPtr<Detail::ServiceBindingBase>>& pendingDependencies) {
            pendingDependencies.ForEach([&](const WeakPtr<Detail::ServiceBindingBase>& pendingBinding) {
                if (SharedPtr<Detail::ServiceBindingBase> resolvedPendingBinding = pendingBinding.ToStrong())
                    SetupDependency(resolvedPendingBinding.Get(), concrete);
            });
        });

        mPendingDependencies.Remove(abstract);
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
        const std::type_info& concrete = GetConcrete(dependency);

        mResolvingTable.TryGet(concrete).Then([&](Ptr<Detail::ServiceBindingBase> dependencyBinding) {
            binding->AddDependency(dependencyBinding.Get());
        }).Or([&]() {
            mPendingDependencies[concrete].Add(binding->AsWeak());
        });
    }

    void ServiceContainer::RegisterResolvableBinding(SharedPtr<Detail::ServiceBindingBase> binding)
    {
        const std::type_index resolvedType(binding->GetResolvedType());

        mPendingDependencies.TryGet(resolvedType).Then([&](const Array<WeakPtr<Detail::ServiceBindingBase>>& pendingBindings) {
            pendingBindings.ForEach([&](const WeakPtr<Detail::ServiceBindingBase>& pendingBinding) {
                if (SharedPtr<Detail::ServiceBindingBase> resolvedPendingBinding = pendingBinding.ToStrong())
                    resolvedPendingBinding->AddDependency(binding.Get());
            });
            mPendingDependencies.Remove(resolvedType);
        });

        mResolvingTable.Put(resolvedType, Move(binding));
    }

    const std::type_info& ServiceContainer::GetConcrete(const std::type_info& abstract) const
    {
        const std::type_index abstractKey(abstract);

        if (Optional<const std::reference_wrapper<const std::type_info>&> concrete = mAliases.TryGet(abstractKey); !concrete.IsEmpty())
            return GetConcrete(concrete.Get());

        return abstract; // NOLINT(bugprone-return-const-ref-from-parameter)
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

            // mUpcasts is keyed by concrete type, then by target base type.
            if (const HashMap<std::type_index, UpcastThunk>* thunks = mUpcasts.Get(concreteSide))
            {
                if (const UpcastThunk* thunk = thunks->Get(abstractSide))
                    instance = (*thunk)(instance);
            }
        }

        return instance;
    }
}
