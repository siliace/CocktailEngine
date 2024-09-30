#ifndef COCKTAIL_CORE_UTILITY_CACHE_CACHEPOOL_HPP
#define COCKTAIL_CORE_UTILITY_CACHE_CACHEPOOL_HPP

#include <unordered_map>

#include <Cocktail/Core/Utility/Cache/CacheItem.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam K 
	 * \tparam V 
	 */
	template <typename K, typename V, typename Hasher = std::hash<K>>
	class CachePool
	{
	public:

		/**
		 * \brief 
		 */
		CachePool() :
			mForceDeferred(false)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param key 
		 * \return 
		 */
		V Get(const K& key) const
		{
			CacheItem<K, V> item = GetItem(key);
			return item.IsHit() ? item.Get() : nullptr;
		}

		/**
		 * \brief
		 * \tparam Callable
		 * \param key
		 * \param callable
		 * \return
		 */
		template <typename Callable>
		V GetOrCreate(const K& key, Callable&& callable)
		{
			CacheItem<K, V> item = GetItem(key);
			if (item.IsHit())
				return item.Get();

			V value = callable();
			item.Set(value);
			Save(item);

			return value;
		}

		/**
		 * \brief Get an item from the cache
		 * \param key
		 * \return
		 */
		CacheItem<K, V> GetItem(const K& key) const
		{
			if (auto it = mItems.find(key); it != mItems.end())
				return CacheItem<K, V>(it->first, it->second, true);

			return CacheItem<K, V>(key, nullptr, false);
		}

		/**
		 * \brief Remove an element from the cache
		 * \param key The key of the element to remove
		 */
		void DeleteItem(const K& key)
		{
			if (mForceDeferred)
			{
				DeleteItemDeferred(key);
			}
			else
			{
				if (auto it = mItems.find(key); it != mItems.end())
					mItems.erase(it);
			}
		}

		/**
		 * \brief Remove an element from the cache
		 * \param key The key of the element to remove
		 */
		void DeleteItemDeferred(const K& key)
		{
			if (auto it = mDeferredItems.find(key); it != mDeferredItems.end())
				mDeferredItems.erase(it);
		}

		/**
		 * \brief
		 * \param key
		 * \return
		 */
		bool HasItem(const K& key) const
		{
			return mItems.contains(key);
		}

		/**
		 * \brief Persists a CacheItem
		 * \param item The CacheItem to save
		 */
		void Save(const CacheItem<K, V>& item)
		{
			if (mForceDeferred)
			{
				SaveDeferred(item);
			}
			else
			{
				mItems.insert_or_assign(item.GetKey(), item.Get());
			}
		}

		/**
		 * \brief Sets a CacheItem to be persisted later
		 * \param item The CacheItem to save
		 * \see CachePool::Commit
		 */
		void SaveDeferred(const CacheItem<K, V>& item)
		{
			mDeferredItems.insert_or_assign(item.GetKey(), item.Get());
		}

		/**
		 * \brief Delete all CacheItem in the pool
		 */
		void Clear()
		{
			mItems.clear();
			mDeferredItems.clear();
		}

		/**
		 * \brief Forget every CacheItem deferred saved that has not been committed yet
		 */
		void Rollback()
		{
			mDeferredItems = mItems;
		}

		/**
		 * \brief Persists any deferred CacheItem
		 */
		void Commit()
		{
			mItems = mDeferredItems;
		}

		/**
		 * \brief 
		 * \tparam Callable 
		 * \param callable 
		 */
		template <typename Callable>
		void ForEach(Callable callable)
		{
			mForceDeferred = true;
			for (const auto&[key, value] : mItems)
				callable(key, value);

			mForceDeferred = false;
			Commit();
		}

		/**
		 * \brief 
		 * \tparam Callable 
		 * \param callable 
		 */
		template <typename Callable>
		void ForEach(Callable callable) const
		{
			for (const auto& [key, value] : mItems)
				callable(key, value);
		}

	private:

		bool mForceDeferred;
		std::unordered_map<K, V, Hasher> mItems;
		std::unordered_map<K, V, Hasher> mDeferredItems;
	};
}

#endif // COCKTAIL_CORE_UTILITY_CACHE_CACHEPOOL_HPP
