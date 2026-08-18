#ifndef COCKTAILENGINE_VULKAN_BUFFER_BUFFERPOOL_HPP
#define COCKTAILENGINE_VULKAN_BUFFER_BUFFERPOOL_HPP

#include <CocktailEngine/Vulkan/Volk.hpp>
#include <CocktailEngine/Vulkan/Buffer/Buffer.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class BufferPool : public Buffer
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param createInfo
		 * \param allocationCallbacks
		 */
		BufferPool(RenderDevice* renderDevice, const Renderer::BufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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

#endif // COCKTAILENGINE_VULKAN_BUFFER_BUFFERPOOL_HPP
