#ifndef COCKTAIL_VULKAN_MEMORY_DEVICEMEMORY_HPP
#define COCKTAIL_VULKAN_MEMORY_DEVICEMEMORY_HPP

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

#include <Cocktail/Vulkan/Memory/DeviceMemoryCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class DeviceMemory : public Renderer::RenderDeviceObject
	{
	public:

		DeviceMemory(RenderDevice* renderDevice, const DeviceMemoryCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		~DeviceMemory() override;

		void SetObjectName(const char* name) const override;

		Renderer::RenderDevice* GetRenderDevice() const override;

		VkDeviceMemory GetHandle() const;

	private:

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkDeviceMemory mHandle;
		std::size_t mSize;
	};
}

#endif // COCKTAIL_VULKAN_MEMORY_DEVICEMEMORY_HPP
