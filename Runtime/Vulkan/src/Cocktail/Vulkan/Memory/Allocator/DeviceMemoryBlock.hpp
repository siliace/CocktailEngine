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
		DeviceMemoryBlock(DeviceMemoryChunk* chunk, VkDeviceSize offset, VkDeviceSize size, void* ptr);

		/**
		 * \brief 
		 * \param alignment 
		 * \return 
		 */
		VkDeviceSize ComputeAllocationPadding(VkDeviceSize alignment) const;

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
		std::shared_ptr<DeviceMemoryBlock> Split(ObjectPool<DeviceMemoryBlock>& blockPool, VkDeviceSize alignment, VkDeviceSize size);

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
		void* Map(VkDeviceSize offset) const;

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
		VkDeviceSize GetSize() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsFree() const;

	private:

		DeviceMemoryChunk* mChunk;
		VkDeviceSize mOffset;
		VkDeviceSize mPadding;
		VkDeviceSize mSize;
		void* mPtr;
		bool mFree;
	};
}

#endif // COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYBLOCK_HPP
