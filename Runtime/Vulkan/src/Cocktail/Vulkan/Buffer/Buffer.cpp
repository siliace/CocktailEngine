#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Buffer/Buffer.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

namespace Ck::Vulkan
{
	Buffer::Buffer(RenderDevice* renderDevice, const Renderer::BufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE),
		mUsage(createInfo.Usage),
		mMemoryType(createInfo.MemoryType),
		mSize(createInfo.Size),
		mExclusive(createInfo.Exclusive)
	{
		const QueueFamilyContext& queueFamilyContext = mRenderDevice->GetQueueFamilyContext();
		Array<unsigned int> queueFamilyIndexes = queueFamilyContext.FindFamilyIndexes();

		VkBufferCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr };
		{
			const VkBufferUsageFlags transferUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			vkCreateInfo.flags = 0;
			vkCreateInfo.size = mSize;
			vkCreateInfo.usage = transferUsage | ToVkTypes(mUsage);

			if (queueFamilyContext.IsUnified() || createInfo.Exclusive)
			{
				vkCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				vkCreateInfo.queueFamilyIndexCount = 0;
				vkCreateInfo.pQueueFamilyIndices = nullptr;
			}
			else
			{
				vkCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
				vkCreateInfo.queueFamilyIndexCount = queueFamilyIndexes.GetSize();
				vkCreateInfo.pQueueFamilyIndices = queueFamilyIndexes.GetData();
			}
		}

		COCKTAIL_VK_CHECK(vkCreateBuffer(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		mMemoryBlock = mRenderDevice->Invoke([&](DeviceMemoryAllocator* deviceMemoryAllocator) {
			return deviceMemoryAllocator->Allocate(*this);
		});

		Buffer::SetObjectName(createInfo.Name);
	}

	Buffer::~Buffer()
	{
		mMemoryBlock->Release();
		vkDestroyBuffer(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}
	
	void Buffer::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_BUFFER;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Renderer::RenderDevice* Buffer::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	bool Buffer::IsExclusive() const
	{
		return mExclusive;
	}

	Renderer::MemoryType Buffer::GetMemoryType() const
	{
		return mMemoryType;
	}

	Renderer::ResourceType Buffer::GetResourceType() const
	{
		return Renderer::ResourceType::Buffer;
	}

	Renderer::BufferUsageFlags Buffer::GetUsage() const
	{
		return mUsage;
	}

	void* Buffer::Map(std::size_t offset, std::size_t length)
	{
		return mMemoryBlock->Map(offset);
	}

	void Buffer::Unmap()
	{
	}

	std::size_t Buffer::GetSize() const
	{
		return mSize;
	}

	VkBuffer Buffer::GetHandle() const
	{
		return mHandle;
	}
}
