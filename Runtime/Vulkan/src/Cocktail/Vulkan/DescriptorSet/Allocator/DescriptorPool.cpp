#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorPool.hpp>

namespace Ck::Vulkan
{
	DescriptorPool::DescriptorPool(std::shared_ptr<RenderDevice> renderDevice, const DescriptorPoolCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		assert(!createInfo.LayoutSignature.SupportPushDescriptor);

		mSupportFree = createInfo.SupportFree;

		unsigned int sizeCount = 0;
		VkDescriptorPoolSize poolSizes[Enum<Renderer::DescriptorType>::ValueCount];
		PushPoolSize(poolSizes, sizeCount, Renderer::DescriptorType::Sampler, createInfo.LayoutSignature.SamplerCount, createInfo.MaxSet);
		PushPoolSize(poolSizes, sizeCount, Renderer::DescriptorType::TextureSampler, createInfo.LayoutSignature.TextureSamplerCount, createInfo.MaxSet);
		PushPoolSize(poolSizes, sizeCount, Renderer::DescriptorType::Texture, createInfo.LayoutSignature.TextureCount, createInfo.MaxSet);
		PushPoolSize(poolSizes, sizeCount, Renderer::DescriptorType::StorageTexture, createInfo.LayoutSignature.StorageTextureCount, createInfo.MaxSet);
		PushPoolSize(poolSizes, sizeCount, Renderer::DescriptorType::UniformBuffer, createInfo.LayoutSignature.UniformBufferCount, createInfo.MaxSet);
		PushPoolSize(poolSizes, sizeCount, Renderer::DescriptorType::StorageBuffer, createInfo.LayoutSignature.StorageBufferCount, createInfo.MaxSet);
		
		VkDescriptorPoolCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.maxSets = createInfo.MaxSet;
			vkCreateInfo.poolSizeCount = sizeCount;
			vkCreateInfo.pPoolSizes = poolSizes;
		}

		COCKTAIL_VK_CHECK(vkCreateDescriptorPool(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		DescriptorPool::SetObjectName(createInfo.Name);
	}

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void DescriptorPool::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_DESCRIPTOR_POOL;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	std::shared_ptr<Renderer::RenderDevice> DescriptorPool::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	void DescriptorPool::Reset() const
	{
		COCKTAIL_VK_CHECK(vkResetDescriptorPool(mRenderDevice->GetHandle(), mHandle, 0));
	}

	bool DescriptorPool::SupportFree() const
	{
		return mSupportFree;
	}

	VkDescriptorPool DescriptorPool::GetHandle() const
	{
		return mHandle;
	}

	void DescriptorPool::PushPoolSize(VkDescriptorPoolSize* sizes, unsigned int& index, Renderer::DescriptorType type, unsigned int descriptorCount, unsigned int setCount)
	{
		if (!descriptorCount)
			return;

		sizes[index] = { ToVkType(type), descriptorCount * setCount };
		++index;
	}
}
