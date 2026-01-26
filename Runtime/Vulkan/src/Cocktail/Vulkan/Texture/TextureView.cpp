#include <Cocktail/Renderer/PixelFormat.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Texture/TextureView.hpp>

namespace Ck::Vulkan
{
	TextureView::TextureView(RenderDevice* renderDevice, const Renderer::TextureViewCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		mTexture = std::static_pointer_cast<AbstractTexture>(createInfo.Texture);

		if (mTexture->HasMutableFormat())
		{
			mFormat = createInfo.Format;
		}
		else
		{
			mFormat = mTexture->GetFormat();
		}

		VkImageViewCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr };
		{
			VkImageAspectFlags aspectMask = 0;
			if (mFormat.IsColor())
			{
				aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			}
			else
			{
				switch (mFormat.GetLayout())
				{
				case PixelFormat::Layout::Depth:
				case PixelFormat::Layout::DepthStencil:
					aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
					break;

				case PixelFormat::Layout::Stencil:
					aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
					break;

				default:
					COCKTAIL_UNREACHABLE();
				}
			}

			vkCreateInfo.flags = 0;
			vkCreateInfo.image = mTexture->GetHandle();
			vkCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			vkCreateInfo.format = ToVkType(mFormat);
			vkCreateInfo.components.r = ToVkType(createInfo.Swizzle.Red);
			vkCreateInfo.components.g = ToVkType(createInfo.Swizzle.Green);
			vkCreateInfo.components.b = ToVkType(createInfo.Swizzle.Blue);
			vkCreateInfo.components.a = ToVkType(createInfo.Swizzle.Alpha);

			vkCreateInfo.subresourceRange.aspectMask = aspectMask;
			if (createInfo.SubResource)
			{
				vkCreateInfo.subresourceRange.baseArrayLayer = createInfo.SubResource->BaseArrayLayer;
				vkCreateInfo.subresourceRange.baseMipLevel = createInfo.SubResource->BaseMipMapLevel;
				vkCreateInfo.subresourceRange.layerCount = createInfo.SubResource->ArrayLayerCount;
				vkCreateInfo.subresourceRange.levelCount = createInfo.SubResource->MipMapLevelCount;
			}
			else
			{
				vkCreateInfo.subresourceRange.baseMipLevel = 0;
				vkCreateInfo.subresourceRange.levelCount = mTexture->GetMipMapCount();
				vkCreateInfo.subresourceRange.baseArrayLayer = 0;
				vkCreateInfo.subresourceRange.layerCount = mTexture->GetArrayLayerCount();
			}
		}

		COCKTAIL_VK_CHECK(vkCreateImageView(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));
	}

	TextureView::~TextureView()
	{
		vkDestroyImageView(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void TextureView::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_IMAGE_VIEW;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(GetHandle());
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Renderer::RenderDevice* TextureView::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	std::shared_ptr<Renderer::Texture> TextureView::GetTexture() const
	{
		return mTexture;
	}

	PixelFormat TextureView::GetFormat() const
	{
		return mFormat;
	}

	VkImageView TextureView::GetHandle() const
	{
		return mHandle;
	}
}
