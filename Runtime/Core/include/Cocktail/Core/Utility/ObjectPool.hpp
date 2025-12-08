#ifndef COCKTAIL_CORE_UTILITY_OBJECTPOOL_HPP
#define COCKTAIL_CORE_UTILITY_OBJECTPOOL_HPP

#include <memory>
#include <mutex>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>
#include <Cocktail/Core/System/Concurrency/NullMutex.hpp>
#include <Cocktail/Core/System/Concurrency/SpinMutex.hpp>

namespace Ck
{
	namespace Detail
	{
		/**
		 * \brief 
		 * \tparam T 
		 * \tparam Lockable 
		 */
		template <typename T, typename Lockable>
		class ObjectPoolBase
		{
		public:

			struct Deleter
			{
				void operator()(T* object) const noexcept
				{
					Pool->Recycle(object);
				}

				ObjectPoolBase* Pool;
			};

			using Shared = std::shared_ptr<T>;
			using Unique = UniquePtr<T, Deleter>;

			/**
			 * \brief Constructor
			 * \param preAllocated 
			 * \param growable
			 */
			explicit ObjectPoolBase(std::size_t preAllocated = 0, bool growable = true) :
				mGrowable(growable),
				mAllocatedObjectCount(0)
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
				assert(mAllocatedObjectCount == 0);
				Clear();
			}

			/**
			 * \brief 
			 * \tparam Args 
			 * \param args 
			 * \return 
			 */
			template <typename... Args>
			T* AllocateUnsafe(Args&&... args)
			{
				std::lock_guard<Lockable> lg(mMutex);
				if (mVacants.IsEmpty())
				{
					assert(mGrowable);
					AllocatePage((mPages.GetSize() + 1) * 32);
				}

				T* location = mVacants.PopLast();

				++mAllocatedObjectCount;

				return new (location) T(std::forward<Args>(args)...);
			}

			/**
			 * \brief
			 * \tparam Args
			 * \param args
			 * \return
			 */
			template <typename... Args>
			Shared Allocate(Args&&... args)
			{
				return Shared(AllocateUnsafe(std::forward<Args>(args)...), Deleter{ this });
			}

			/**
			 * \brief
			 * \tparam Args
			 * \param args
			 * \return
			 */
			template <typename... Args>
			Unique AllocateUnique(Args&&... args)
			{
				return Unique(AllocateUnsafe(std::forward<Args>(args)...), Deleter{ this });
			}

			/**
			 * \brief
			 * \param object
			 */
			void Recycle(T* object)
			{
				object->~T();
				{
					std::lock_guard<Lockable> lg(mMutex);
					mVacants.Add(object);

					--mAllocatedObjectCount;
				}
			}

			/**
			 * \brief 
			 */
			void Clear()
			{
				std::lock_guard<Lockable> lg(mMutex);
				mVacants.Clear();
				mPages.Clear();
			}

		private:

			/**
			 * \brief 
			 * \param objectCount 
			 */
			void AllocatePage(std::size_t objectCount)
			{
				auto page = MakeUnique<Byte[]>(sizeof(T) * objectCount);
				T* vacants = reinterpret_cast<T*>(page.Get());
			    
			    mVacants.Reserve(objectCount);
				for (std::size_t i = 0; i < objectCount; i++)
					mVacants.Add(&vacants[i]);

				mPages.Add(std::move(page));
			}

			bool mGrowable;
			Lockable mMutex;
			Array<T*> mVacants;
			Array<UniquePtr<Byte[]>> mPages;
			std::size_t mAllocatedObjectCount;
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

