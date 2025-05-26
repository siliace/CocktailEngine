#ifndef COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYBLOCK_HPP
#define COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYBLOCK_HPP

#include <Cocktail/Core/Utility/ObjectPool.hpp>

namespace Ck::Vulkan
{
	class AbstractTexture;
	class Buffer;
	class DeviceMemoryChunk;
	class RenderDevice;

	/**
	 * \brief 
	 */
	class DeviceMemoryBlock
	{
	public:

		/**
		 * \brief 
		 * \param chunk 
		 * \param offset 
		 * \param size
		 * \param ptr 
		 */
		DeviceMemoryBlock(DeviceMemoryChunk* chunk, std::size_t offset, std::size_t size, void* ptr);

		/**
		 * \brief 
		 * \param alignment 
		 * \return 
		 */
		std::size_t ComputeAllocationPadding(std::size_t alignment) const;

		/**
		 * \brief 
		 * \param buffer 
		 */
		void BindToBuffer(const RenderDevice& renderDevice, const Buffer& buffer) const;

		/**
		 * \brief 
		 * \param texture 
		 */
		void BindToTexture(const RenderDevice& renderDevice, const AbstractTexture& texture) const;

		/**
		 * \brief 
		 * \param blockPool
		 * \param alignment 
		 * \param size 
		 * \return 
		 */
		std::shared_ptr<DeviceMemoryBlock> Split(ObjectPool<DeviceMemoryBlock>& blockPool, std::size_t alignment, std::size_t size);

		/**
		 * \brief 
		 * \param block 
		 */
		void Merge(const DeviceMemoryBlock& block);

		/**
		 * \brief 
		 * \param offset 
		 * \return 
		 */
		void* Map(std::size_t offset) const;

		/**
		 * \brief 
		 */
		void Acquire();

		/**
		 * \brief 
		 */
		void Release();
		
		/**
		 * \brief 
		 * \return 
		 */
		DeviceMemoryChunk* GetChunk() const;
		
		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetSize() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsFree() const;

	private:

		DeviceMemoryChunk* mChunk;
		std::size_t mOffset;
		std::size_t mPadding;
		std::size_t mSize;
		void* mPtr;
		bool mFree;
	};
}

#endif // COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYBLOCK_HPP
