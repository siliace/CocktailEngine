#ifndef COCKTAIL_VULKAN_BUFFER_BUFFERPOOL_HPP
#define COCKTAIL_VULKAN_BUFFER_BUFFERPOOL_HPP

#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/Buffer/Buffer.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class BufferPool : public Extends<BufferPool, Buffer>
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param createInfo
		 * \param allocationCallbacks
		 */
		BufferPool(Ref<RenderDevice> renderDevice, const Renderer::BufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
#endif // COCKTAIL_VULKAN_BUFFER_BUFFERPOOL_HPP
