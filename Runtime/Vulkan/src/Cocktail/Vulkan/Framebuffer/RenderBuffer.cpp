#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderBuffer.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

namespace Ck::Vulkan
{
	RenderBuffer::RenderBuffer(std::shared_ptr<RenderDevice> renderDevice, const RenderBufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		AbstractTexture(renderDevice),
		mFormat(createInfo.Format),
		mSize(MakeExtent(createInfo.Size, 1u)),
		mSamples(createInfo.Samples),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		VkImageCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			vkCreateInfo.format = ToVkType(mFormat);
			vkCreateInfo.extent = ToVkType(mSize);
			vkCreateInfo.mipLevels = 1;
			vkCreateInfo.arrayLayers = 1;
			vkCreateInfo.samples = ToVkType(mSamples);
			// TODO: Handle unified memory type 
			vkCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			vkCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
			vkCreateInfo.usage |= mFormat.IsColor() ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			vkCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			vkCreateInfo.queueFamilyIndexCount = 0;
			vkCreateInfo.pQueueFamilyIndices = nullptr;
			vkCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		}

		COCKTAIL_VK_CHECK(vkCreateImage(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		mDeviceMemoryBlock = mRenderDevice->Invoke([&](DeviceMemoryAllocator* deviceMemoryAllocator) {
			return deviceMemoryAllocator->Allocate(*this);
		});

		RenderBuffer::SetObjectName(createInfo.Name);
	}

	RenderBuffer::~RenderBuffer()
	{
		mDeviceMemoryBlock->Release();
		vkDestroyImage(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	bool RenderBuffer::IsExclusive() const
	{
		return true;
	}

	Renderer::MemoryType RenderBuffer::GetMemoryType() const
	{
		return Renderer::MemoryType::Static;
	}

	Renderer::TextureType RenderBuffer::GetType() const
	{
		return Renderer::TextureType::e2D;
	}

	PixelFormat RenderBuffer::GetFormat() const
	{
		return mFormat;
	}

	Extent3D<unsigned int> RenderBuffer::GetSize() const
	{
		return mSize;
	}

	unsigned RenderBuffer::GetMipMapCount() const
	{
		return 1;
	}

	unsigned RenderBuffer::GetArrayLayerCount() const
	{
		return 1;
	}

	Renderer::RasterizationSamples RenderBuffer::GetSamples() const
	{
		return mSamples;
	}

	VkImage RenderBuffer::GetHandle() const
	{
		return mHandle;
	}
}
