#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Buffer/BufferView.hpp>

namespace Ck::Vulkan
{
	BufferView::BufferView(std::shared_ptr<RenderDevice> renderDevice, const Renderer::BufferViewCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mFormat(createInfo.Format),
		mOffset(createInfo.Offset),
		mRange(createInfo.Range),
		mAllocationCallbacks(allocationCallbacks)
	{
		mBuffer = static_cast<Buffer*>(createInfo.Buffer)->shared_from_this();

		VkBufferViewCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.buffer = mBuffer->GetHandle();
			vkCreateInfo.format = ToVkType(mFormat);
			vkCreateInfo.offset = mOffset;
			vkCreateInfo.range = mRange;
		}

		COCKTAIL_VK_CHECK(vkCreateBufferView(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		BufferView::SetObjectName(createInfo.Name);
	}

	BufferView::~BufferView()
	{
		vkDestroyBufferView(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void BufferView::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_BUFFER_VIEW;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	std::shared_ptr<Renderer::RenderDevice> BufferView::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	std::shared_ptr<Renderer::Buffer> BufferView::GetBuffer() const
	{
		return mBuffer;
	}

	PixelFormat BufferView::GetFormat() const
	{
		return mFormat;
	}

	std::size_t BufferView::GetOffset() const
	{
		return mOffset;
	}

	std::size_t BufferView::GetRange() const
	{
		return mRange;
	}

	VkBufferView BufferView::GetHandle() const
	{
		return mHandle;
	}
}
