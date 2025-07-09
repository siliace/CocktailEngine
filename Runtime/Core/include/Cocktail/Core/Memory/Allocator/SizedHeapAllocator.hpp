#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_SIZEDHEAPALLOCATOR_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_SIZEDHEAPALLOCATOR_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
	template <int IndexSize>
	struct IndexSizeToSizeType {};

	template <>	struct IndexSizeToSizeType<8> { using Type = Uint8; };
	template <> struct IndexSizeToSizeType<16> { using Type = Uint16; };
	template <>	struct IndexSizeToSizeType<32> { using Type = Uint32; };
	template <>	struct IndexSizeToSizeType<64> { using Type = Uint64; };

	template <int IndexSize>
	class SizedHeapAllocator
	{
	public:

		static constexpr bool IsTyped = true;

		using SizeType = typename IndexSizeToSizeType<IndexSize>::Type;

		class Raw
		{
		public:

			/**
			 * \brief 
			 */
			Raw() :
				mData(nullptr)
			{
				/// Nothing
			}

			/**
			 * \brief 
			 * \param other 
			 */
			Raw(const Raw& other) = delete;

			/**
			 * \brief 
			 * \param other 
			 */
			Raw(Raw&& other) noexcept
			{
				mData = std::exchange(other.mData, nullptr);
			}

			/**
			 * \brief
			 */
			virtual ~Raw()
			{
				std::free(mData);
			}

			/**
			 * \brief 
			 * \param other 
			 * \return 
			 */
			Raw& operator=(const Raw& other) = delete;

			/**
			 * \brief 
			 * \param other 
			 * \return 
			 */
			Raw& operator=(Raw&& other) noexcept
			{
				if (this == &other)
					return *this;

				std::free(mData);
				mData = std::exchange(other.mData, nullptr);

				return *this;
			}

			/**
			 * \brief 
			 * \param currentSize
			 * \param nextSize
			 * \param elementSize
			 */
			void ResizeAllocation(SizeType currentSize, SizeType nextSize, std::size_t elementSize)
			{
				if (mData || nextSize)
					mData = std::realloc(mData, nextSize * elementSize);
			}

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

			void* mData;
		};

		template <typename T>
		class Typed : public Raw
		{
		public:

			Typed() = default;

			/**
			 * \brief
			 * \param currentSize
			 * \param nextSize
			 * \param elementSize
			 */
			void ResizeAllocation(SizeType currentSize, SizeType nextSize, std::size_t elementSize)
			{
				T* block = static_cast<T*>(std::malloc(nextSize * elementSize));
				MoveRange(currentSize, block, GetAllocation());

				std::free(Raw::mData);
				Raw::mData = block;
			}

			T* GetAllocation() const
			{
				return static_cast<T*>(Raw::GetAllocation());
			}
		};
	};
}

#endif // COCKTAIL_CORE_MEMORY_ALLOCATOR_SIZEDHEAPALLOCATOR_HPP
