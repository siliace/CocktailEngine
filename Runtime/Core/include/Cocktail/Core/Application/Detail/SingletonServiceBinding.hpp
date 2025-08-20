#ifndef COCKTAIL_CORE_APPLICATION_DETAIL_SINGLETONSERVICEBINDING_HPP
#define COCKTAIL_CORE_APPLICATION_DETAIL_SINGLETONSERVICEBINDING_HPP

#include <mutex>

#include <Cocktail/Core/Application/Detail/CallableServiceBinding.hpp>

namespace Ck::Detail
{
	template <typename T>
	class SingletonServiceBinding : public CallableServiceBinding<T>
	{
	public:

		/**
		 * \brief 
		 * \param owner 
		 * \param resolver 
		 */
		SingletonServiceBinding(ServiceContainer* owner, ServiceResolver<T> resolver, bool lazy) :
			mOwner(owner),
			mResolver(std::move(resolver)),
			mLazy(lazy)
		{
			if (!mLazy)
				mInstance = CallableServiceBinding<T>::InvokeDecorators(mResolver(mOwner));
		}

		/**
		 * \brief 
		 * \return 
		 */
		T* Resolve() override
		{
			if (mLazy && !mInstance)
			{
				std::lock_guard lg(mLock);
				if (!mInstance)
					mInstance = CallableServiceBinding<T>::InvokeDecorators(mResolver(mOwner));
			}

			return mInstance.get();
		}

	private:

		ServiceContainer* mOwner;
		ServiceResolver<T> mResolver;
		bool mLazy;
		std::mutex mLock;
		std::unique_ptr<T> mInstance;
	};
}

#endif // COCKTAIL_CORE_APPLICATION_DETAIL_SINGLETONSERVICEBINDING_HPP
