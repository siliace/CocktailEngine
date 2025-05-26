#ifndef COCKTAIL_VULKAN_COMMAND_STAGING_STAGINGBUFFER_HPP
#define COCKTAIL_VULKAN_COMMAND_STAGING_STAGINGBUFFER_HPP

#include <Cocktail/Vulkan/Buffer/Buffer.hpp>

namespace Ck::Vulkan
{
	class StagingBuffer : public Buffer
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		StagingBuffer(std::shared_ptr<RenderDevice> renderDevice, const Renderer::BufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 * \param alignment
		 * \param length 
		 * \param data 
		 * \return 
		 */
		std::size_t PushData(std::size_t alignment, std::size_t length, const void* data);

		/**
		 * \brief 
		 * \param alignment 
		 * \return 
		 */
		std::size_t ComputePadding(std::size_t alignment) const;

		/**
		 * \brief 
		 */
		void Reset();

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetRemainingCapacity() const;

	private:

		std::size_t mRemainingCapacity;
	};
}

#endif // COCKTAIL_VULKAN_COMMAND_STAGING_STAGINGBUFFER_HPP
