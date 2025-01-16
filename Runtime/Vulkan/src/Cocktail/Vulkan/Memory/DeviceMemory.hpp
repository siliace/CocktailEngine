#ifndef COCKTAIL_VULKAN_MEMORY_DEVICEMEMORY_HPP
#define COCKTAIL_VULKAN_MEMORY_DEVICEMEMORY_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

#include <Cocktail/Vulkan/Memory/DeviceMemoryCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class DeviceMemory : public Inherit<DeviceMemory, Object, Renderer::RenderDeviceObject>
	{
	public:

		DeviceMemory(Ref<RenderDevice> renderDevice, const DeviceMemoryCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		~DeviceMemory() override;

		void SetObjectName(const char* name) const override;

		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

		VkDeviceMemory GetHandle() const;

	private:

		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkDeviceMemory mHandle;
		std::size_t mSize;
	};
}

#endif // COCKTAIL_VULKAN_MEMORY_DEVICEMEMORY_HPP
