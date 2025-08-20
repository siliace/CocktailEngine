#include <Cocktail/Core/Application/ServiceContainer.hpp>

namespace Ck
{
	void ServiceContainer::Alias(const std::type_info& abstract, const std::type_info& concrete)
	{
		if (abstract == concrete)
			return;

		mAliases.insert_or_assign(abstract, concrete);
	}

	void ServiceContainer::Clear()
	{
		mBindings.clear();
		mAliases.clear();
	}

	void ServiceContainer::RegisterBinding(std::unique_ptr<Detail::ServiceBindingBase> binding)
	{
		mBindings.insert_or_assign(binding->GetResolvedType(), std::move(binding));
	}

	const std::type_info& ServiceContainer::GetConcrete(const std::type_info& abstract) const
	{
		if (auto it = mAliases.find(abstract); it != mAliases.end())
			return GetConcrete(it->second);

		return abstract;
	}
}
