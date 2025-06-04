#ifndef COCKTAIL_COMMAND_STAGING_STAGING_STAGINGALLOCATOR_HPP
#define COCKTAIL_COMMAND_STAGING_STAGING_STAGINGALLOCATOR_HPP

#include <Cocktail/Vulkan/Command/Staging/StagingBuffer.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief Allocator class to acquire 'one-shot' buffer memory
	 */
	class StagingAllocator
	{
	public:

		/**
		 * \brief Constructor
		 * Create a new StagingAllocator
		 * \param renderDevice The render device to use to create buffers
		 * \param bufferUsage The usage of buffers to create
		 * \param bufferSize The size of buffers to create
		 */
		StagingAllocator(std::shared_ptr<RenderDevice> renderDevice, Renderer::BufferUsageFlags bufferUsage, std::size_t bufferSize);

		/**
		 * \brief Acquire a staging buffer with at last \p allocationSize bytes available alignment on a specified \p alignment
		 * The allocator will try to reuse a much as possible existing buffers.
		 * If no staging buffer as enough space to fit \p allocationSize, a new buffer of the appropriate size will be created.
		 * \param alignment The alignment the data to fit in the staging buffer should respect
		 * \param allocationSize The size (in bytes) of the data to fit in the staging buffer
		 * \return A staging buffer
		 */
		StagingBuffer& AcquireStagingBuffer(std::size_t alignment, std::size_t allocationSize);

		/**
		 * \brief Ensure at least \p allocationSize is available in one of the staging buffer
		 * If no buffer can fit \p allocationSize, a new buffer will be created.
		 * \param allocationSize The size to reserve
		 */
		void Reserve(std::size_t allocationSize);

		/**
		 * \brief Reset the allocator
		 * Reset the allocator will discard the content of any StagingBuffer previously acquired from AcquireStagingBuffer.
		 * If \p release is false, the allocator will reuse existing buffers instead of creating new ones.
		 * If \p release is true, the allocator will destroy any acquired buffers.
		 * \param release Flag to indicate whether releasing also underlying buffers
		 */
		void Reset(bool release = false);

	private:

		std::shared_ptr<RenderDevice> mRenderDevice;
		Renderer::BufferUsageFlags mBufferUsage;
		std::size_t mBufferSize;
		std::vector<StagingBuffer> mAcquiredBuffers;
		std::vector<StagingBuffer> mAvailableBuffers;
	};
}

#endif // COCKTAIL_COMMAND_STAGING_STAGING_STAGINGALLOCATOR_HPP
