#ifndef COCKTAIL_CORE_APPLICATION_DETAIL_FACTORYSERVICEBINDING_HPP
#define COCKTAIL_CORE_APPLICATION_DETAIL_FACTORYSERVICEBINDING_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Application/Detail/CallableServiceBinding.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

namespace Ck::Detail
{
	template <typename T>
	class FactoryServiceBinding : public Inherit<FactoryServiceBinding<T>, Object, CallableServiceBinding<T>>
	{
	public:

		/**
		 * \brief
		 * \param owner
		 * \param resolver
		 */
		FactoryServiceBinding(const Ref<ServiceContainer>& owner, ServiceResolver<T> resolver) :
			mOwner(owner),
			mResolver(std::move(resolver))
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \return
		 */
		Ref<T> Resolve() override
		{
			return Inherit<FactoryServiceBinding<T>, Object, CallableServiceBinding<T>>::InvokeDecorators(mResolver(mOwner));
		}

	private:

		Ref<ServiceContainer> mOwner;
		ServiceResolver<T> mResolver;
	};
}

#endif // COCKTAIL_CORE_APPLICATION_DETAIL_FACTORYSERVICEBINDING_HPP
