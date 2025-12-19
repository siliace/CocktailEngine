#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCache.hpp>

namespace Ck::Vulkan
{
	PipelineCache::PipelineCache(RenderDevice* renderDevice, const PipelineCacheCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		VkPipelineCacheCreateInfo vkCreateInfo{VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO, nullptr};
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.initialDataSize = createInfo.InitialDataSize;
			vkCreateInfo.pInitialData = createInfo.InitialData;
		}

		COCKTAIL_VK_CHECK(vkCreatePipelineCache(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		PipelineCache::SetObjectName(createInfo.Name);
	}

	PipelineCache::~PipelineCache()
	{
		vkDestroyPipelineCache(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void PipelineCache::Merge(const PipelineCache& other) const
	{
		VkPipelineCache sourceCacheHandles[] = {
			other.GetHandle()
		};

		COCKTAIL_VK_CHECK(vkMergePipelineCaches(mRenderDevice->GetHandle(), mHandle, 1, sourceCacheHandles));
	}

	void PipelineCache::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_PIPELINE_CACHE;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	RenderDevice* PipelineCache::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	LargeByteArray PipelineCache::GetCacheData() const
	{
		Uint64 binarySize;
		COCKTAIL_VK_CHECK(vkGetPipelineCacheData(mRenderDevice->GetHandle(), mHandle, &binarySize, nullptr));

		LargeByteArray binary(binarySize);
		COCKTAIL_VK_CHECK(vkGetPipelineCacheData(mRenderDevice->GetHandle(), mHandle, &binarySize, binary.GetData()));

		return binary;
	}

	VkPipelineCache PipelineCache::GetHandle() const
	{
		return mHandle;
	}

}
