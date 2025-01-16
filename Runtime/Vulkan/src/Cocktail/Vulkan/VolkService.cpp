#include <stdexcept>

#include <Cocktail/Vulkan/VolkService.hpp>

namespace Ck::Vulkan
{
	VolkService::VolkService()
	{
		if (VkResult result = volkInitialize(); result != VK_SUCCESS)
			throw std::runtime_error("Failed to initialize Volk");
	}

	void VolkService::LoadInstanceOnly(VkInstance instance) const
	{
		volkLoadInstanceOnly(instance);
	}

	void VolkService::LoadDevice(VkDevice device) const
	{
		volkLoadDevice(device);
	}
}
