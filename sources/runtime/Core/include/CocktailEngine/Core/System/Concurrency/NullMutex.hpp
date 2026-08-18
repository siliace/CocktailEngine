#ifndef COCKTAILENGINE_CORE_CONCURRENCY_NULLMUTEX_HPP
#define COCKTAILENGINE_CORE_CONCURRENCY_NULLMUTEX_HPP

#include <CocktailEngine/Core/Export.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_CORE_API NullMutex
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

#endif // COCKTAILENGINE_CORE_CONCURRENCY_NULLMUTEX_HPP
