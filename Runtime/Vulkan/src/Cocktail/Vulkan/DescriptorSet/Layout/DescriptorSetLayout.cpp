#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayout.hpp>

namespace Ck::Vulkan
{
	DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<RenderDevice> renderDevice, const DescriptorSetLayoutCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE),
		mSupportPushDescriptor(createInfo.SupportPushDescriptor)
	{
		mBindings.reserve(createInfo.BindingCount);
		for (std::size_t i = 0; i < createInfo.BindingCount; i++)
			mBindings.push_back(createInfo.Bindings[i]);

		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.reserve(mBindings.size());
		for (unsigned int i = 0; i < createInfo.BindingCount; i++)
		{
			VkDescriptorSetLayoutBinding binding;
			binding.binding = mBindings[i].Binding;
			binding.descriptorType = ToVkType(mBindings[i].Type);
			binding.descriptorCount = mBindings[i].DescriptorCount;
			binding.stageFlags = ToVkTypes(mBindings[i].ShaderStages);
			binding.pImmutableSamplers = nullptr;

			bindings.push_back(binding);
		}

		VkDescriptorSetLayoutCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			if (mSupportPushDescriptor)
				vkCreateInfo.flags |= VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR;

			vkCreateInfo.bindingCount = static_cast<unsigned int>(bindings.size());
			vkCreateInfo.pBindings = bindings.data();
		}

		COCKTAIL_VK_CHECK(vkCreateDescriptorSetLayout(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		DescriptorSetLayout::SetObjectName(createInfo.Name);
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void DescriptorSetLayout::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	std::shared_ptr<Renderer::RenderDevice> DescriptorSetLayout::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	bool DescriptorSetLayout::IsCompatibleWith(const DescriptorSetLayout& other) const
	{
		if (mBindings.size() != other.mBindings.size())
			return false;

		for (unsigned int i = 0; i < mBindings.size(); i++)
		{
			const DescriptorSetLayoutBinding* otherBinding = nullptr;
			for (unsigned int j = 0; j < mBindings.size() && !otherBinding; j++)
			{
				if (mBindings[i].Binding == other.mBindings[j].Binding)
					otherBinding = &other.mBindings[j];
			}

			if (!otherBinding)
				return false;

			if (mBindings[i].Type != otherBinding->Type)
				return false;

			if (mBindings[i].DescriptorCount != otherBinding->DescriptorCount)
				return false;

			if (mBindings[i].ShaderStages != otherBinding->ShaderStages)
				return false;
		}

		return true;
	}

	bool DescriptorSetLayout::SupportPushDescriptor() const
	{
		return mSupportPushDescriptor;
	}

	unsigned int DescriptorSetLayout::GetBindingCount() const
	{
		return static_cast<unsigned int>(mBindings.size());
	}

	unsigned int DescriptorSetLayout::GetDescriptorCount() const
	{
		unsigned int descriptorCount = 0;
		for (const DescriptorSetLayoutBinding& binding : mBindings)
			descriptorCount += binding.DescriptorCount;

		return descriptorCount;
	}

	const DescriptorSetLayoutBinding* DescriptorSetLayout::GetBinding(unsigned int index) const
	{
		assert(index < mBindings.size());
		return &mBindings[index];
	}

	DescriptorSetLayoutSignature DescriptorSetLayout::ToSignature() const
	{
		DescriptorSetLayoutSignature signature;
		signature.SupportPushDescriptor = mSupportPushDescriptor;

		for (const DescriptorSetLayoutBinding& binding : mBindings)
		{
			switch (binding.Type)
			{
			case Renderer::DescriptorType::Sampler:
				signature.SamplerCount += binding.DescriptorCount;
				break;

			case Renderer::DescriptorType::TextureSampler:
				signature.TextureSamplerCount += binding.DescriptorCount;
				break;

			case Renderer::DescriptorType::Texture:
				signature.TextureCount += binding.DescriptorCount;
				break;

			case Renderer::DescriptorType::StorageTexture:
				signature.StorageTextureCount += binding.DescriptorCount;
				break;

			case Renderer::DescriptorType::UniformBuffer: 
				signature.UniformBufferCount += binding.DescriptorCount;
				break;

			case Renderer::DescriptorType::StorageBuffer:
				signature.StorageBufferCount += binding.DescriptorCount;
				break;
			}
		}

		return signature;
	}

	VkDescriptorSetLayout DescriptorSetLayout::GetHandle() const
	{
		return mHandle;
	}
}
