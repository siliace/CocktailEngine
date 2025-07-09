#ifndef COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYCHUNK_HPP
#define COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYCHUNK_HPP

#include <Cocktail/Vulkan/Memory/DeviceMemory.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryBlock.hpp>
#include <Cocktail/Vulkan/Texture/AbstractTexture.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class DeviceMemoryChunk
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param size
		 * \param memoryTypeIndex
		 */
		DeviceMemoryChunk(std::shared_ptr<RenderDevice> renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, std::size_t size, unsigned int memoryTypeIndex);

		/**
		 * \brief
		 * \param renderDevice
		 * \param texture
		 * \param size
		 * \param memoryTypeIndex
		 */
		DeviceMemoryChunk(std::shared_ptr<RenderDevice> renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, const AbstractTexture* texture, std::size_t size, unsigned int memoryTypeIndex);

		/**
		 * \brief
		 * \param renderDevice
		 * \param buffer
		 * \param size
		 * \param memoryTypeIndex
		 */
		DeviceMemoryChunk(std::shared_ptr<RenderDevice> renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, const Buffer* buffer, std::size_t size, unsigned int memoryTypeIndex);

		/**
		 * \brief 
		 * \param alignment 
		 * \param size 
		 */
		DeviceMemoryBlock* AllocateBlock(std::size_t alignment, std::size_t size);

		/**
		 * \brief 
		 */
		void Compact();

		/**
		 * \brief 
		 * \return 
		 */
		bool IsFree() const;

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetSize() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetMemoryTypeIndex() const;

		/**
		 * \brief Tell whether this chunk as been allocated for a dedicated resource
		 * \return True if dedicated, false otherwise
		 */
		bool IsDedicated() const;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<DeviceMemory> GetDeviceMemory() const;

	private:
		
		/**
		 * \brief 
		 * \param texture 
		 * \param buffer 
		 */
		void AllocateDeviceMemory(const AbstractTexture* texture, const Buffer* buffer);

		std::shared_ptr<RenderDevice> mRenderDevice;
		ObjectPool<DeviceMemoryBlock>& mBlockPool;
		std::size_t mSize;
		unsigned int mMemoryTypeIndex;
		bool mDedicated;
		void* mPtr;
		std::shared_ptr<DeviceMemory> mDeviceMemory;
		Array<std::shared_ptr<DeviceMemoryBlock>> mBlocks;
	};
}

#endif // COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYCHUNK_HPP
