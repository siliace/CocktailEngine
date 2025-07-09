#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Texture/Texture.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

namespace Ck::Vulkan
{
	Texture::Texture(std::shared_ptr<RenderDevice> renderDevice, const Renderer::TextureCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		AbstractTexture(renderDevice),
		mType(createInfo.Type),
		mFormat(createInfo.Format),
		mSize(createInfo.Size),
		mExclusive(createInfo.Exclusive),
		mMipMapCount(createInfo.MipMapsCount),
		mArrayLayerCount(createInfo.ArrayLayerCount),
		mAllocationCallbacks(allocationCallbacks)
	{
		const QueueFamilyContext& queueFamilyContext = mRenderDevice->GetQueueFamilyContext();
		Array<unsigned int> queueIndexes = queueFamilyContext.FindFamilyIndexes();

		const bool hasAttachmentFlag = createInfo.Usage & Renderer::TextureUsageFlagBits::Attachment;
		if (mFormat.IsCompressed() && hasAttachmentFlag)
			throw std::invalid_argument("Cannot create a texture with FramebufferAttachment flag and a compressed format");

		const bool hasCubmapCompatibleFlag = createInfo.Flags & Renderer::TextureCreateFlagBits::CubemapCompatible;
		if (mType != Renderer::TextureType::e2D && hasCubmapCompatibleFlag)
			throw std::invalid_argument("Cannot created a texture with CubemapCompatible flag and a texture type other than 2D");

		const bool hasArray2DCompatibleFlag = createInfo.Flags & Renderer::TextureCreateFlagBits::Array2DCompatible;
		if (mType != Renderer::TextureType::e3D && hasArray2DCompatibleFlag)
			throw std::invalid_argument("Cannot created a texture with Array2DCompatible flag and a texture type other than 3D");

		VkImageCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.imageType = ToVkType(mType);
			vkCreateInfo.format = ToVkType(mFormat);
			vkCreateInfo.extent = ToVkType(mSize);
			vkCreateInfo.mipLevels = mMipMapCount;
			vkCreateInfo.arrayLayers = mArrayLayerCount;
			vkCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			// TODO: Handle unified memory type 
			vkCreateInfo.tiling = createInfo.MemoryType == Renderer::MemoryType::Static ? VK_IMAGE_TILING_OPTIMAL : VK_IMAGE_TILING_LINEAR;
			vkCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

			if (createInfo.Usage & Renderer::TextureUsageFlagBits::Sampled)
				vkCreateInfo.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;

			if (createInfo.Usage & Renderer::TextureUsageFlagBits::Storage)
				vkCreateInfo.usage |= VK_IMAGE_USAGE_STORAGE_BIT;

			if (hasAttachmentFlag)
			{
				if (mFormat.IsColor())
				{
					vkCreateInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
					
				}
				else
				{
					vkCreateInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
				}
			}

			if (queueFamilyContext.IsUnified() || createInfo.Exclusive)
			{
				vkCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				vkCreateInfo.queueFamilyIndexCount = 0;
				vkCreateInfo.pQueueFamilyIndices = nullptr;
			}
			else
			{
				vkCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
				vkCreateInfo.queueFamilyIndexCount = queueIndexes.GetSize();
				vkCreateInfo.pQueueFamilyIndices = queueIndexes.GetData();
			}
			vkCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		}

		std::unique_ptr<VkFormat[]> vkViewFormats;
		VkImageFormatListCreateInfoKHR imageFormatList{ VK_STRUCTURE_TYPE_IMAGE_FORMAT_LIST_CREATE_INFO_KHR, nullptr };
		if (Renderer::TextureMutableFormatCreateInfo* mutableFormat = createInfo.MutableFormat; mutableFormat)
		{
			SetViewFormats(mutableFormat->CompatibleFormatCount, mutableFormat->CompatibleFormats);

			vkViewFormats = std::make_unique<VkFormat[]>(mutableFormat->CompatibleFormatCount);
			for (unsigned int i = 0; i < mutableFormat->CompatibleFormatCount; i++)
				vkViewFormats[i] = ToVkType(mutableFormat->CompatibleFormats[i]);

			imageFormatList.viewFormatCount = mutableFormat->CompatibleFormatCount;
			imageFormatList.pViewFormats = vkViewFormats.get();

			imageFormatList.pNext = vkCreateInfo.pNext;
			vkCreateInfo.pNext = &imageFormatList;
		}

		COCKTAIL_VK_CHECK(vkCreateImage(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		mDeviceMemoryBlock = mRenderDevice->Invoke([&](DeviceMemoryAllocator* deviceMemoryAllocator) {
			return deviceMemoryAllocator->Allocate(*this);
		});

		Texture::SetObjectName(createInfo.Name);
	}

	Texture::~Texture()
	{
		mDeviceMemoryBlock->Release();
		vkDestroyImage(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	bool Texture::IsExclusive() const
	{
		return mExclusive;
	}

	Renderer::MemoryType Texture::GetMemoryType() const
	{
		return Renderer::MemoryType::Static;
	}

	Renderer::TextureType Texture::GetType() const
	{
		return mType;
	}

	PixelFormat Texture::GetFormat() const
	{
		return mFormat;
	}

	Extent3D<unsigned int> Texture::GetSize() const
	{
		return mSize;
	}

	unsigned int Texture::GetMipMapCount() const
	{
		return mMipMapCount;
	}

	unsigned int Texture::GetArrayLayerCount() const
	{
		return mArrayLayerCount;
	}

	VkImage Texture::GetHandle() const
	{
		return mHandle;
	}
}
