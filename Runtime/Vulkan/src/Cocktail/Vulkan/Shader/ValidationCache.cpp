#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Shader/ValidationCache.hpp>

namespace Ck::Vulkan
{
	ValidationCache::ValidationCache(RenderDevice* renderDevice, const ValidationCacheCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		VkValidationCacheCreateInfoEXT vkCreateInfo{ VK_STRUCTURE_TYPE_VALIDATION_CACHE_CREATE_INFO_EXT, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.initialDataSize = createInfo.InitialDataSize;
			vkCreateInfo.pInitialData = createInfo.InitialData;
		}

		COCKTAIL_VK_CHECK(vkCreateValidationCacheEXT(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		ValidationCache::SetObjectName(createInfo.Name);
	}

	ValidationCache::~ValidationCache()
	{
		vkDestroyValidationCacheEXT(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void ValidationCache::Merge(const ValidationCache& other) const
	{
		VkValidationCacheEXT sourceHandles[] = {
			other.GetHandle()
		};
		COCKTAIL_VK_CHECK(vkMergeValidationCachesEXT(mRenderDevice->GetHandle(), mHandle, 1, sourceHandles));
	}

	void ValidationCache::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_VALIDATION_CACHE_EXT;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	RenderDevice* ValidationCache::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	ByteArray ValidationCache::GetCacheData() const
	{
		std::size_t cacheDataSize;
		COCKTAIL_VK_CHECK(vkGetValidationCacheDataEXT(mRenderDevice->GetHandle(), mHandle, &cacheDataSize, nullptr));

		ByteArray cacheData(cacheDataSize);
		COCKTAIL_VK_CHECK(vkGetValidationCacheDataEXT(mRenderDevice->GetHandle(), mHandle, &cacheDataSize, cacheData.GetData()));

		return cacheData;
	}

	VkValidationCacheEXT ValidationCache::GetHandle() const
	{
		return mHandle;
	}
}
