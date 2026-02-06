#include <cstring>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorSet.hpp>
#include <Cocktail/Vulkan/Pipeline/State/StateManager.hpp>
#include <Cocktail/Vulkan/Shader/ShaderProgram.hpp>

namespace Ck::Vulkan
{
	StateManager::StateManager(RenderDevice* renderDevice, DescriptorSetAllocator* descriptorSetAllocator) :
		mRenderDevice(std::move(renderDevice)),
		mDescriptorSetAllocator(std::move(descriptorSetAllocator)),
		mDescriptorSetDirtyFlags(0)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;

		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(mRenderDevice->GetPhysicalDeviceHandle(), &physicalDeviceProperties);

		mDescriptorSetStateManagers.Reserve(physicalDeviceProperties.limits.maxBoundDescriptorSets);
		for (unsigned int i = 0; i < physicalDeviceProperties.limits.maxBoundDescriptorSets; i++)
			mDescriptorSetStateManagers.Emplace(mRenderDevice, mDescriptorSetAllocator);

		mPipelineConstantStorageSize = physicalDeviceProperties.limits.maxPushConstantsSize;
		mPipelineConstantStorage = MakeUnique<unsigned char[]>(mPipelineConstantStorageSize);
	}

	void StateManager::SetShaderProgram(const ShaderProgram* shaderProgram)
	{
		for (Renderer::ShaderType shaderType : Enum<Renderer::ShaderType>::Values)
			mShaderStages[shaderType] = std::static_pointer_cast<Shader>(shaderProgram->GetStage(shaderType));
	}

	void StateManager::BindSampler(unsigned int set, unsigned int binding, Flags<Renderer::ShaderType> shaderStages, unsigned int arrayIndex, const Sampler* sampler)
	{
		bool dirty = mDescriptorSetStateManagers[set].BindSampler(binding, shaderStages, arrayIndex, sampler);
		if (dirty)
		{
			mDescriptorSetDirtyFlags |= Bit(set);
			mDirtyFlags |= DirtyFlagBits::DescriptorSet;
		}
	}

	void StateManager::BindTextureSampler(unsigned int set, unsigned int binding, Flags<Renderer::ShaderType> shaderStages, unsigned int arrayIndex, const TextureView* textureView, const Sampler* sampler)
	{
		bool dirty = mDescriptorSetStateManagers[set].BindTextureSampler(binding, shaderStages, arrayIndex, textureView, sampler);
		if (dirty)
		{
			mDescriptorSetDirtyFlags |= Bit(set);
			mDirtyFlags |= DirtyFlagBits::DescriptorSet;
		}
	}

	void StateManager::BindTexture(unsigned int set, unsigned int binding, Flags<Renderer::ShaderType> shaderStages, unsigned int arrayIndex, const TextureView* textureView)
	{
		bool dirty = mDescriptorSetStateManagers[set].BindTexture(binding, shaderStages, arrayIndex, textureView);
		if (dirty)
		{
			mDescriptorSetDirtyFlags |= Bit(set);
			mDirtyFlags |= DirtyFlagBits::DescriptorSet;
		}
	}

	void StateManager::BindStorageTexture(unsigned int set, unsigned int binding, Flags<Renderer::ShaderType> shaderStages, unsigned int arrayIndex, const TextureView* textureView)
	{
		bool dirty = mDescriptorSetStateManagers[set].BindStorageTexture(binding, shaderStages, arrayIndex, textureView);
		if (dirty)
		{
			mDescriptorSetDirtyFlags |= Bit(set);
			mDirtyFlags |= DirtyFlagBits::DescriptorSet;
		}
	}

	void StateManager::BindUniformBuffer(unsigned int set, unsigned int binding, Flags<Renderer::ShaderType> shaderStages, unsigned int arrayIndex, const Buffer* uniformBuffer, std::size_t offset, std::size_t range)
	{
		bool dirty = mDescriptorSetStateManagers[set].BindUniformBuffer(binding, shaderStages, arrayIndex, uniformBuffer, offset, range);
		if (dirty)
		{
			mDescriptorSetDirtyFlags |= Bit(set);
			mDirtyFlags |= DirtyFlagBits::DescriptorSet;
		}
	}

	void StateManager::BindStorageBuffer(unsigned int set, unsigned int binding, Flags<Renderer::ShaderType> shaderStages, unsigned int arrayIndex, const Buffer* uniformBuffer, std::size_t offset, std::size_t range)
	{
		bool dirty = mDescriptorSetStateManagers[set].BindStorageBuffer(binding, shaderStages, arrayIndex, uniformBuffer, offset, range);
		if (dirty)
		{
			mDescriptorSetDirtyFlags |= Bit(set);
			mDirtyFlags |= DirtyFlagBits::DescriptorSet;
		}
	}

	void StateManager::UpdatePipelineConstant(Renderer::ShaderType shaderType, unsigned int offset, unsigned int size, const void* data)
	{
		if (std::shared_ptr<Shader> shader = mShaderStages[shaderType])
		{
			if (const PushConstantBlockInfo* pushConstantBlock = shader->GetPushConstantBlock(0))
			{
				offset += pushConstantBlock->BaseOffset;
				assert(offset + size < mPipelineConstantStorageSize);

				mDirtyFlags |= DirtyFlagBits::PipelineConstant;
				Memory::Copy(mPipelineConstantStorage.Get() + offset, data, size);

				PipelineConstantRangeState& range = mPipelineConstantRanges[shaderType];
				if (range.Dirty)
				{
					range.Offset = std::min(range.Offset, offset);
					range.Size = std::max(range.Size, offset + size);
				}
				else
				{
					range.Offset = offset;
					range.Size = size;
					range.Dirty = true;
				}
			}
		}
	}

	void StateManager::ResetBindings(unsigned int firstSet)
	{
	    for (unsigned int i = firstSet; i < mDescriptorSetStateManagers.GetSize(); i++)
		    mDescriptorSetStateManagers[i].ResetBindings();
	}

	void StateManager::ResetBindings()
	{
		for (DescriptorSetStateManager& descriptorSetStateManager : mDescriptorSetStateManagers)
			descriptorSetStateManager.ResetBindings();
	}

	PipelineConstantRange StateManager::CompilePipelineConstants(Renderer::ShaderType shaderType)
	{
		PipelineConstantRangeState& range = mPipelineConstantRanges[shaderType];
		range.Dirty = false;

		PipelineConstantRange pipelineConstantRange;
		pipelineConstantRange.Offset = range.Offset;
		pipelineConstantRange.Size = range.Size;
		pipelineConstantRange.Data = mPipelineConstantStorage.Get() + range.Offset;

		return pipelineConstantRange;
	}

	unsigned int StateManager::CompilePushDescriptors(std::shared_ptr<DescriptorSetLayout> setLayout, unsigned int set, VkDescriptorImageInfo* imagesInfo, VkDescriptorBufferInfo* buffersInfo, VkWriteDescriptorSet* writes)
	{
		unsigned int descriptorWriteCount = 0;
		if (IsDescriptorSetDirty(set))
		{
			descriptorWriteCount = mDescriptorSetStateManagers[set].CompileDescriptors(setLayout, imagesInfo, buffersInfo, writes);
			
			mDescriptorSetDirtyFlags &= ~Bit(set);
		}

		if (!mDescriptorSetDirtyFlags)
			mDirtyFlags &= ~DirtyFlagBits::DescriptorSet;

		return descriptorWriteCount;
	}

	void StateManager::CompilePushDescriptorsWithTemplate(std::shared_ptr<DescriptorSetLayout> setLayout, std::shared_ptr<DescriptorUpdateTemplate> descriptorUpdateTemplate, unsigned int set, unsigned char* descriptors)
	{
		if (IsDescriptorSetDirty(set))
		{
			mDescriptorSetStateManagers[set].CompileDescriptorsWithTemplate(setLayout, descriptorUpdateTemplate, descriptors);

			mDescriptorSetDirtyFlags &= ~Bit(set);
		}

		if (!mDescriptorSetDirtyFlags)
			mDirtyFlags &= ~DirtyFlagBits::DescriptorSet;
	}

	DescriptorSetRange StateManager::CompileDescriptorSets(std::shared_ptr<PipelineLayout> pipelineLayout, unsigned int set)
	{
		unsigned int descriptorSetCount = 0;
		DescriptorSetRange range;

		while (IsDescriptorSetDirty(set))
		{
			mDescriptorSetDirtyFlags &= ~Bit(set);

			const std::shared_ptr<DescriptorSetLayout> setLayout = pipelineLayout->GetDescriptorSetLayout(set);
			if (setLayout->SupportPushDescriptor())
				continue;

			std::shared_ptr<DescriptorSet> descriptorSet;
			DescriptorSetStateManager& stateManager = mDescriptorSetStateManagers[set];
			if (std::shared_ptr<DescriptorUpdateTemplate> descriptorUpdateTemplate = pipelineLayout->GetDescriptorUpdateTemplate(set))
			{
				descriptorSet = stateManager.CompileSetWithTemplate(setLayout, descriptorUpdateTemplate);
			}
			else
			{
				descriptorSet = stateManager.CompileSet(setLayout);
			}
			range.DescriptorSetHandles[descriptorSetCount] = descriptorSet->GetHandle();
			
			++set;
			++descriptorSetCount;
		}

		range.DescriptorSetCount = descriptorSetCount;

		if (!mDescriptorSetDirtyFlags)
			mDirtyFlags &= ~DirtyFlagBits::DescriptorSet;

		return range;
	}

	bool StateManager::IsDirty(DirtyFlagBits flag) const
	{
		return mDirtyFlags & flag;
	}

	bool StateManager::IsDescriptorSetDirty(unsigned int set) const
	{
		return mDescriptorSetDirtyFlags & Bit(set);
	}

	bool StateManager::IsDirtyPipelineConstants(Renderer::ShaderType shaderType) const
	{
		return mPipelineConstantRanges[shaderType].Dirty;
	}
}
