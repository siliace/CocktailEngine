#ifndef COCKTAIL_CORE_CONCURRENCY_NULLMUTEX_HPP
#define COCKTAIL_CORE_CONCURRENCY_NULLMUTEX_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API NullMutex
	{
	public:

		/**
		 * \brief 
		 */
		NullMutex() noexcept;

		/**
		 * \brief 
		 */
		~NullMutex() noexcept;

		/**
		 * \brief 
		 */
		NullMutex(NullMutex const&) = delete;

		/**
		 * \brief 
		 * \return 
		 */
		NullMutex& operator=(const NullMutex&) = delete;

		/**
		 * \brief 
		 */
		NullMutex(NullMutex&&) = delete;

		/**
		 * \brief 
		 * \return 
		 */
		NullMutex& operator=(NullMutex&&) = delete;

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
	};
}

#endif // COCKTAIL_CORE_CONCURRENCY_NULLMUTEX_HPP
