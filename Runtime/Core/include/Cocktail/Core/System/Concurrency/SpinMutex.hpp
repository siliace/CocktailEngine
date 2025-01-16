#ifndef COCKTAIL_CORE_CONCURRENCY_SPINMUTEX_HPP
#define COCKTAIL_CORE_CONCURRENCY_SPINMUTEX_HPP

#include <atomic>

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API SpinMutex
	{
	public:

		/**
		 * \brief 
		 */
		SpinMutex() noexcept;

		/**
		 * \brief 
		 */
		~SpinMutex() noexcept;

		/**
		 * \brief 
		 */
		SpinMutex(SpinMutex const&) = delete;

		/**
		 * \brief 
		 * \return 
		 */
		SpinMutex& operator=(const SpinMutex&) = delete;

		/**
		 * \brief 
		 */
		SpinMutex(SpinMutex&&) = delete;

		/**
		 * \brief 
		 */
		SpinMutex& operator=(SpinMutex&&) = delete;

		/**
		 * \brief 
		 */
		void lock() noexcept;

		/**
		 * \brief 
		 * \return 
		 */
		bool try_lock() noexcept;

		/**
		 * \brief 
		 */
		void unlock() noexcept;

	private:

		std::atomic<bool> mState;
	};
}

#endif // COCKTAIL_CORE_CONCURRENCY_SPINMUTEX_HPP
