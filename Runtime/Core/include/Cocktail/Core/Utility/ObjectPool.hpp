#ifndef COCKTAIL_CORE_UTILITY_OBJECTPOOL_HPP
#define COCKTAIL_CORE_UTILITY_OBJECTPOOL_HPP

#include <mutex>
#include <memory>
#include <vector>

#include <Cocktail/Core/Memory/Ref.hpp>
#include <Cocktail/Core/Memory/Detail/StorageRefCounter.hpp>
#include <Cocktail/Core/System/Concurrency/NullMutex.hpp>
#include <Cocktail/Core/System/Concurrency/SpinMutex.hpp>

namespace Ck
{
	namespace Detail
	{
		template <typename, typename>
		class ObjectPoolBase;

		/**
		 * \brief 
		 * \tparam T 
		 */
		template <typename T, typename Lockable>
		class ObjectPoolRefCounter : public StorageRefCounter<T>
		{
		public:

			/**
			 * \brief 
			 * \tparam Args 
			 * \param owner 
			 * \param args 
			 */
			template <typename... Args>
			explicit ObjectPoolRefCounter(ObjectPoolBase<T, Lockable>* owner, Args&&... args) :
				StorageRefCounter<T>(std::forward<Args>(args)...),
				mOwner(owner)
			{
				/// Nothing
			}

		protected:

			void Suicide() override
			{
				mOwner->Recycle(this);
			}

		private:

			ObjectPoolBase<T, Lockable>* mOwner;
		};

		/**
		 * \brief 
		 * \tparam T 
		 * \tparam Lockable 
		 */
		template <typename T, typename Lockable>
		class ObjectPoolBase
		{
		public:

			using PoolRefCounter = ObjectPoolRefCounter<T, Lockable>;

			/**
			 * \brief Constructor
			 * \param preAllocated 
			 * \param growable
			 */
			explicit ObjectPoolBase(std::size_t preAllocated = 0, bool growable = true) :
				mGrowable(growable)
			{
				assert(preAllocated > 0 || mGrowable);

				std::lock_guard<Lockable> lg(mMutex);
				if (preAllocated)
					AllocatePage(preAllocated);
			}

			/**
			 * \brief 
			 * \param other 
			 */
			ObjectPoolBase(const ObjectPoolBase& other) = delete;

			/**
			 * \brief 
			 * \param other 
			 */
			ObjectPoolBase(ObjectPoolBase&& other) noexcept 
			{
				*this = std::move(other);
			}

			/**
			 * \brief 
			 * \param other 
			 * \return 
			 */
			ObjectPoolBase& operator=(const ObjectPoolBase& other) = delete;

			/**
			 * \brief 
			 * \param other 
			 * \return 
			 */
			ObjectPoolBase& operator=(ObjectPoolBase&& other) noexcept
			{
				mGrowable = other.mGrowable;

				std::lock_guard<Lockable> lg(mMutex);
				std::lock_guard<Lockable> rhsLg(other.mMutex);

				mPages = std::move(other.mPages);
				mVacants = std::move(other.mVacants);

				return *this;
			}

			/**
			 * \brief 
			 */
			~ObjectPoolBase()
			{
				Clear();
			}

			/**
			 * \brief 
			 * \tparam Args 
			 * \param args 
			 * \return 
			 */
			template <typename... Args>
			Ref<T> Allocate(Args&&... args)
			{
				std::lock_guard<Lockable> lg(mMutex);
				if (mVacants.empty())
				{
					assert(mGrowable);
					AllocatePage((mPages.size() + 1) * 32);
				}

				PoolRefCounter* location = mVacants.back();
				mVacants.pop_back();

				PoolRefCounter* refCounter = new (location) PoolRefCounter(this, std::forward<Args>(args)...);

				T* pointer = refCounter->GetPointer();
				Object::AssignRefCounter(pointer, refCounter);

				return Ref<T>(pointer, refCounter);
			}

			/**
			 * \brief 
			 */
			void Clear()
			{
				std::lock_guard<Lockable> lg(mMutex);
				
				mVacants.clear();
				mPages.clear();
			}

		private:

			friend class ObjectPoolRefCounter<T, Lockable>;

			/**
			 * \brief 
			 * \param objectCount 
			 */
			void AllocatePage(std::size_t objectCount)
			{
				auto page = std::make_unique<unsigned char[]>(sizeof(PoolRefCounter) * objectCount);
				PoolRefCounter* vacants = reinterpret_cast<PoolRefCounter*>(page.get());
				for (std::size_t i = 0; i < objectCount; i++)
					mVacants.emplace_back(&vacants[i]);

				mPages.emplace_back(std::move(page));
			}

			/**
			 * \brief
			 * \param object
			 */
			void Recycle(PoolRefCounter* object)
			{
				std::lock_guard<Lockable> lg(mMutex);
				mVacants.emplace_back(object);
			}

			bool mGrowable;
			Lockable mMutex;
			std::vector<PoolRefCounter*> mVacants;
			std::vector<std::unique_ptr<unsigned char[]>> mPages;
		};
	}

	template <typename T>
	using ObjectPool = Detail::ObjectPoolBase<T, std::recursive_mutex>;

	template <typename T>
	using SpinObjectPool = Detail::ObjectPoolBase<T, SpinMutex>;

	template <typename T>
	using TlsObjectPool = Detail::ObjectPoolBase<T, NullMutex>;
}

#endif // COCKTAIL_CORE_UTILITY_OBJECTPOOL_HPP

