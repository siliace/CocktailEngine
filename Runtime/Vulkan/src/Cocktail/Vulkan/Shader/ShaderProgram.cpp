#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Shader/ShaderProgram.hpp>
#include <Cocktail/Vulkan/Shader/UniformSlot.hpp>
#include <Cocktail/Vulkan/Shader/Reflection/DescriptorSetInfo.hpp>

namespace Ck::Vulkan
{
	namespace
	{
		void InsertBindingToLayoutCreateInfo(std::vector<DescriptorSetLayoutBinding>& layoutBindings, Renderer::ShaderType shaderType, const DescriptorSetBindingInfo& bindingInfo)
		{
			DescriptorSetLayoutBinding* foundLayoutBinding = nullptr;
			for (unsigned int i = 0; i < layoutBindings.size(); i++)
			{
				DescriptorSetLayoutBinding& layoutBinding = layoutBindings[i];
				if (bindingInfo.Binding == layoutBinding.Binding)
				{
					foundLayoutBinding = &layoutBinding;
					break;
				}
			}

			if (foundLayoutBinding)
			{
				assert(foundLayoutBinding->Type == bindingInfo.Type);
				assert(foundLayoutBinding->DescriptorCount == bindingInfo.ArrayLength);

				foundLayoutBinding->ShaderStages |= shaderType;
			}
			else
			{
				assert(bindingInfo.ArrayLength > 0);

				DescriptorSetLayoutBinding layoutBinding;
				layoutBinding.Binding = bindingInfo.Binding;
				layoutBinding.Type = bindingInfo.Type;
				layoutBinding.DescriptorCount = bindingInfo.ArrayLength;
				layoutBinding.ShaderStages = shaderType;


				layoutBindings.push_back(layoutBinding);
			}
		}
	}

	ShaderProgram::ShaderProgram(std::shared_ptr<RenderDevice> renderDevice, const Renderer::ShaderProgramCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks)
	{
		mType = createInfo.Type;

		for (unsigned int i = 0; i < createInfo.ShaderCount; i++)
		{
			std::shared_ptr<Shader> shader = std::static_pointer_cast<Shader>(createInfo.Shaders[i]);
			if (!shader)
				continue;

			assert(Renderer::ProgramSupportShader(mType, shader->GetType()));

			mShaders[shader->GetType()] = shader;
		}

		CreatePipelineLayout();

		CreateUniformSlots();

		ShaderProgram::SetObjectName(createInfo.Name);
	}

	void ShaderProgram::SetObjectName(const char* name) const
	{
	}

	std::shared_ptr<Renderer::RenderDevice> ShaderProgram::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Renderer::ShaderProgramType ShaderProgram::GetType() const
	{
		return mType;
	}

	std::shared_ptr<Renderer::Shader> ShaderProgram::GetStage(Renderer::ShaderType type) const
	{
		return mShaders[type];
	}

	Renderer::UniformSlot* ShaderProgram::FindUniformSlot(std::string_view name) const
	{	
		for (const std::unique_ptr<UniformSlot>& uniformSlot : mUniformSlots)
		{
			if (uniformSlot->GetName() == name)
				return uniformSlot.get();
		}

		return nullptr;
	}

	std::shared_ptr<PipelineLayout> ShaderProgram::GetPipelineLayout() const
	{
		return mPipelineLayout;
	}

	VkPipelineBindPoint BindPointFromProgramType(Renderer::ShaderProgramType programType)
	{
		switch (programType)
		{
			case Renderer::ShaderProgramType::Graphic:
				return VK_PIPELINE_BIND_POINT_GRAPHICS;

			case Renderer::ShaderProgramType::Compute:
				return VK_PIPELINE_BIND_POINT_COMPUTE;
		}

		COCKTAIL_UNREACHABLE();
		return {};
	}

	void ShaderProgram::CreatePipelineLayout()
	{
		static const DescriptorSetLayoutCreateInfo EmptyLayoutCreateInfo;

		PipelineLayoutCreateInfo pipelineLayoutCreateInfo;
		pipelineLayoutCreateInfo.BindPoint = BindPointFromProgramType(mType);

		std::map<unsigned int, DescriptorSetLayoutCreateInfo> descriptorSetLayoutsCreateInfo;
		std::map<unsigned int, std::vector<DescriptorSetLayoutBinding>> descriptorSetLayoutsBindings;

		for (Renderer::ShaderType shaderType : Enum<Renderer::ShaderType>::Values)
		{
			std::shared_ptr<Shader> shader = mShaders[shaderType];
			if (!shader)
				continue;

			if (shader->GetPushConstantBlockCount())
			{
				assert(shader->GetPushConstantBlockCount() == 1); /// For now we handle only one as glsl does (per stage)

				const PushConstantBlockInfo* pushConstantBlock = shader->GetPushConstantBlock(0);
				pipelineLayoutCreateInfo.PushConstantBlocks[shaderType] = Optional<PushConstantBlockInfo>::Of(*pushConstantBlock);
			}

			for (unsigned int i = 0; i < shader->GetDescriptorSetInfoCount(); i++)
			{
				const DescriptorSetInfo* descriptorSetInfo = shader->GetDescriptorSetInfo(i);
				DescriptorSetLayoutCreateInfo& layoutCreateInfo = descriptorSetLayoutsCreateInfo[descriptorSetInfo->Set];
				std::vector<DescriptorSetLayoutBinding>& layoutBindings = descriptorSetLayoutsBindings[descriptorSetInfo->Set];

				// Fill missing layouts with dummy values
				for (unsigned int j = 0; j < descriptorSetInfo->Set; j++)
				{
					if (descriptorSetLayoutsCreateInfo.find(j) == descriptorSetLayoutsCreateInfo.end())
						descriptorSetLayoutsCreateInfo[j] = EmptyLayoutCreateInfo;
				}

				for (const DescriptorSetBindingInfo& binding : descriptorSetInfo->Bindings)
					InsertBindingToLayoutCreateInfo(layoutBindings, shaderType, binding);
			}
		}

		if (mRenderDevice->IsFeatureSupported(RenderDeviceFeature::PushDescriptors) && descriptorSetLayoutsCreateInfo.size() == 1)
		{
			VkPhysicalDevicePushDescriptorPropertiesKHR pushDescriptorProperties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PUSH_DESCRIPTOR_PROPERTIES_KHR, nullptr };
			VkPhysicalDeviceProperties2KHR properties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR, &pushDescriptorProperties };

			VkPhysicalDevice physicalDevice = mRenderDevice->GetPhysicalDeviceHandle();
			vkGetPhysicalDeviceProperties2KHR(physicalDevice, &properties);

			auto& [descriptorSetIndex, layoutCreateInfo] = *descriptorSetLayoutsCreateInfo.begin();
			layoutCreateInfo.SupportPushDescriptor = layoutCreateInfo.BindingCount <= pushDescriptorProperties.maxPushDescriptors;
		}

		for (auto&[descriptorSetIndex, layoutCreateInfo] : descriptorSetLayoutsCreateInfo)
		{
			std::vector<DescriptorSetLayoutBinding>& layoutBindings = descriptorSetLayoutsBindings[descriptorSetIndex];

			layoutCreateInfo.BindingCount = static_cast<unsigned>(layoutBindings.size());
			layoutCreateInfo.Bindings = layoutBindings.data();

			std::shared_ptr<DescriptorSetLayout> descriptorSetLayout = mRenderDevice->CreateDescriptorSetLayout(layoutCreateInfo);
			pipelineLayoutCreateInfo.DescriptorSetLayouts.push_back(descriptorSetLayout);
		}

		mPipelineLayout = mRenderDevice->CreatePipelineLayout(pipelineLayoutCreateInfo);
	}

	void ShaderProgram::CreateUniformSlots()
	{
		for (unsigned int i = 0; i < mPipelineLayout->GetDescriptorSetLayoutCount(); i++)
		{
			std::shared_ptr<DescriptorSetLayout> descriptorSetLayout = mPipelineLayout->GetDescriptorSetLayout(i);

			for (unsigned int j = 0; j < descriptorSetLayout->GetBindingCount(); j++)
			{
				const DescriptorSetLayoutBinding* binding = descriptorSetLayout->GetBinding(j);

				// Get name of the uniform, keep the first one found.
				std::string name;
				std::vector<BlockMember> members;
				for (Renderer::ShaderType type : Enum<Renderer::ShaderType>::Values)
				{
					std::shared_ptr<Shader> shader = mShaders[type];
					if (!shader)
						continue;

					Optional<DescriptorSetBindingInfo> bindingInfo = shader->GetDescriptorSetBindingInfo(i, binding->Binding);
					if (bindingInfo.IsEmpty())
						continue;

					name = bindingInfo.Get().Name;
					members = bindingInfo.Get().Members;
					break;
				}

				mUniformSlots.emplace_back(new UniformSlot(mType, members, std::move(name), *binding, i));
			}
		}
	}
}
