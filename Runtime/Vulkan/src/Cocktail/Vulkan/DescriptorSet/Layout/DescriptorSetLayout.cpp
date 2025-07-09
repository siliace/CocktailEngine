#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayout.hpp>
#include <Cocktail/Vulkan/Texture/Sampler.hpp>

namespace Ck::Vulkan
{
	DescriptorSetLayout::DescriptorSetLayout(std::shared_ptr<RenderDevice> renderDevice, const DescriptorSetLayoutCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE),
		mSupportPushDescriptor(createInfo.SupportPushDescriptor)
	{
		mBindings.Append(createInfo.Bindings, createInfo.BindingCount);

		VkSampler* staticSamplerHandles = COCKTAIL_STACK_ALLOC(VkSampler, createInfo.BindingCount);
		for (unsigned int i = 0; i < createInfo.BindingCount; i++)
		{
			const Sampler* sampler = createInfo.Bindings[i].StaticSampler;
			staticSamplerHandles[i] = sampler ? sampler->GetHandle() : VK_NULL_HANDLE;
		}

		Array<VkDescriptorSetLayoutBinding> bindings = mBindings.Transform([&](const DescriptorSetLayoutBinding& layoutBinding, unsigned int index) -> VkDescriptorSetLayoutBinding {
			VkDescriptorSetLayoutBinding binding;
			binding.binding = layoutBinding.Binding;
			binding.descriptorType = ToVkType(layoutBinding.Type);
			binding.descriptorCount = layoutBinding.DescriptorCount;
			binding.stageFlags = ToVkTypes(layoutBinding.ShaderStages);
			binding.pImmutableSamplers = staticSamplerHandles[index] != VK_NULL_HANDLE ? &staticSamplerHandles[index] : nullptr;

			return binding;
		});

		VkDescriptorSetLayoutCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			if (mSupportPushDescriptor)
				vkCreateInfo.flags |= VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR;

			vkCreateInfo.bindingCount = bindings.GetSize();
			vkCreateInfo.pBindings = bindings.GetData();
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
		if (mBindings.GetSize() != other.mBindings.GetSize())
			return false;

		for (unsigned int i = 0; i < mBindings.GetSize(); i++)
		{
			const DescriptorSetLayoutBinding* otherBinding = nullptr;
			for (unsigned int j = 0; j < mBindings.GetSize() && !otherBinding; j++)
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

	unsigned int DescriptorSetLayout::GetDescriptorCount() const
	{
		return mBindings.Reduce(0, [](unsigned int current, const DescriptorSetLayoutBinding& binding) {
			return current + binding.DescriptorCount;
		});
	}

	const Array<DescriptorSetLayoutBinding>& DescriptorSetLayout::GetBindings() const
	{
		return mBindings;
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
