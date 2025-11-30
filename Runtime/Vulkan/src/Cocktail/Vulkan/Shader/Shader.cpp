#include <spirv_reflect.h>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Shader/Shader.hpp>
#include <Cocktail/Vulkan/Shader/ValidationCache.hpp>
#include <Cocktail/Vulkan/Shader/VertexAttributeLocation.hpp>

namespace Ck::Vulkan
{
	namespace
	{
		Renderer::DescriptorType ToCkType(SpvReflectDescriptorType descriptorType)
		{
			switch (descriptorType)
			{
			case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER: 
				return Renderer::DescriptorType::Sampler;

			case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
				return Renderer::DescriptorType::TextureSampler;

			case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
				return Renderer::DescriptorType::Texture;

			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE:
				return Renderer::DescriptorType::StorageTexture;

			case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				return Renderer::DescriptorType::UniformBuffer;

			case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER:
				return Renderer::DescriptorType::StorageBuffer;
			}

			COCKTAIL_UNREACHABLE();
			return {};
		}

		DataType GetReflectFormatDataType(SpvReflectFormat reflectFormat)
		{
			switch (reflectFormat)
			{
			case SPV_REFLECT_FORMAT_R32_UINT:
			case SPV_REFLECT_FORMAT_R32G32_UINT:
			case SPV_REFLECT_FORMAT_R32G32B32_UINT:
			case SPV_REFLECT_FORMAT_R32G32B32A32_UINT:
				return DataType::UnsignedInt32;

			case SPV_REFLECT_FORMAT_R32_SINT:
			case SPV_REFLECT_FORMAT_R32G32_SINT:
			case SPV_REFLECT_FORMAT_R32G32B32_SINT:
			case SPV_REFLECT_FORMAT_R32G32B32A32_SINT:
				return DataType::Int32;

			case SPV_REFLECT_FORMAT_R32_SFLOAT:
			case SPV_REFLECT_FORMAT_R32G32_SFLOAT:
			case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT:
			case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT:
				return DataType::Float32;

			case SPV_REFLECT_FORMAT_R64_UINT:
			case SPV_REFLECT_FORMAT_R64G64_UINT:
			case SPV_REFLECT_FORMAT_R64G64B64_UINT:
			case SPV_REFLECT_FORMAT_R64G64B64A64_UINT:
				return DataType::UnsignedInt64;

			case SPV_REFLECT_FORMAT_R64_SINT:
			case SPV_REFLECT_FORMAT_R64G64_SINT:
			case SPV_REFLECT_FORMAT_R64G64B64_SINT:
			case SPV_REFLECT_FORMAT_R64G64B64A64_SINT:
				return DataType::Int64;

			case SPV_REFLECT_FORMAT_R64_SFLOAT:
			case SPV_REFLECT_FORMAT_R64G64_SFLOAT:
			case SPV_REFLECT_FORMAT_R64G64B64_SFLOAT:
			case SPV_REFLECT_FORMAT_R64G64B64A64_SFLOAT:
				return DataType::Float64;
			}

			COCKTAIL_UNREACHABLE();
			return {};
		}

		DataType ReflectDataType(SpvReflectTypeFlags typeFlags, const SpvReflectNumericTraits& numericTraits)
		{
			const std::size_t size = numericTraits.scalar.width / 8;
			const bool isFloatingPoint = typeFlags & SpvReflectTypeFlagBits::SPV_REFLECT_TYPE_FLAG_FLOAT;
			const bool isUnsigned = numericTraits.scalar.signedness != 0;

			return DataType(size, isFloatingPoint, isUnsigned);
		}

		unsigned int ReflectElementCount(const SpvReflectNumericTraits& numericTraits)
		{
			const SpvReflectNumericTraits::Matrix& matrixTrait = numericTraits.matrix;
			const SpvReflectNumericTraits::Vector& vectorTrait = numericTraits.vector;
			if (matrixTrait.column_count && matrixTrait.row_count)
				return matrixTrait.column_count * matrixTrait.row_count;

			if (vectorTrait.component_count)
				return vectorTrait.component_count;

			return 1;
		}

		UniquePtr<VertexAttributeLocation> ReflectInterfaceVariable(const SpvReflectInterfaceVariable* reflectInterfaceVariable)
		{
			unsigned int location = reflectInterfaceVariable->location;
			AsciiString name = reflectInterfaceVariable->name;
			unsigned int elementCount = ReflectElementCount(reflectInterfaceVariable->numeric);

			return MakeUnique<VertexAttributeLocation>(location, GetReflectFormatDataType(reflectInterfaceVariable->format), elementCount, std::move(name));
		}

		BlockMember ReflectBlockMember(const SpvReflectBlockVariable& blockVariable)
		{
			BlockMember member;
			member.Offset = blockVariable.offset;
			member.DataType = ReflectDataType(blockVariable.type_description->type_flags, blockVariable.numeric);
			member.ElementCount = ReflectElementCount(blockVariable.numeric);
			member.Name = blockVariable.name;
			member.ArrayLength = 1;

			return member;
		}
	}

	Shader::Shader(RenderDevice* renderDevice, const ValidationCache* validationCache, const Renderer::ShaderCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		mType = createInfo.Type;

		VkShaderModuleCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, nullptr };
		VkShaderModuleValidationCacheCreateInfoEXT validationCacheCreateInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_VALIDATION_CACHE_CREATE_INFO_EXT, nullptr };
		{
			if (mRenderDevice->IsFeatureSupported(RenderDeviceFeature::ValidationCache) && validationCache)
			{
				Chain(vkCreateInfo, validationCacheCreateInfo);
				validationCacheCreateInfo.validationCache = validationCache->GetHandle();
			}

			vkCreateInfo.flags = 0;
			vkCreateInfo.codeSize = createInfo.Code.GetSize();
			vkCreateInfo.pCode = reinterpret_cast<const Uint32*>(createInfo.Code.GetData());
		}
		 
		COCKTAIL_VK_CHECK(vkCreateShaderModule(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		Shader::SetObjectName(createInfo.Name);

		SpvReflectShaderModule module;
		SpvReflectResult result = spvReflectCreateShaderModule(vkCreateInfo.codeSize, vkCreateInfo.pCode, &module);
		assert(result == SPV_REFLECT_RESULT_SUCCESS);

		// Reflect descriptor sets
		{
			unsigned int reflectDescriptorSetCount;
			result = spvReflectEnumerateDescriptorSets(&module, &reflectDescriptorSetCount, nullptr);
			assert(result == SPV_REFLECT_RESULT_SUCCESS);

			if (reflectDescriptorSetCount)
			{
				Array<SpvReflectDescriptorSet*> reflectDescriptorSets(reflectDescriptorSetCount);
				result = spvReflectEnumerateDescriptorSets(&module, &reflectDescriptorSetCount, reflectDescriptorSets.GetData());
				assert(result == SPV_REFLECT_RESULT_SUCCESS);

				mDescriptorSetInfo = Array<DescriptorSetInfo>(reflectDescriptorSetCount);
				for (unsigned int i = 0; i < mDescriptorSetInfo.GetSize(); i++)
				{
					DescriptorSetInfo descriptorSetInfo;
					SpvReflectDescriptorSet* reflectDescriptorSet = reflectDescriptorSets[i];

					descriptorSetInfo.Set = reflectDescriptorSet->set;
					descriptorSetInfo.Bindings = Array<DescriptorSetBindingInfo>(reflectDescriptorSet->binding_count);
					for (unsigned int j = 0; j < reflectDescriptorSet->binding_count; j++)
					{
						SpvReflectDescriptorBinding* reflectBinding = reflectDescriptorSet->bindings[j];
						Renderer::DescriptorType descriptorType = ToCkType(reflectBinding->descriptor_type);

						descriptorSetInfo.Bindings[j].Binding = reflectBinding->binding;
						descriptorSetInfo.Bindings[j].Type = descriptorType;

						if (descriptorType == Renderer::DescriptorType::UniformBuffer || descriptorType == Renderer::DescriptorType::StorageBuffer)
						{
							Array<BlockMember> members(reflectBinding->block.member_count);
							for (unsigned int k = 0; k < members.GetSize(); k++)
								members[k] = ReflectBlockMember(reflectBinding->block.members[k]);

							descriptorSetInfo.Bindings[j].Members = std::move(members);
						}

						if (reflectBinding->array.dims_count)
						{
							descriptorSetInfo.Bindings[j].ArrayLength = reflectBinding->array.dims[0];
						}
						else
						{
							descriptorSetInfo.Bindings[j].ArrayLength = 1;
						}

						descriptorSetInfo.Bindings[j].Name = reflectBinding->name;
					}

					mDescriptorSetInfo[i] = descriptorSetInfo;
				}
			}
		}

		// Reflect push constant
		{
			unsigned int reflectPushConstantBlockCount;
			result = spvReflectEnumeratePushConstantBlocks(&module, &reflectPushConstantBlockCount, nullptr);
			assert(result == SPV_REFLECT_RESULT_SUCCESS);

			if (reflectPushConstantBlockCount)
			{
				Array<SpvReflectBlockVariable*> reflectPushConstantBlocks(reflectPushConstantBlockCount);
				result = spvReflectEnumeratePushConstantBlocks(&module, &reflectPushConstantBlockCount, reflectPushConstantBlocks.GetData());
				assert(result == SPV_REFLECT_RESULT_SUCCESS);

				mPushConstantBlockInfo.Resize(reflectPushConstantBlockCount);
				for (unsigned int i = 0; i < mPushConstantBlockInfo.GetSize(); i++)
				{
					PushConstantBlockInfo pushConstantBlockInfo;
					SpvReflectBlockVariable* reflectPushConstantBlock = reflectPushConstantBlocks[i];

					Array<BlockMember> members(reflectPushConstantBlock->member_count);
					for (unsigned int j = 0; j < members.GetSize(); j++)
						members[j] = ReflectBlockMember(reflectPushConstantBlock->members[j]);

					pushConstantBlockInfo.Name = reflectPushConstantBlock->name;
					pushConstantBlockInfo.Members = std::move(members);
					pushConstantBlockInfo.BaseOffset = pushConstantBlockInfo.Members[0].Offset;
					pushConstantBlockInfo.Size = reflectPushConstantBlock->size - reflectPushConstantBlock->offset;

					mPushConstantBlockInfo[i] = pushConstantBlockInfo;
				}
			}
		}

		// Reflect input vertex attributes
		{
			unsigned int reflectInputVertexAttributeCount;
			result = spvReflectEnumerateInputVariables(&module, &reflectInputVertexAttributeCount, nullptr);
			assert(result == SPV_REFLECT_RESULT_SUCCESS);

			if (reflectInputVertexAttributeCount)
			{
				Array<SpvReflectInterfaceVariable*> reflectInputVariables(reflectInputVertexAttributeCount);
				result = spvReflectEnumerateInputVariables(&module, &reflectInputVertexAttributeCount, reflectInputVariables.GetData());
				assert(result == SPV_REFLECT_RESULT_SUCCESS);

				mInputAttributeLocations.Resize(reflectInputVertexAttributeCount);
				for (unsigned int i = 0; i < reflectInputVertexAttributeCount; i++)
				{
					if (reflectInputVariables[i]->location == static_cast<unsigned int>(-1))
						continue;

					mInputAttributeLocations[i] = ReflectInterfaceVariable(reflectInputVariables[i]);
				}
			}
		}

		// Reflect output vertex attributes
		{
			unsigned int reflectOutputVertexAttributeCount;
			result = spvReflectEnumerateOutputVariables(&module, &reflectOutputVertexAttributeCount, nullptr);
			assert(result == SPV_REFLECT_RESULT_SUCCESS);

			if (reflectOutputVertexAttributeCount)
			{
				Array<SpvReflectInterfaceVariable*> reflectOutputVariables(reflectOutputVertexAttributeCount);
				result = spvReflectEnumerateOutputVariables(&module, &reflectOutputVertexAttributeCount, &reflectOutputVariables[0]);
				assert(result == SPV_REFLECT_RESULT_SUCCESS);

				mOutputAttributeLocations.Resize(reflectOutputVertexAttributeCount);
				for (unsigned int i = 0; i < reflectOutputVertexAttributeCount; i++)
				{
					if (reflectOutputVariables[i]->location == static_cast<unsigned int>(-1))
						continue;

					mOutputAttributeLocations[i] = ReflectInterfaceVariable(reflectOutputVariables[i]);
				}
			}
		}

		spvReflectDestroyShaderModule(&module);
	}

	Shader::~Shader()
	{
		vkDestroyShaderModule(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void Shader::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_SHADER_MODULE;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Renderer::RenderDevice* Shader::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Renderer::ShaderType Shader::GetType() const
	{
		return mType;
	}

	Renderer::VertexAttributeLocation* Shader::FindInputAttribute(AsciiStringView name) const
	{
		for (const UniquePtr<VertexAttributeLocation>& vertexAttributeLocation : mInputAttributeLocations)
		{
			if (vertexAttributeLocation->GetName() == name)
				return vertexAttributeLocation.Get();
		}

		return nullptr;
	}

	Renderer::VertexAttributeLocation* Shader::FindOutputAttribute(AsciiStringView name) const
	{
		for (const UniquePtr<VertexAttributeLocation>& vertexAttributeLocation : mOutputAttributeLocations)
		{
			if (vertexAttributeLocation->GetName() == name)
				return vertexAttributeLocation.Get();
		}

		return nullptr;
	}

	unsigned int Shader::GetDescriptorSetInfoCount() const
	{
		return mDescriptorSetInfo.GetSize();
	}

	const DescriptorSetInfo* Shader::GetDescriptorSetInfo(unsigned int index) const
	{
		return &mDescriptorSetInfo[index];
	}

	unsigned Shader::GetPushConstantBlockCount() const
	{
		return mPushConstantBlockInfo.GetSize();
	}

	const PushConstantBlockInfo* Shader::GetPushConstantBlock(unsigned index) const
	{
		return &mPushConstantBlockInfo[index];
	}

	Optional<DescriptorSetBindingInfo> Shader::GetDescriptorSetBindingInfo(unsigned int set, unsigned int binding) const
	{
		for (const DescriptorSetInfo& descriptorSetInfo : mDescriptorSetInfo)
		{
			if (descriptorSetInfo.Set != set)
				continue;

			for (const DescriptorSetBindingInfo& descriptorSetBindingInfo : descriptorSetInfo.Bindings)
			{
				if (descriptorSetBindingInfo.Binding == binding)
					return Optional<DescriptorSetBindingInfo>::Of(descriptorSetBindingInfo);
			}
		}

		return Optional<DescriptorSetBindingInfo>::Empty();
	}

	VkShaderModule Shader::GetHandle() const
	{
		return mHandle;
	}
	
}
