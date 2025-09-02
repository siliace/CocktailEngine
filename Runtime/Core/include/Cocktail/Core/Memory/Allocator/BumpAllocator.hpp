#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_BUMPALLOCATOR_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_BUMPALLOCATOR_HPP

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Memory/Allocator/AllocatorUtils.hpp>

#include "Cocktail/Core/Exception.hpp"

namespace Ck
{
	template <unsigned int MaxSize>
	class BumpAllocator
	{
	public:

		static constexpr bool IsTyped = true;

		using SizeType = unsigned int;

		class Raw
		{
		public:

			/**
			 * \brief
			 * \param currentSize
			 * \param nextSize
			 * \param elementSize
			 */
			void ResizeAllocation(SizeType currentSize, SizeType nextSize, std::size_t elementSize)
			{
#ifndef NDEBUG
				SizeType allocationSize = nextSize * elementSize;
				if (currentSize + allocationSize > MaxSize * elementSize)
					throw OutOfMemory();

				mAllocatedCount += allocationSize;
#endif
			}

			/**
			 * \brief
			 * \return
			 */
			SizeType GetInitialCapacity() const
			{
				return 0;
			}

			/**
			 * \brief
			 * \return
			 */
			void* GetAllocation() const
			{
				return mData;
			}

		protected:

			Uint8 mData[MaxSize] = {};
			std::size_t mAllocatedCount = 0;
		};

		template <typename T>
		class Typed
		{
		public:

			/**
			 * \brief 
			 */
			~Typed()
			{
				AllocatorUtils::DestroyRange(mAllocatedCount, mData);
			}

			/**
			 * \brief
			 * \param currentSize
			 * \param nextSize
			 * \param elementSize
			 */
			void ResizeAllocation(SizeType currentSize, SizeType nextSize, std::size_t elementSize)
			{
#ifndef NDEBUG
				SizeType allocationSize = nextSize * elementSize;
				if (currentSize + allocationSize > MaxSize * elementSize)
					throw OutOfMemory();

				mAllocatedCount += allocationSize;
#endif
			}

			/**
			 * \brief
			 * \return
			 */
			SizeType GetInitialCapacity() const
			{
				return 0;
			}

			/**
			 * \brief
			 * \return
			 */
			T* GetAllocation() const
			{
				return (T*)mData;
			}

		protected:

			Uint8 mData[MaxSize * sizeof(T)] = {};
			std::size_t mAllocatedCount = 0;
		};
	};
}
#endif // COCKTAIL_CORE_MEMORY_ALLOCATOR_BUMPALLOCATOR_HPP
