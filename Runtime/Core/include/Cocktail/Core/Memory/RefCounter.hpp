#ifndef COCKTAIL_CORE_MEMORY_REFCOUNTER_HPP
#define COCKTAIL_CORE_MEMORY_REFCOUNTER_HPP

#include <atomic>
#include <utility>

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API RefCounter
	{
	public:

		/**
		 * \brief 
		 */
		RefCounter();

		/**
		 * \brief 
		 * \param strongCounter 
		 * \param weakCounter 
		 */
		RefCounter(Uint64 strongCounter, Uint64 weakCounter);

		/**
		 * \brief 
		 */
		virtual ~RefCounter() = default;

		/**
		 * \brief 
		 */
		void Acquire();

		/**
		 * \brief 
		 */
		void WeakAcquire();

		/**
		 * \brief 
		 */
		void Release();

		/**
		 * \brief 
		 */
		void WeakRelease();

		/**
		 * \brief 
		 * \return 
		 */
		Uint64 GetCounter() const;

		/**
		 * \brief 
		 * \return 
		 */
		Uint64 GetWeakCounter() const;

	protected:

		/**
		 * \brief 
		 */
		virtual void Destroy() = 0;

		/**
		 * \brief 
		 */
		virtual void Suicide() = 0;

	private:

		std::atomic<Uint64> mStrongCounter;
		std::atomic<Uint64> mWeakCounter;
	};
}

#endif // COCKTAIL_CORE_MEMORY_REFCOUNTER_HPP
