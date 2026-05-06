#include <Cocktail/Core/Application/ServiceContainer.hpp>

namespace Ck
{
	void ServiceContainer::Alias(const std::type_info& abstract, const std::type_info& concrete)
	{
		if (abstract == concrete)
			return;

		mAliases.Put(abstract, concrete);
	}

	void ServiceContainer::Clear()
	{
		mBindings.Clear();
		mAliases.Clear();
	}

	void ServiceContainer::RegisterBinding(UniquePtr<Detail::ServiceBindingBase>&& binding)
	{
		mBindings.Put(binding->GetResolvedType(), std::move(binding));
	}

	const std::type_info& ServiceContainer::GetConcrete(const std::type_info& abstract) const
	{
		if (Optional<const std::reference_wrapper<const std::type_info>&> concrete = mAliases.TryGet(abstract); !concrete.IsEmpty())
			return GetConcrete(concrete.Get());

		return abstract;  // NOLINT(bugprone-return-const-ref-from-parameter)
	}
}
