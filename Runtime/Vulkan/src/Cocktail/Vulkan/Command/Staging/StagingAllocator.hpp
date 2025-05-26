#ifndef COCKTAIL_COMMAND_STAGING_STAGING_STAGINGALLOCATOR_HPP
#define COCKTAIL_COMMAND_STAGING_STAGING_STAGINGALLOCATOR_HPP

#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Vulkan/Command/Staging/StagingBuffer.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	class StagingAllocator
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param bufferSize 
		 */
		StagingAllocator(std::shared_ptr<RenderDevice> renderDevice, std::size_t bufferSize);

		/**
		 * \brief 
		 * \param alignment 
		 * \param size 
		 * \return 
		 */
		std::shared_ptr<StagingBuffer> AcquireStagingBuffer(std::size_t alignment, std::size_t size);

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

		std::shared_ptr<RenderDevice> mRenderDevice;
		std::size_t mBufferSize;
		std::vector<std::shared_ptr<StagingBuffer>> mAcquiredBuffers;
		std::vector<std::shared_ptr<StagingBuffer>> mAvailableBuffers;
		TlsObjectPool<StagingBuffer> mBufferPool;
	};
}

#endif // COCKTAIL_COMMAND_STAGING_STAGING_STAGINGALLOCATOR_HPP
