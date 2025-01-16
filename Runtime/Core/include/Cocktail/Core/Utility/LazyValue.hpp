#ifndef COCKTAIL_CORE_UTILITY_LAZYVALUE_HPP
#define COCKTAIL_CORE_UTILITY_LAZYVALUE_HPP

#include <functional>

#include <Cocktail/Core/Utility/Optional.hpp>

namespace Ck
{
	/**
	 * \brief Utility wrapper for a value that should be recomputed only when we really need it
	 * \tparam T 
	 */
	template <typename T>
	class LazyValue
	{
	public:

		/**
		 * \brief Default constructor
		 */
		LazyValue() :
			mDirty(true)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \param initialValue
		 */
		explicit LazyValue(const T& initialValue) :
			mDirty(false),
			mValue(initialValue)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \param dirty
		 */
		void SetDirty(bool dirty = true)
		{
			mDirty = dirty;
		}

		/**
		 * \brief
		 * \return
		 */
		const T& Get(const std::function<T()>& updater) const
		{
			if (mDirty || mValue.IsEmpty())
			{
				mValue = Optional<T>::Of(updater());
				mDirty = false;
			}

			return mValue.Get();
		}

	private:

		mutable bool mDirty;
		mutable Optional<T> mValue;
	};

}
#endif // COCKTAIL_CORE_UTILITY_LAZYVALUE_HPP
