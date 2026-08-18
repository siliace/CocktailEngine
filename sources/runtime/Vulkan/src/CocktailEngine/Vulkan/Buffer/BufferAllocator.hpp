#ifndef COCKTAILENGINE_VULKAN_BUFFER_BUFFERALLOCATOR_HPP
#define COCKTAILENGINE_VULKAN_BUFFER_BUFFERALLOCATOR_HPP

#include <CocktailEngine/Renderer/Buffer/BufferAllocator.hpp>
#include <CocktailEngine/Renderer/Buffer/BufferUsage.hpp>
#include <CocktailEngine/Renderer/Memory/MemoryType.hpp>

#include <CocktailEngine/Vulkan/Buffer/BufferPool.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class BufferAllocator : public Renderer::BufferAllocator
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param usage
		 * \param bufferSize
		 * \param memoryType
		 */
		BufferAllocator(RenderDevice* renderDevice, Renderer::BufferUsageFlags usage, std::size_t bufferSize, Renderer::MemoryType memoryType);

		/**
		 * \brief
		 */
		~BufferAllocator() override;

		/**
		 * \brief
		 * \param size
		 * \param data
		 * \return
		 */
		Renderer::BufferArea PushData(std::size_t size, const void* data) override;

		/**
		 * \brief
		 * \param size
		 */
		void Reserve(std::size_t size) override;

		/**
		 * \brief
		 * \param release
		 */
		void Reset(bool release = false);

		/**
		 * \brief
		 * \return
		 */
		std::size_t GetBufferSize() const;

	private:

		/**
		 * \brief
		 * \param size
		 * \return
		 */
		BufferPool* AcquirePool(std::size_t size);

		RenderDevice* mRenderDevice;
		Renderer::BufferUsageFlags mUsage;
		std::size_t mBufferSize;
		Renderer::MemoryType mMemoryType;
		std::size_t mMinAlignment;
		Array<BufferPool*> mAcquiredBufferPools;
		Array<BufferPool*> mAvailableBufferPools;
		TlsObjectPool<BufferPool> mBufferPool;
	};
}

#endif // COCKTAILENGINE_VULKAN_BUFFER_BUFFERALLOCATOR_HPP
