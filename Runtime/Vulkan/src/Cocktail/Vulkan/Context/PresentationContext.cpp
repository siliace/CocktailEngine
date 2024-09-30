#include <algorithm>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Context/PresentationContext.hpp>
#include <Cocktail/Vulkan/Queue/QueueFamilyContext.hpp>

namespace Ck::Vulkan
{
	namespace
	{
		PixelFormat ChooseSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, DisplayColorDepth colorDepth, DisplayAlphaDepth alphaDepth, Renderer::ColorSpace colorSpace)
		{
			unsigned int surfaceFormatCount;
			COCKTAIL_VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr));

			std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
			COCKTAIL_VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data()));

			for (const VkSurfaceFormatKHR& surfaceFormat : surfaceFormats)
			{
				PixelFormat format = FromVkType(surfaceFormat.format);
				
				if (Renderer::FormatToColorDepth(format) != colorDepth)
					continue;

				if (Renderer::FormatToAlphaDepth(format) != alphaDepth)
					continue;

				if (colorSpace != Renderer::ColorSpace::Hdr10)
				{
					Renderer::ColorSpace formatColorSpace = Renderer::FormatToColorSpace(format);
					if (formatColorSpace != Renderer::ColorSpace::Linear && formatColorSpace != colorSpace)
						continue;

					if (FromVkType(surfaceFormat.colorSpace) != Renderer::ColorSpace::Srgb)
						continue;
				}
				else
				{
					if (FromVkType(surfaceFormat.colorSpace) != colorSpace)
						continue;
				}

				return format;
			}

			throw std::runtime_error("Failed to find compatible format for RenderSurface");
		}

		Extent2D<unsigned int> ChooseSwapchainExtent(const Extent2D<unsigned int>& windowSize, const VkSurfaceCapabilitiesKHR& capabilities)
		{
			if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
				return MakeExtent(capabilities.currentExtent.width, capabilities.currentExtent.height);

			Extent2D<unsigned int> actualExtent;
			actualExtent.Width = std::clamp(windowSize.Width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.Height = std::clamp(windowSize.Height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	PresentationContext::PresentationContext(Ref<RenderDevice> renderDevice, VkSurfaceKHR surface, unsigned int bufferCount, DisplayColorDepth colorDepth, DisplayAlphaDepth alphaDepth, Renderer::ColorSpace colorSpace) :
		mRenderDevice(std::move(renderDevice)),
		mSurface(surface),
		mSurfaceColorSpace(colorSpace)
	{
		VkPhysicalDevice physicalDevice = mRenderDevice->GetPhysicalDeviceHandle();

		VkBool32 supported;
		const QueueFamilyContext& queueFamilyContext = mRenderDevice->GetQueueFamilyContext();
		COCKTAIL_VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyContext.GetFamily(Renderer::CommandQueueType::Graphic).GetIndex(), mSurface, &supported));
		if (supported == VK_FALSE)
			throw std::runtime_error("Surface not supported by physical device");

		unsigned int presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mSurface, &presentModeCount, nullptr);

		mPresentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, mSurface, &presentModeCount, mPresentModes.data());

		mSurfaceFormat = ChooseSurfaceFormat(physicalDevice, mSurface, colorDepth, alphaDepth, mSurfaceColorSpace);

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		COCKTAIL_VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mRenderDevice->GetPhysicalDeviceHandle(), mSurface, &surfaceCapabilities));
		mBufferCount = std::clamp(bufferCount, surfaceCapabilities.minImageCount, surfaceCapabilities.maxImageCount);
	}

	Ref<Swapchain> PresentationContext::CreateSwapchain(const Extent2D<unsigned int>& size, VkPresentModeKHR presentMode, Swapchain* old) const
	{
		if (!IsPresentationModeSupported(presentMode))
			throw std::invalid_argument("PresentMode not supported by the PresentationContext");

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		COCKTAIL_VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mRenderDevice->GetPhysicalDeviceHandle(), mSurface, &surfaceCapabilities));

		SwapchainCreateInfo createInfo;
		createInfo.Surface = mSurface;
		createInfo.MinImageCount = mBufferCount;
		createInfo.Format = mSurfaceFormat;
		createInfo.ColorSpace = mSurfaceColorSpace;
		createInfo.Size = ChooseSwapchainExtent(size, surfaceCapabilities);
		createInfo.PresentMode = presentMode;
		createInfo.Old = old;

		return mRenderDevice->CreateSwapchain(createInfo);
	}

	bool PresentationContext::IsPresentationModeSupported(VkPresentModeKHR presentMode) const
	{
		return std::find(mPresentModes.begin(), mPresentModes.end(), presentMode) != mPresentModes.end();
	}

	PixelFormat PresentationContext::GetSurfaceFormat() const
	{
		return mSurfaceFormat;
	}

	Renderer::ColorSpace PresentationContext::GetSurfaceColorSpace() const
	{
		return mSurfaceColorSpace;
	}

	unsigned int PresentationContext::GetBufferCount() const
	{
		return mBufferCount;
	}
}
