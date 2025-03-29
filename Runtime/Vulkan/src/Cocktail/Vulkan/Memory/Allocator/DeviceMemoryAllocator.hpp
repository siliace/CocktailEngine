#ifndef COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYALLOCATOR_HPP
#define COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYALLOCATOR_HPP

#include <algorithm>

#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryChunk.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class DeviceMemoryAllocator
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice
		 * \param defaultChuckSize 
		 */
		DeviceMemoryAllocator(Ref<RenderDevice> renderDevice, std::size_t defaultChuckSize);

		/**
		 * \brief 
		 * \param texture
		 * \return 
		 */
		DeviceMemoryBlock* Allocate(const AbstractTexture& texture);

		/**
		 * \brief 
		 * \param buffer 
		 * \return 
		 */
		DeviceMemoryBlock* Allocate(const Buffer& buffer);

		/**
		 * \brief 
		 * \param compact 
		 */
		void GarbageCollect(bool compact);

	private:

		/**
		 * \brief 
		 * \param buffer
		 * \param requirements 
		 * \param dedicated 
		 */
		void GetBufferMemoryRequirements(const Buffer& buffer, VkMemoryRequirements& requirements, bool& dedicated) const;

		/**
		 * \brief
		 * \param texture
		 * \param requirements
		 * \param dedicated
		 */
		void GetTextureMemoryRequirements(const AbstractTexture& texture, VkMemoryRequirements& requirements, bool& dedicated) const;

		/**
		 * \brief 
		 * \param typeIndexMask 
		 * \param memoryType 
		 * \return 
		 */
		unsigned int FindMemoryTypeIndex(unsigned int typeIndexMask, Renderer::MemoryType memoryType) const;

		/**
		 * \brief 
		 * \tparam T 
		 * \param resource 
		 * \param memoryRequirements 
		 * \param memoryTypeIndex 
		 * \return 
		 */
		template <typename T>
		DeviceMemoryBlock* AllocateBlock(const T* resource, const VkMemoryRequirements& memoryRequirements, unsigned int memoryTypeIndex)
		{
			DeviceMemoryBlock* block = nullptr;
			if (resource)
			{
				Ref<DeviceMemoryChunk> chunk = mChunkPool.Allocate(mRenderDevice, mBlockPool, resource, memoryRequirements.size, memoryTypeIndex);
				mChunks.push_back(chunk);

				block = chunk->AllocateBlock(memoryRequirements.alignment, memoryRequirements.size);
			}
			else
			{
				for (const Ref<DeviceMemoryChunk>& chunk : mChunks)
				{
					if (chunk->IsDedicated())
						continue;

					if (chunk->GetMemoryTypeIndex() != memoryTypeIndex)
						continue;

					block = chunk->AllocateBlock(memoryRequirements.alignment, memoryRequirements.size);
				}

				if (!block)
				{
					Ref<DeviceMemoryChunk> chunk = mChunkPool.Allocate(mRenderDevice, mBlockPool, std::max(memoryRequirements.size, mDefaultChuckSize), memoryTypeIndex);
					mChunks.push_back(chunk);

					block = chunk->AllocateBlock(memoryRequirements.alignment, memoryRequirements.size);
				}
			}

			return block;
		}

		Ref<RenderDevice> mRenderDevice;
		std::size_t mDefaultChuckSize;
		std::vector<Ref<DeviceMemoryChunk>> mChunks;
		ObjectPool<DeviceMemoryChunk> mChunkPool;
		ObjectPool<DeviceMemoryBlock> mBlockPool;
	}; 
}

#endif // COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYALLOCATOR_HPP
