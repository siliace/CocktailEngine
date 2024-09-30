#ifndef COCKTAIL_CORE_APPLICATION_DETAIL_SINGLETONSERVICEBINDING_HPP
#define COCKTAIL_CORE_APPLICATION_DETAIL_SINGLETONSERVICEBINDING_HPP

#include <mutex>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Application/Detail/CallableServiceBinding.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

namespace Ck::Detail
{
	template <typename T>
	class SingletonServiceBinding : public Inherit<SingletonServiceBinding<T>, Object, CallableServiceBinding<T>>
	{
	public:

		/**
		 * \brief 
		 * \param owner 
		 * \param resolver 
		 */
		SingletonServiceBinding(const Ref<ServiceContainer>& owner, ServiceResolver<T> resolver) :
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
			if (!mInstance)
			{
				std::lock_guard lg(mLock);
				if (!mInstance)
					mInstance = InvokeDecorators(mResolver(mOwner));
			}

			return mInstance;
		}

	private:

		Ref<ServiceContainer> mOwner;
		ServiceResolver<T> mResolver;
		std::mutex mLock;
		Ref<T> mInstance;
	};
}

#endif // COCKTAIL_CORE_APPLICATION_DETAIL_SINGLETONSERVICEBINDING_HPP
