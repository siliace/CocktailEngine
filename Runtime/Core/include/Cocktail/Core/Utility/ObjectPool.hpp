#ifndef COCKTAIL_CORE_UTILITY_OBJECTPOOL_HPP
#define COCKTAIL_CORE_UTILITY_OBJECTPOOL_HPP

#include <mutex>
#include <memory>
#include <vector>

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

			using SharedPtr = std::shared_ptr<T>;
			using UniquePtr = std::unique_ptr<T, Deleter>;

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
			T* AllocateUnsafe(Args&&... args)
			{
				std::lock_guard<Lockable> lg(mMutex);
				if (mVacants.empty())
				{
					assert(mGrowable);
					AllocatePage((mPages.size() + 1) * 32);
				}

				T* location = mVacants.back();
				mVacants.pop_back();

				return new (location) T(std::forward<Args>(args)...);
			}

			/**
			 * \brief
			 * \tparam Args
			 * \param args
			 * \return
			 */
			template <typename... Args>
			SharedPtr Allocate(Args&&... args)
			{
				return SharedPtr(AllocateUnsafe(std::forward<Args>(args)...), Deleter{ this });
			}

			/**
			 * \brief
			 * \tparam Args
			 * \param args
			 * \return
			 */
			template <typename... Args>
			UniquePtr AllocateUnique(Args&&... args)
			{
				return UniquePtr(AllocateUnsafe(std::forward<Args>(args)...), Deleter{this });
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
					mVacants.emplace_back(object);
				}
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

			/**
			 * \brief 
			 * \param objectCount 
			 */
			void AllocatePage(std::size_t objectCount)
			{
				auto page = std::make_unique<unsigned char[]>(sizeof(T) * objectCount);
				T* vacants = reinterpret_cast<T*>(page.get());
				for (std::size_t i = 0; i < objectCount; i++)
					mVacants.emplace_back(&vacants[i]);

				mPages.emplace_back(std::move(page));
			}

			bool mGrowable;
			Lockable mMutex;
			std::vector<T*> mVacants;
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

