#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Context/RenderSurface.hpp>
#include <Cocktail/Vulkan/Context/Swapchain.hpp>
#include <Cocktail/Vulkan/Texture/SwapchainTexture.hpp>
#include <Cocktail/Vulkan/WSI/WSI.hpp>

namespace Ck::Vulkan
{
	Swapchain::Swapchain(RenderDevice* renderDevice, const SwapchainCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks):
		mRenderDevice(renderDevice),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		VkPhysicalDevice physicalDevice = mRenderDevice->GetPhysicalDeviceHandle();
				
		const QueueFamilyContext& queueFamilyContext = mRenderDevice->GetQueueFamilyContext();
		Array<unsigned int> presentationFamilyQueueIndexes = queueFamilyContext.FindFamilyIndexes([&](const QueueFamily& queueFamily) {
			return WSI::GetPhysicalDevicePresentationSupport(physicalDevice, queueFamily.GetIndex());
		});

		Swapchain* old = createInfo.Old;

		VkSwapchainCreateInfoKHR vkCreateInfo{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.surface = createInfo.Surface->GetHandle();
			vkCreateInfo.minImageCount = createInfo.MinImageCount;
			vkCreateInfo.imageFormat = ToVkType(createInfo.Format);
			vkCreateInfo.imageColorSpace = ToVkType(createInfo.ColorSpace);
			vkCreateInfo.imageExtent = ToVkType(createInfo.Size);
			vkCreateInfo.imageArrayLayers = 1;
			vkCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			if (queueFamilyContext.IsUnified() || presentationFamilyQueueIndexes.GetSize() == 1)
			{
				vkCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				vkCreateInfo.queueFamilyIndexCount = 0;
				vkCreateInfo.pQueueFamilyIndices = nullptr;
			}
			else
			{
				vkCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				vkCreateInfo.queueFamilyIndexCount = presentationFamilyQueueIndexes.GetSize();
				vkCreateInfo.pQueueFamilyIndices = presentationFamilyQueueIndexes.GetData();
			}

			vkCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			vkCreateInfo.presentMode = createInfo.PresentMode;
			vkCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			vkCreateInfo.clipped = VK_FALSE;
			vkCreateInfo.oldSwapchain = old ? old->GetHandle() : VK_NULL_HANDLE;
		}

		COCKTAIL_VK_CHECK(vkCreateSwapchainKHR(mRenderDevice->GetHandle(), &vkCreateInfo, allocationCallbacks, &mHandle));

		unsigned int swapchainImageCount;
		COCKTAIL_VK_CHECK(vkGetSwapchainImagesKHR(mRenderDevice->GetHandle(), mHandle, &swapchainImageCount, nullptr));

		VkImage swapchainImages[MaxSwapchainTexture];
		COCKTAIL_VK_CHECK(vkGetSwapchainImagesKHR(mRenderDevice->GetHandle(), mHandle, &swapchainImageCount, swapchainImages));

		if (old)
		{
			if (swapchainImageCount != old->GetTextureCount())
				throw std::runtime_error("Swapchain has been recreated with a different image number");
			
			mTextureCount = old->GetTextureCount();
			for (unsigned int i = 0; i < mTextureCount; i++)
			{
				mTextures[i] = std::move(old->mTextures[i]);
				mTextures[i]->Recycle(createInfo.Format, createInfo.Size, swapchainImages[i]);
			}
		}
		else
		{
			mTextureCount = swapchainImageCount;
			for (unsigned int i = 0; i < mTextureCount; i++)
				mTextures[i] = std::make_shared<SwapchainTexture>(mRenderDevice, createInfo.Format, createInfo.Size, swapchainImages[i]);
		}

	    mSize = createInfo.Size;
	}

	Swapchain::~Swapchain()
	{
		vkDestroySwapchainKHR(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void Swapchain::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_SWAPCHAIN_KHR;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(GetHandle());
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Renderer::RenderDevice* Swapchain::GetRenderDevice() const
	{
		return mRenderDevice;
	}

    Extent2D<unsigned int> Swapchain::GetSize() const
    {
	    return mSize;
    }

    unsigned int Swapchain::GetTextureCount() const
	{
		return mTextureCount;
	}

	std::shared_ptr<SwapchainTexture> Swapchain::GetTexture(unsigned int index) const
	{
		if (index >= mTextureCount)
			return nullptr;

		return mTextures[index];
	}

	VkSwapchainKHR Swapchain::GetHandle() const
	{
		return mHandle;
	}
}
