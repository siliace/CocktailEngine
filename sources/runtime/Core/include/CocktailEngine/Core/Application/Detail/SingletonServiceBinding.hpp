#ifndef COCKTAILENGINE_CORE_APPLICATION_DETAIL_SINGLETONSERVICEBINDING_HPP
#define COCKTAILENGINE_CORE_APPLICATION_DETAIL_SINGLETONSERVICEBINDING_HPP

#include <mutex>

#include <CocktailEngine/Core/Application/Detail/CallableServiceBinding.hpp>

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
			mResolver(Move(resolver)),
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

			return mInstance.Get();
		}

	private:

		ServiceContainer* mOwner;
		ServiceResolver<T> mResolver;
		bool mLazy;
		std::mutex mLock;
		UniquePtr<T> mInstance;
	};
}

#endif // COCKTAILENGINE_CORE_APPLICATION_DETAIL_SINGLETONSERVICEBINDING_HPP
