#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Pipeline/Pipeline.hpp>

namespace Ck::Vulkan
{
	Pipeline::Pipeline(std::shared_ptr<RenderDevice> renderDevice) :
		mRenderDevice(renderDevice)
	{
		/// Nothing
	}

	void Pipeline::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_PIPELINE;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(GetHandle());
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	std::shared_ptr<Renderer::RenderDevice> Pipeline::GetRenderDevice() const
	{
		return mRenderDevice;
	}
}
