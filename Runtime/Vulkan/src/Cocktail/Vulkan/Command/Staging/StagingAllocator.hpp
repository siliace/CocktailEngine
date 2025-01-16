#ifndef COCKTAIL_COMMAND_STAGING_STAGING_STAGINGALLOCATOR_HPP
#define COCKTAIL_COMMAND_STAGING_STAGING_STAGINGALLOCATOR_HPP

#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Vulkan/Command/Staging/StagingBuffer.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	class StagingAllocator : public Extends<StagingAllocator, Object>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param bufferSize 
		 */
		StagingAllocator(const Ref<RenderDevice>& renderDevice, std::size_t bufferSize);

		/**
		 * \brief 
		 * \param alignment 
		 * \param size 
		 * \return 
		 */
		Ref<StagingBuffer> AcquireStagingBuffer(std::size_t alignment, std::size_t size);

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

		Ref<RenderDevice> mRenderDevice;
		std::size_t mBufferSize;
		std::vector<Ref<StagingBuffer>> mAcquiredBuffers;
		std::vector<Ref<StagingBuffer>> mAvailableBuffers;
		TlsObjectPool<StagingBuffer> mBufferPool;
	};
}

#endif // COCKTAIL_COMMAND_STAGING_STAGING_STAGINGALLOCATOR_HPP
