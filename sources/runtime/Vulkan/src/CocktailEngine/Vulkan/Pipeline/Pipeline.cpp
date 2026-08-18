#include <CocktailEngine/Vulkan/RenderDevice.hpp>
#include <CocktailEngine/Vulkan/VulkanUtils.hpp>
#include <CocktailEngine/Vulkan/Pipeline/Pipeline.hpp>

namespace Ck::Vulkan
{
	Pipeline::Pipeline(RenderDevice* renderDevice) :
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

	Renderer::RenderDevice* Pipeline::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	const PipelineCreationFeedback& Pipeline::GetCreationFeedback() const
	{
		return mCreationFeedback;
	}
}
