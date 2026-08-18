#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/System/Concurrency/ConditionVariable.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/Utility/Optional.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class BlockingQueue
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		T Take()
		{
			mNotifier.WaitFor(mLock, [&] {
				return !mQueue.empty();
			});

			return mQueue.PopFirst();
		}

		/**
		 * \brief
		 * \param timeout 
		 * \return 
		 */
		Optional<T> Take(const Duration& timeout)
		{
			bool completed = mNotifier.WaitFor(mLock, timeout, [&] {
				return !mQueue.empty();
			});

			if (!completed)
				return Optional<T>::Empty();

		    T value = mQueue.PopFirst();
			return Optional<T>::Of(Move(value));
		}

		/**
		 * \brief
		 * \return
		 */
		Optional<T> TryTake()
		{
			LockGuard lock(mLock);
			if (mQueue.IsEmpty())
				return Optional<T>::Empty();

			T value = mQueue.PopFirst();
		    return Optional<T>::Of(Move(value));
		}

		/**
		 * \brief 
		 * \return 
		 */
		bool IsEmpty() const
		{
			LockGuard lock(mLock);
			return mQueue.IsEmpty();
		}

		/**
		 * \brief 
		 * \param value 
		 */
		void Push(const T& value)
		{
			{
				LockGuard lock(mLock);
				mQueue.push(value);
			}
			mNotifier.NotifyOne();
		}

		/**
		 * \brief
		 * \param value
		 */
		void Push(T&& value)
		{
			{
				LockGuard lock(mLock);
				mQueue.push(Forward<T>(value));
			}
			mNotifier.NotifyOne();
		}

		/**
		 * \brief 
		 * \tparam Args 
		 * \param args 
		 */
		template <typename... Args>
		void Emplace(Args&&... args)
		{
			{
				LockGuard lock(mLock);
				mQueue.Emplace(Forward<Args>(args)...);
			}
			mNotifier.NotifyOne();
		}

	private:

		Mutex mLock;
		ConditionVariable mNotifier;
		Array<T> mQueue;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP
