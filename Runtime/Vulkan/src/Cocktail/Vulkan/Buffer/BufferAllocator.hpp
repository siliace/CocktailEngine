#ifndef COCKTAIL_VULKAN_BUFFER_BUFFERALLOCATOR_HPP
#define COCKTAIL_VULKAN_BUFFER_BUFFERALLOCATOR_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/Buffer/BufferAllocator.hpp>
#include <Cocktail/Renderer/Buffer/BufferUsage.hpp>
#include <Cocktail/Renderer/Memory/MemoryType.hpp>

#include <Cocktail/Vulkan/Buffer/BufferPool.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class BufferAllocator : public Inherit<BufferAllocator, Object, Renderer::BufferAllocator>
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param usage
		 * \param bufferSize
		 * \param memoryType
		 */
		BufferAllocator(Ref<RenderDevice> renderDevice, Renderer::BufferUsageFlags usage, std::size_t bufferSize, Renderer::MemoryType memoryType);

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
		Ref<BufferPool> AcquirePool(std::size_t size);

		Ref<RenderDevice> mRenderDevice;
		Renderer::BufferUsageFlags mUsage;
		std::size_t mBufferSize;
		Renderer::MemoryType mMemoryType;
		std::size_t mMinAlignment;
		std::vector<Ref<BufferPool>> mAcquiredBufferPools;
		std::vector<Ref<BufferPool>> mAvailableBufferPools;
		TlsObjectPool<BufferPool> mBufferPool;
	};
}

#endif // COCKTAIL_VULKAN_BUFFER_BUFFERALLOCATOR_HPP
