#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP

#include <condition_variable>
#include <queue>

#include <Cocktail/Core/Utility/Optional.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

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
			std::unique_lock lock(mLock);
			mNotifier.wait(lock, [&] {
				return !mQueue.empty();
			});

			T value = std::move(mQueue.front());
			mQueue.pop();

			return value;
		}

		/**
		 * \brief 
		 * \tparam Rep 
		 * \tparam Period 
		 * \param timeout 
		 * \return 
		 */
		Optional<T> Take(const Duration& timeout)
		{
			std::unique_lock lock(mLock);
			std::cv_status status = mNotifier.wait_for(lock, timeout.As(TimeUnit::Milliseconds()).GetCount(), [&] {
				return !mQueue.empty();
			});

			if (status == std::cv_status::timeout)
				return Optional<T>::Empty();

			T value = std::move(mQueue.front());
			mQueue.pop();

			return Optional<T>::Of(std::move(value));
		}

		/**
		 * \brief
		 * \return
		 */
		Optional<T> TryTake()
		{
			std::unique_lock lock(mLock);
			if (mQueue.empty())
				return Optional<T>::Empty();

			T value = std::move(mQueue.front());
			mQueue.pop();

			return Optional<T>::Of(std::move(value));
		}

		/**
		 * \brief 
		 * \return 
		 */
		bool IsEmpty() const
		{
			std::unique_lock lock(mLock);
			return mQueue.empty();
		}

		/**
		 * \brief 
		 * \param value 
		 */
		void Push(const T& value)
		{
			{
				std::unique_lock lock(mLock);
				mQueue.push(value);
			}
			mNotifier.notify_one();
		}

		/**
		 * \brief
		 * \param value
		 */
		void Push(T&& value)
		{
			{
				std::unique_lock lock(mLock);
				mQueue.push(std::forward<T>(value));
			}
			mNotifier.notify_one();
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
				std::unique_lock lock(mLock);
				mQueue.emplace(std::forward<Args>(args)...);
			}
			mNotifier.notify_one();
		}

	private:

		mutable std::mutex mLock;
		std::condition_variable mNotifier;
		std::queue<T> mQueue;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP
