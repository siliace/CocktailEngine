#include <CocktailEngine/Core/Application/ServiceContainer.hpp>

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
}
