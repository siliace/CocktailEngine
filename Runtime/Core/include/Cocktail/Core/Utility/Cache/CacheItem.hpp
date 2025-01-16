#ifndef COCKTAIL_CORE_UTILITY_CACHE_CACHEITEM_HPP
#define COCKTAIL_CORE_UTILITY_CACHE_CACHEITEM_HPP

namespace Ck
{
	/**
	 * \brief 
	 * \tparam K 
	 * \tparam V 
	 */
	template <typename K, typename V>
	class CacheItem 
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		const K& GetKey() const
		{
			return mKey;
		}

		/**
		 * \brief 
		 * \return 
		 */
		const V& Get() const
		{
			return mValue;
		}

		/**
		 * \brief 
		 * \param value 
		 */
		void Set(const V& value)
		{
			mValue = value;
		}

		/**
		 * \brief 
		 * \return 
		 */
		bool IsHit() const
		{
			return mHit;
		}

	private:

		template <typename, typename, typename>
		friend class CachePool;

		/**
		 * \brief 
		 * \param key 
		 * \param value 
		 * \param hit 
		 */
		CacheItem(const K& key, const V& value, bool hit) :
			mKey(key),
			mValue(value),
			mHit(hit)
		{
			/// Nothing
		}

		K mKey;
		V mValue;
		bool mHit;
	};
}

#endif // COCKTAIL_CORE_UTILITY_CACHE_CACHEITEM_HPP
