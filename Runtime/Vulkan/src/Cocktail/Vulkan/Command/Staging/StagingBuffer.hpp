#ifndef COCKTAIL_VULKAN_COMMAND_STAGING_STAGINGBUFFER_HPP
#define COCKTAIL_VULKAN_COMMAND_STAGING_STAGINGBUFFER_HPP

#include <Cocktail/Vulkan/Buffer/Buffer.hpp>

namespace Ck::Vulkan
{
	class StagingBuffer
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param bufferUsage 
		 * \param bufferSize 
		 */
		StagingBuffer(const std::shared_ptr<RenderDevice>& renderDevice, Renderer::BufferUsageFlags bufferUsage, std::size_t bufferSize);

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
		Buffer* GetBuffer() const;

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetRemainingCapacity() const;

	private:

		std::shared_ptr<Buffer> mBuffer;
		std::size_t mRemainingCapacity;
	};
}

#endif // COCKTAIL_VULKAN_COMMAND_STAGING_STAGINGBUFFER_HPP
