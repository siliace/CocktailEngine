#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorSet.hpp>
#include <Cocktail/Vulkan/DescriptorSet/State/DescriptorSetStateManager.hpp>

namespace Ck::Vulkan
{
	DescriptorSetStateManager::DescriptorSetStateManager(std::shared_ptr<RenderDevice> renderDevice, DescriptorSetAllocator* descriptorSetAllocator) :
		mRenderDevice(std::move(renderDevice)),
		mDescriptorSetAllocator(std::move(descriptorSetAllocator)),
		mBindingDirtyFlags(0)
	{
		/// Nothing
	}

	bool DescriptorSetStateManager::BindSampler(unsigned int binding, unsigned int arrayIndex, const Sampler* sampler)
	{
		DescriptorState* state = FindOrCreateDescriptorState(binding, arrayIndex);
		bool dirty = FillImageState(state, Renderer::DescriptorType::Sampler, nullptr, sampler);

		if (dirty)
			mBindingDirtyFlags |= Bit(binding);

		return dirty;
	}

	bool DescriptorSetStateManager::BindTextureSampler(unsigned int binding, unsigned int arrayIndex, const TextureView* textureView, const Sampler* sampler)
	{
		DescriptorState* state = FindOrCreateDescriptorState(binding, arrayIndex);
 		bool dirty = FillImageState(state, Renderer::DescriptorType::TextureSampler, textureView, sampler);

		if (dirty)
			mBindingDirtyFlags |= Bit(binding);

		return dirty;
	}

	bool DescriptorSetStateManager::BindTexture(unsigned int binding, unsigned int arrayIndex, const TextureView* textureView)
	{
		DescriptorState* state = FindOrCreateDescriptorState(binding, arrayIndex);
		bool dirty = FillImageState(state, Renderer::DescriptorType::Texture, textureView, nullptr);

		if (dirty)
			mBindingDirtyFlags |= Bit(binding);

		return dirty;
	}

	bool DescriptorSetStateManager::BindStorageTexture(unsigned int binding, unsigned int arrayIndex, const TextureView* textureView)
	{
		DescriptorState* state = FindOrCreateDescriptorState(binding, arrayIndex);
		bool dirty = FillImageState(state, Renderer::DescriptorType::StorageTexture, textureView, nullptr);

		if (dirty)
			mBindingDirtyFlags |= Bit(binding);

		return dirty;
	}

	bool DescriptorSetStateManager::BindUniformBuffer(unsigned int binding, unsigned int arrayIndex, const Buffer* buffer, std::size_t offset, std::size_t range)
	{
		DescriptorState* state = FindOrCreateDescriptorState(binding, arrayIndex);
		bool dirty = FillBufferState(state, Renderer::DescriptorType::UniformBuffer, buffer, offset, range);

		if (dirty)
			mBindingDirtyFlags |= Bit(binding);

		return dirty;
	}

	bool DescriptorSetStateManager::BindStorageBuffer(unsigned int binding, unsigned int arrayIndex, const Buffer* buffer, std::size_t offset, std::size_t range)
	{
		DescriptorState* state = FindOrCreateDescriptorState(binding, arrayIndex);
		bool dirty = FillBufferState(state, Renderer::DescriptorType::StorageBuffer, buffer, offset, range);

		if (dirty)
			mBindingDirtyFlags |= Bit(binding);

		return dirty;
	}

	void DescriptorSetStateManager::ResetBindings()
	{
		mBindingStates.clear();
		mBindingDirtyFlags = 0;
		mDescriptorSetAllocator->Reset();
	}

	unsigned int DescriptorSetStateManager::CompileDescriptors(std::shared_ptr<DescriptorSetLayout> layout, VkDescriptorImageInfo* imagesInfo, VkDescriptorBufferInfo* buffersInfo, VkWriteDescriptorSet* writes)
	{
		unsigned int descriptorCount = 0;
		unsigned int descriptorImageCount = 0;
		unsigned int descriptorBufferCount = 0;

		for (unsigned int i = 0; i < layout->GetBindingCount(); i++)
		{
			if (!IsBindingDirty(i))
				continue;

			const DescriptorSetLayoutBinding* layoutBinding = layout->GetBinding(i);

			for (unsigned arrayElement = 0; arrayElement < layoutBinding->DescriptorCount; arrayElement++)
			{
				DescriptorState* descriptorState = FindDescriptorState(layoutBinding->Binding, arrayElement);
				if (!descriptorState)
					throw std::runtime_error("Missing state in descriptor set compilation");

				if (!descriptorState->Dirty)
					continue;

				descriptorState->Dirty = false;

				VkWriteDescriptorSet& write = writes[descriptorCount];
				write = VkWriteDescriptorSet{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr};
				{
					write.dstSet = VK_NULL_HANDLE;
					write.dstBinding = descriptorState->Binding;
					write.dstArrayElement = descriptorState->ArrayElement;
					write.descriptorCount = 1;
					write.descriptorType = ToVkType(descriptorState->Type);
				}

				switch (descriptorState->Type)
				{
					case Renderer::DescriptorType::Sampler:
					case Renderer::DescriptorType::TextureSampler:
					case Renderer::DescriptorType::Texture:
					case Renderer::DescriptorType::StorageTexture:
						{
							const Sampler* sampler = descriptorState->ImageInfo.Sampler;
							const TextureView* textureView = descriptorState->ImageInfo.TextureView;

							VkDescriptorImageInfo& imageInfo = imagesInfo[descriptorImageCount];
							imageInfo.sampler = sampler ? sampler->GetHandle() : VK_NULL_HANDLE;
							imageInfo.imageView = textureView ? textureView->GetHandle() : VK_NULL_HANDLE;
							imageInfo.imageLayout = descriptorState->Type == Renderer::DescriptorType::StorageTexture ? VK_IMAGE_LAYOUT_GENERAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

							write.pImageInfo = &imageInfo;
							++descriptorImageCount;
						}
						break;

					case Renderer::DescriptorType::UniformBuffer:
					case Renderer::DescriptorType::StorageBuffer:
						{
							const Buffer* buffer = descriptorState->BufferInfo.Buffer;

							VkDescriptorBufferInfo& bufferInfo = buffersInfo[descriptorBufferCount];
							bufferInfo.buffer = buffer->GetHandle();
							bufferInfo.offset = descriptorState->BufferInfo.Offset;
							bufferInfo.range = descriptorState->BufferInfo.Range;

							write.pBufferInfo = &bufferInfo;
							++descriptorBufferCount;
						}
						break;
				}

				++descriptorCount;
			}
		}

		return descriptorCount;
	}

	void DescriptorSetStateManager::CompileDescriptorsWithTemplate(std::shared_ptr<DescriptorSetLayout> layout, std::shared_ptr<DescriptorUpdateTemplate> descriptorUpdateTemplate, unsigned char* descriptors)
	{
		unsigned int k = 0;
		const std::size_t elementStride = descriptorUpdateTemplate->GetElementStride();

		for (unsigned int i = 0; i < layout->GetBindingCount(); i++)
		{
			const DescriptorSetLayoutBinding* layoutBinding = layout->GetBinding(i);

			for (unsigned arrayElement = 0; arrayElement < layoutBinding->DescriptorCount; arrayElement++)
			{
				DescriptorState* descriptorState = FindDescriptorState(layoutBinding->Binding, arrayElement);
				if (!descriptorState)
					throw std::runtime_error("Missing state in descriptor set compilation");

				unsigned char* descriptor = descriptors + k * elementStride;
				switch (descriptorState->Type)
				{
					case Renderer::DescriptorType::Sampler:
					case Renderer::DescriptorType::TextureSampler:
					case Renderer::DescriptorType::Texture:
					case Renderer::DescriptorType::StorageTexture:
						{
							const Sampler* sampler = descriptorState->ImageInfo.Sampler;
							const TextureView* textureView = descriptorState->ImageInfo.TextureView;

							VkDescriptorImageInfo& imageInfo = *reinterpret_cast<VkDescriptorImageInfo*>(descriptor);
							imageInfo.sampler = sampler ? sampler->GetHandle() : VK_NULL_HANDLE;
							imageInfo.imageView = textureView ? textureView->GetHandle() : VK_NULL_HANDLE;
							imageInfo.imageLayout = descriptorState->Type == Renderer::DescriptorType::StorageTexture ? VK_IMAGE_LAYOUT_GENERAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						}
						break;

					case Renderer::DescriptorType::UniformBuffer:
					case Renderer::DescriptorType::StorageBuffer:
						{
							const Buffer* buffer = descriptorState->BufferInfo.Buffer;

							VkDescriptorBufferInfo& bufferInfo = *reinterpret_cast<VkDescriptorBufferInfo*>(descriptor);
							bufferInfo.buffer = buffer->GetHandle();
							bufferInfo.offset = descriptorState->BufferInfo.Offset;
							bufferInfo.range = descriptorState->BufferInfo.Range;
						}
						break;
				}

				k++;
			}
		}
	}

	std::shared_ptr<DescriptorSet> DescriptorSetStateManager::CompileSet(std::shared_ptr<DescriptorSetLayout> setLayout)
	{
		DescriptorSetCreateInfo createInfo;
		createInfo.Layout = setLayout;

		mBindingDirtyFlags = 0;

		std::shared_ptr<DescriptorSet> descriptorSet = mDescriptorSetAllocator->CreateDescriptorSet(createInfo);

		unsigned int descriptorCount = 0;
		unsigned int descriptorImageCount = 0;
		unsigned int descriptorBufferCount = 0;
		VkDescriptorImageInfo* imagesInfo = COCKTAIL_STACK_ALLOC(VkDescriptorImageInfo, setLayout->GetDescriptorCount());
		VkDescriptorBufferInfo* buffersInfo = COCKTAIL_STACK_ALLOC(VkDescriptorBufferInfo, setLayout->GetDescriptorCount());
		VkWriteDescriptorSet* writes = COCKTAIL_STACK_ALLOC(VkWriteDescriptorSet, setLayout->GetDescriptorCount());

		for (unsigned int i = 0; i < setLayout->GetBindingCount(); i++)
		{
			const DescriptorSetLayoutBinding* layoutBinding = setLayout->GetBinding(i);

			for (unsigned arrayElement = 0; arrayElement < layoutBinding->DescriptorCount; arrayElement++)
			{
				DescriptorState* descriptorState = FindDescriptorState(layoutBinding->Binding, arrayElement);
				if (!descriptorState)
					throw std::runtime_error("Missing state in descriptor set compilation");

				if (descriptorState->Type != layoutBinding->Type)
					throw std::runtime_error("Incoherence between descriptor set layout binding type and descriptor state type");

				VkWriteDescriptorSet write{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, nullptr};
				{
					write.dstSet = descriptorSet->GetHandle();
					write.dstBinding = descriptorState->Binding;
					write.dstArrayElement = descriptorState->ArrayElement;
					write.descriptorCount = 1;
					write.descriptorType = ToVkType(descriptorState->Type);

					switch (descriptorState->Type)
					{
						case Renderer::DescriptorType::Sampler:
						case Renderer::DescriptorType::TextureSampler:
						case Renderer::DescriptorType::Texture:
						case Renderer::DescriptorType::StorageTexture:
							{
								const Sampler* sampler = descriptorState->ImageInfo.Sampler;
								const TextureView* textureView = descriptorState->ImageInfo.TextureView;

								assert(sampler || textureView);

								VkDescriptorImageInfo& imageInfo = imagesInfo[descriptorImageCount];
								imageInfo.sampler = sampler ? sampler->GetHandle() : VK_NULL_HANDLE;
								imageInfo.imageView = textureView ? textureView->GetHandle() : VK_NULL_HANDLE;
								imageInfo.imageLayout = descriptorState->Type == Renderer::DescriptorType::StorageTexture ? VK_IMAGE_LAYOUT_GENERAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

								write.pImageInfo = &imageInfo;
								++descriptorImageCount;
							}
							break;

						case Renderer::DescriptorType::UniformBuffer:
						case Renderer::DescriptorType::StorageBuffer:
							{
								const Buffer* buffer = descriptorState->BufferInfo.Buffer;

								VkDescriptorBufferInfo& bufferInfo = buffersInfo[descriptorBufferCount];
								bufferInfo.buffer = buffer->GetHandle();
								bufferInfo.offset = descriptorState->BufferInfo.Offset;
								bufferInfo.range = descriptorState->BufferInfo.Range;

								write.pBufferInfo = &bufferInfo;
								++descriptorBufferCount;
							}
							break;
					}

					writes[descriptorCount] = write;
				}

				++descriptorCount;
			}
		}

		vkUpdateDescriptorSets(mRenderDevice->GetHandle(), descriptorCount, writes, 0, nullptr);

		return descriptorSet;
	}

	std::shared_ptr<DescriptorSet> DescriptorSetStateManager::CompileSetWithTemplate(std::shared_ptr<DescriptorSetLayout> descriptorSetLayout, std::shared_ptr<DescriptorUpdateTemplate> descriptorUpdateTemplate)
	{
		DescriptorSetCreateInfo createInfo;
		createInfo.Layout = descriptorSetLayout;

		mBindingDirtyFlags = 0;

		std::shared_ptr<DescriptorSet> descriptorSet = mDescriptorSetAllocator->CreateDescriptorSet(createInfo);

		unsigned int k = 0;
		const unsigned int descriptorCount = descriptorSetLayout->GetDescriptorCount();
		const std::size_t elementStride = std::max(sizeof(VkDescriptorBufferInfo), sizeof(VkDescriptorImageInfo));
		unsigned char* descriptors = COCKTAIL_STACK_ALLOC(unsigned char, descriptorCount * elementStride);
		for (unsigned int i = 0; i < descriptorSetLayout->GetBindingCount(); i++)
		{
			const DescriptorSetLayoutBinding* layoutBinding = descriptorSetLayout->GetBinding(i);

			for (unsigned arrayElement = 0; arrayElement < layoutBinding->DescriptorCount; arrayElement++)
			{
				DescriptorState* descriptorState = FindDescriptorState(layoutBinding->Binding, arrayElement);
				assert(descriptorState != nullptr && descriptorState->Type == layoutBinding->Type);

				unsigned char* descriptor = descriptors + k * elementStride;
				switch (descriptorState->Type)
				{
					case Renderer::DescriptorType::Sampler:
					case Renderer::DescriptorType::TextureSampler:
					case Renderer::DescriptorType::Texture:
					case Renderer::DescriptorType::StorageTexture:
						{
							const Sampler* sampler = descriptorState->ImageInfo.Sampler;
							const TextureView* textureView = descriptorState->ImageInfo.TextureView;

							VkDescriptorImageInfo& imageInfo = *reinterpret_cast<VkDescriptorImageInfo*>(descriptor);
							imageInfo.sampler = sampler ? sampler->GetHandle() : VK_NULL_HANDLE;
							imageInfo.imageView = textureView ? textureView->GetHandle() : VK_NULL_HANDLE;
							imageInfo.imageLayout = descriptorState->Type == Renderer::DescriptorType::StorageTexture ? VK_IMAGE_LAYOUT_GENERAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						}
						break;

					case Renderer::DescriptorType::UniformBuffer:
					case Renderer::DescriptorType::StorageBuffer:
						{
							const Buffer* buffer = descriptorState->BufferInfo.Buffer;

							VkDescriptorBufferInfo& bufferInfo = *reinterpret_cast<VkDescriptorBufferInfo*>(descriptor);
							bufferInfo.buffer = buffer->GetHandle();
							bufferInfo.offset = descriptorState->BufferInfo.Offset;
							bufferInfo.range = descriptorState->BufferInfo.Range;
						}
						break;
				}

				k++;
			}
		}

		vkUpdateDescriptorSetWithTemplateKHR(mRenderDevice->GetHandle(), descriptorSet->GetHandle(), descriptorUpdateTemplate->GetHandle(), descriptors);

		return descriptorSet;
	}

	bool DescriptorSetStateManager::IsBindingDirty(unsigned int binding) const
	{
		return mBindingDirtyFlags & Bit(binding);
	}

	bool DescriptorSetStateManager::FillImageState(DescriptorState* state, Renderer::DescriptorType type, const TextureView* textureView, const Sampler* sampler)
	{
		bool changed = false;

		changed |= CheckedAssign(state->Type, type);
		changed |= CheckedAssign(state->ImageInfo.TextureView, textureView);
		changed |= CheckedAssign(state->ImageInfo.Sampler, sampler);
		state->Dirty = changed;

		return changed;
	}

	bool DescriptorSetStateManager::FillBufferState(DescriptorState* state, Renderer::DescriptorType type, const Buffer* buffer, std::size_t offset, std::size_t range)
	{
		bool changed = false;

		changed |= CheckedAssign(state->Type, type);
		changed |= CheckedAssign(state->BufferInfo.Buffer, buffer);
		changed |= CheckedAssign(state->BufferInfo.Offset, offset);
		changed |= CheckedAssign(state->BufferInfo.Range, range);
		state->Dirty = changed;

		return changed;
	}

	DescriptorState* DescriptorSetStateManager::FindDescriptorState(unsigned binding, unsigned int arrayElement)
	{
		for (std::size_t i = 0; i < mBindingStates.size(); i++)
		{
			DescriptorState* state = &mBindingStates[i];
			if (state->Binding == binding && state->ArrayElement == arrayElement)
				return state;
		}

		return nullptr;
	}

	DescriptorState* DescriptorSetStateManager::FindOrCreateDescriptorState(unsigned int binding, unsigned int arrayElement)
	{
		DescriptorState* state = FindDescriptorState(binding, arrayElement);
		if (!state)
		{
			state = &mBindingStates.emplace_back();

			state->Binding = binding;
			state->ArrayElement = arrayElement;
		}

		return state;
	}
}
