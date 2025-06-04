#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Buffer/Buffer.hpp>
#include <Cocktail/Vulkan/Command/CommandList.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandListPool.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorUpdateTemplate.hpp>
#include <Cocktail/Vulkan/Framebuffer/DepthResolver.hpp>
#include <Cocktail/Vulkan/Framebuffer/Framebuffer.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPass.hpp>
#include <Cocktail/Vulkan/Pipeline/Pipeline.hpp>
#include <Cocktail/Vulkan/Pipeline/State/ComputeStateManager.hpp>
#include <Cocktail/Vulkan/Pipeline/State/GraphicStateManager.hpp>
#include <Cocktail/Vulkan/Shader/ShaderProgram.hpp>
#include <Cocktail/Vulkan/Shader/UniformSlot.hpp>
#include <Cocktail/Vulkan/Texture/Sampler.hpp>
#include <Cocktail/Vulkan/Texture/Texture.hpp>
#include <Cocktail/Vulkan/Texture/TextureView.hpp>

namespace Ck::Vulkan
{
	namespace
	{
		VkImageLayout GetResourceStateImageLayout(Renderer::ResourceState resourceState, const PixelFormat& format)
		{
			switch (resourceState)
			{
			default:
			case Renderer::ResourceState::Undefined:
				return VK_IMAGE_LAYOUT_UNDEFINED;

			case Renderer::ResourceState::General:
				return VK_IMAGE_LAYOUT_GENERAL;

			case Renderer::ResourceState::FramebufferAttachment:
				return format.IsColor() ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			case Renderer::ResourceState::GraphicShaderResource:
			case Renderer::ResourceState::ComputeShaderResource:
				return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			case Renderer::ResourceState::CopySource:
				return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

			case Renderer::ResourceState::CopyDestination:
				return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			}

			COCKTAIL_UNREACHABLE();
			return {};
		}

		VkAccessFlags GetSourceResourceStateAccess(Renderer::ResourceState resourceState)
		{
			return 0;
		}

		VkAccessFlags2 GetSourceResourceStateAccess2(Renderer::ResourceState resourceState)
		{
			return 0;
		}

		VkAccessFlags GetDestinationResourceStateAccess(Renderer::ResourceState resourceState)
		{
			return 0;
		}

		VkAccessFlags2 GetDestinationResourceStateAccess2(Renderer::ResourceState resourceState)
		{
			return 0;
		}

		VkPipelineStageFlags GetResourceStatePipelineStage(Renderer::ResourceState resourceState, const PixelFormat& format)
		{
			switch (resourceState)
			{
			case Renderer::ResourceState::Undefined:
				return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

			case Renderer::ResourceState::FramebufferAttachment:
				return format.IsColor() ? VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT : VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;

			case Renderer::ResourceState::General:
			case Renderer::ResourceState::UniformBuffer:
			case Renderer::ResourceState::GraphicShaderResource:
			case Renderer::ResourceState::ComputeShaderResource:
				return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

			case Renderer::ResourceState::CopySource:
			case Renderer::ResourceState::CopyDestination:
				return VK_PIPELINE_STAGE_TRANSFER_BIT;

			case Renderer::ResourceState::VertexBuffer:
			case Renderer::ResourceState::IndexBuffer:
				return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
			}

			COCKTAIL_UNREACHABLE();
			return {};
		}

		VkPipelineStageFlags2 GetResourceStatePipelineStage2(Renderer::ResourceState resourceState, const PixelFormat& format)
		{
			switch (resourceState)
			{
			case Renderer::ResourceState::Undefined:
				return VK_PIPELINE_STAGE_2_NONE_KHR;

			case Renderer::ResourceState::FramebufferAttachment:
				return format.IsColor() ? VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR : VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT_KHR;

			case Renderer::ResourceState::General:
			case Renderer::ResourceState::UniformBuffer:
			case Renderer::ResourceState::GraphicShaderResource:
			case Renderer::ResourceState::ComputeShaderResource:
				return VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT_KHR;

			case Renderer::ResourceState::CopySource:
			case Renderer::ResourceState::CopyDestination:
				return VK_PIPELINE_STAGE_2_TRANSFER_BIT_KHR;

			case Renderer::ResourceState::VertexBuffer:
				return VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT_KHR;

			case Renderer::ResourceState::IndexBuffer:
				return VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT_KHR;
			}

			COCKTAIL_UNREACHABLE();
			return {};
		}

		VkImageSubresourceRange GetImageSubResourceRange(const PixelFormat& pixelFormat, const Renderer::TextureSubResource& subResource)
		{
			VkImageSubresourceRange subresourceRange;
			if (pixelFormat.IsColor())
			{
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			}
			else
			{
				subresourceRange.aspectMask = 0;
				if (pixelFormat.IsDepth() || pixelFormat.IsDepthStencil())
					subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;

				if (pixelFormat.IsStencil() || pixelFormat.IsDepthStencil())
					subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}

			subresourceRange.baseMipLevel = subResource.BaseMipMapLevel;
			subresourceRange.levelCount = subResource.MipMapLevelCount;
			subresourceRange.baseArrayLayer = subResource.BaseArrayLayer;
			subresourceRange.layerCount = subResource.ArrayLayerCount;

			return subresourceRange;
		}

		template <typename T>
		void PopulateBufferMemoryBarrier(const QueueFamilyContext& queueFamilyContext, T& bufferMemoryBarrier, const Renderer::GpuBarrier& barrier)
		{
			Renderer::ResourceState oldState = barrier.Buffer.OldState;
			Renderer::ResourceState newState = barrier.Buffer.NewState;
			const Renderer::Buffer* resource = barrier.Buffer.Resource;

			bufferMemoryBarrier.srcAccessMask = GetSourceResourceStateAccess(oldState);
			bufferMemoryBarrier.dstAccessMask = GetDestinationResourceStateAccess(newState);
			if (Renderer::ResourceQueueTransfer* queueTransfer = barrier.QueueTransfer)
			{
				Renderer::CommandQueueType sourceQueue = queueTransfer->SourceQueueType;
				Renderer::CommandQueueType destinationQueue = queueTransfer->DestinationQueueType;

				if (!queueFamilyContext.IsUnified() && sourceQueue != destinationQueue)
				{
					bufferMemoryBarrier.srcQueueFamilyIndex = queueFamilyContext.GetFamily(sourceQueue).GetIndex();
					bufferMemoryBarrier.dstQueueFamilyIndex = queueFamilyContext.GetFamily(destinationQueue).GetIndex();
				}
				else
				{
					bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
					bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				}
			}
			else
			{
				bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			}
			bufferMemoryBarrier.buffer = static_cast<const Buffer*>(resource)->GetHandle();
			bufferMemoryBarrier.offset = barrier.Buffer.Offset;
			bufferMemoryBarrier.size = barrier.Buffer.Size;
		}

		template <typename T>
		void PopulateImageMemoryBarrier(const QueueFamilyContext& queueFamilyContext, T& imageMemoryBarrier, const Renderer::GpuBarrier& barrier)
		{
			Renderer::ResourceState oldState = barrier.Texture.OldState;
			Renderer::ResourceState newState = barrier.Texture.NewState;
			const Renderer::Texture* resource = barrier.Texture.Resource;

			imageMemoryBarrier.srcAccessMask = GetSourceResourceStateAccess(oldState);
			imageMemoryBarrier.dstAccessMask = GetDestinationResourceStateAccess(newState);
			imageMemoryBarrier.oldLayout = GetResourceStateImageLayout(oldState, resource->GetFormat());
			imageMemoryBarrier.newLayout = GetResourceStateImageLayout(newState, resource->GetFormat());
			if (Renderer::ResourceQueueTransfer* queueTransfer = barrier.QueueTransfer)
			{
				Renderer::CommandQueueType sourceQueue = queueTransfer->SourceQueueType;
				Renderer::CommandQueueType destinationQueue = queueTransfer->DestinationQueueType;

				if (!queueFamilyContext.IsUnified() && sourceQueue != destinationQueue)
				{
					imageMemoryBarrier.srcQueueFamilyIndex = queueFamilyContext.GetFamily(sourceQueue).GetIndex();
					imageMemoryBarrier.dstQueueFamilyIndex = queueFamilyContext.GetFamily(destinationQueue).GetIndex();
				}
				else
				{
					imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
					imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				}
			}
			else
			{
				imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			}
			imageMemoryBarrier.image = static_cast<const AbstractTexture*>(resource)->GetHandle();
			imageMemoryBarrier.subresourceRange = GetImageSubResourceRange(resource->GetFormat(), barrier.Texture.SubResource);
		}
	}

	CommandList::CommandList(std::shared_ptr<RenderDevice> renderDevice, std::shared_ptr<CommandListPool> allocator, DescriptorSetAllocator* descriptorSetAllocator, const Renderer::CommandListCreateInfo& createInfo) :
		mRenderDevice(std::move(renderDevice)),
		mAllocator(std::move(allocator)),
		mHandle(VK_NULL_HANDLE),
		mSecondary(createInfo.Secondary),
		mState(Renderer::CommandListState::Initial),
		mUsage(createInfo.Usage),
		mDynamicState(createInfo.DynamicState)
	{
		Renderer::CommandQueueType queueType = SelectQueueForUsage(mUsage);

		VkCommandBufferAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, nullptr };
		{
			allocateInfo.commandPool = mAllocator->GetCommandPool(queueType)->GetHandle();
			allocateInfo.level = !createInfo.Secondary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
			allocateInfo.commandBufferCount = 1;
		}

		COCKTAIL_VK_CHECK(vkAllocateCommandBuffers(mRenderDevice->GetHandle(), &allocateInfo, &mHandle));

		if (mUsage != Renderer::CommandListUsage::Transfer)
		{
			mStateManagers[Renderer::ShaderProgramType::Compute] = std::make_unique<ComputeStateManager>(mRenderDevice, descriptorSetAllocator);

			if (mUsage == Renderer::CommandListUsage::Graphic)
				mStateManagers[Renderer::ShaderProgramType::Graphic] = std::make_unique<GraphicStateManager>(mRenderDevice, descriptorSetAllocator, mDynamicState);
		}

		mOneShot = mAllocator->IsTransient();

		CommandList::SetObjectName(createInfo.Name);
	}

	CommandList::~CommandList()
	{
		DisconnectAll();

		Renderer::CommandQueueType queueType = SelectQueueForUsage(mUsage);
		vkFreeCommandBuffers(mRenderDevice->GetHandle(), mAllocator->GetCommandPool(queueType)->GetHandle(), 1, &mHandle);
	}

	void CommandList::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_COMMAND_BUFFER;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	std::shared_ptr<Renderer::RenderDevice> CommandList::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	void CommandList::Begin(Renderer::CommandList* primary)
	{
		assert(mState == Renderer::CommandListState::Initial);

		VkCommandBufferInheritanceInfo inheritanceInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO, nullptr };
		{
			if (mSecondary && primary)
			{
				const Framebuffer* framebuffer = static_cast<CommandList*>(primary)->mCurrentFramebuffer;
				inheritanceInfo.renderPass = framebuffer ? framebuffer->GetRenderPass()->GetHandle(Renderer::RenderPassMode::Initial) : VK_NULL_HANDLE;
				inheritanceInfo.framebuffer = framebuffer ? framebuffer->GetHandle() : VK_NULL_HANDLE;
			}
			else
			{
				inheritanceInfo.renderPass = VK_NULL_HANDLE;
				inheritanceInfo.framebuffer = VK_NULL_HANDLE;
			}

			inheritanceInfo.subpass = 0;
			inheritanceInfo.occlusionQueryEnable = false;
			inheritanceInfo.queryFlags = {};
			inheritanceInfo.pipelineStatistics = {};
		}

		VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, nullptr };
		{
			beginInfo.flags = 0;
			if (mOneShot)
				beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			beginInfo.pInheritanceInfo = mSecondary ? &inheritanceInfo : nullptr;
		}

		COCKTAIL_VK_CHECK(vkBeginCommandBuffer(mHandle, &beginInfo));

		mState = Renderer::CommandListState::Recording;
	}

	void CommandList::End()
	{
		assert(mState == Renderer::CommandListState::Recording || mState == Renderer::CommandListState::RecordingRenderPass);

		if (mState == Renderer::CommandListState::RecordingRenderPass)
			EndRenderPass();

		mState = Renderer::CommandListState::Executable;

		COCKTAIL_VK_CHECK(vkEndCommandBuffer(mHandle));
	}

	void CommandList::Barrier(unsigned int barrierCount, const Renderer::GpuBarrier* barriers)
	{
		if (mRenderDevice->IsFeatureSupported(RenderDeviceFeature::Synchronization2))
		{
			unsigned int memoryBarrierCount = 0;
			VkMemoryBarrier2KHR* memoryBarriers = COCKTAIL_STACK_ALLOC(VkMemoryBarrier2KHR, barrierCount);

			unsigned int bufferMemoryBarrierCount = 0;
			VkBufferMemoryBarrier2KHR* bufferMemoryBarriers = COCKTAIL_STACK_ALLOC(VkBufferMemoryBarrier2KHR, barrierCount);

			unsigned int imageMemoryBarrierCount = 0;
			VkImageMemoryBarrier2KHR* imageMemoryBarriers = COCKTAIL_STACK_ALLOC(VkImageMemoryBarrier2KHR, barrierCount);

			VkDependencyInfoKHR dependencyInfo{ VK_STRUCTURE_TYPE_DEPENDENCY_INFO_KHR, nullptr };
			{
				for (unsigned int i = 0; i < barrierCount; i++)
				{
					const Renderer::GpuBarrier& barrier = barriers[i];
					if (barrier.Type == Renderer::GpuBarrierType::Memory)
					{
						VkMemoryBarrier2KHR memoryBarrier{ VK_STRUCTURE_TYPE_MEMORY_BARRIER_2_KHR, nullptr };
						memoryBarrier.srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
						memoryBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
						memoryBarrier.dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
						memoryBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT;

						memoryBarriers[memoryBarrierCount] = memoryBarrier;
						memoryBarrierCount++;
					}
					else if (barrier.Type == Renderer::GpuBarrierType::Buffer)
					{
						Renderer::ResourceState oldState = barrier.Buffer.OldState;
						Renderer::ResourceState newState = barrier.Buffer.NewState;

						VkBufferMemoryBarrier2KHR bufferMemoryBarrier{ VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2_KHR, nullptr };
						PopulateBufferMemoryBarrier(mRenderDevice->GetQueueFamilyContext(), bufferMemoryBarrier, barrier);
						bufferMemoryBarrier.srcStageMask = GetResourceStatePipelineStage2(oldState, PixelFormat::Undefined());
						bufferMemoryBarrier.dstStageMask = GetResourceStatePipelineStage2(newState, PixelFormat::Undefined());

						bufferMemoryBarriers[bufferMemoryBarrierCount] = bufferMemoryBarrier;
						bufferMemoryBarrierCount++;
					}
					else if (barrier.Type == Renderer::GpuBarrierType::Texture)
					{
						const Renderer::Texture* resource = barrier.Texture.Resource;
						Renderer::ResourceState oldState = barrier.Texture.OldState;
						Renderer::ResourceState newState = barrier.Texture.NewState;

						VkImageMemoryBarrier2KHR imageMemoryBarrier{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2_KHR, nullptr };
						PopulateImageMemoryBarrier(mRenderDevice->GetQueueFamilyContext(), imageMemoryBarrier, barrier);
						imageMemoryBarrier.srcStageMask = GetResourceStatePipelineStage2(oldState, resource->GetFormat());
						imageMemoryBarrier.dstStageMask = GetResourceStatePipelineStage2(newState, resource->GetFormat());

						imageMemoryBarriers[imageMemoryBarrierCount] = imageMemoryBarrier;
						imageMemoryBarrierCount++;
					}
				}

				dependencyInfo.dependencyFlags = 0;
				dependencyInfo.memoryBarrierCount = memoryBarrierCount;
				dependencyInfo.pMemoryBarriers = memoryBarriers;
				dependencyInfo.bufferMemoryBarrierCount = bufferMemoryBarrierCount;
				dependencyInfo.pBufferMemoryBarriers = bufferMemoryBarriers;
				dependencyInfo.imageMemoryBarrierCount = imageMemoryBarrierCount;
				dependencyInfo.pImageMemoryBarriers = imageMemoryBarriers;
			}

			vkCmdPipelineBarrier2KHR(mHandle, &dependencyInfo);
		}
		else
		{
			for (unsigned int i = 0; i < barrierCount; i++)
			{
				const Renderer::GpuBarrier& barrier = barriers[i];
				if (barrier.Type == Renderer::GpuBarrierType::Memory)
				{
					VkMemoryBarrier memoryBarrier;
					memoryBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
					memoryBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT;

					vkCmdPipelineBarrier(mHandle,
						VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
						0,
						1, &memoryBarrier,
						0, nullptr,
						0, nullptr
					);
				}
				else if (barrier.Type == Renderer::GpuBarrierType::Buffer)
				{
					VkBufferMemoryBarrier bufferMemoryBarrier{ VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER, nullptr };
					PopulateBufferMemoryBarrier(mRenderDevice->GetQueueFamilyContext(), bufferMemoryBarrier, barrier);

					VkPipelineStageFlags sourceStages = GetResourceStatePipelineStage(barrier.Texture.OldState, PixelFormat::Undefined());
					VkPipelineStageFlags destinationStages = GetResourceStatePipelineStage(barrier.Texture.NewState, PixelFormat::Undefined());

					vkCmdPipelineBarrier(mHandle,
						sourceStages, destinationStages,
						0,
						0, nullptr,
						1, &bufferMemoryBarrier,
						0, nullptr
					);
				}
				else if (barrier.Type == Renderer::GpuBarrierType::Texture)
				{
					const Renderer::Texture* resource = barrier.Texture.Resource;

					VkImageMemoryBarrier imageMemoryBarrier{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, nullptr };
					PopulateImageMemoryBarrier(mRenderDevice->GetQueueFamilyContext(), imageMemoryBarrier, barrier);

					VkPipelineStageFlags sourceStages = GetResourceStatePipelineStage(barrier.Texture.OldState, resource->GetFormat());
					VkPipelineStageFlags destinationStages = GetResourceStatePipelineStage(barrier.Texture.NewState, resource->GetFormat());

					vkCmdPipelineBarrier(mHandle,
						sourceStages, destinationStages,
						0,
						0, nullptr,
						0, nullptr,
						1, &imageMemoryBarrier
					);
				}
			}
		}
	}

	void CommandList::GenerateMipMaps(const Renderer::Texture* texture, Renderer::ResourceState resourceState, const Renderer::TextureSubResource& subResource)
	{
		assert(subResource.BaseMipMapLevel > 0);
		assert(mState == Renderer::CommandListState::Recording);
		assert(subResource.BaseMipMapLevel + subResource.MipMapLevelCount <= texture->GetMipMapCount());
		assert(subResource.BaseArrayLayer + subResource.ArrayLayerCount <= texture->GetArrayLayerCount());

		const Texture* vkTexture = static_cast<const Texture*>(texture);

		auto rootSubResource = Renderer::TextureSubResource::AllLayersOneLevel(*texture, subResource.BaseMipMapLevel - 1);

		Renderer::GpuBarrier preBarriers[] = {
			Renderer::GpuBarrier::Of(texture, resourceState, Renderer::ResourceState::General, rootSubResource),
			Renderer::GpuBarrier::Of(texture, Renderer::ResourceState::Undefined, Renderer::ResourceState::General, subResource)
		};
		Barrier(2, preBarriers);

		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(mRenderDevice->GetPhysicalDeviceHandle(), ToVkType(vkTexture->GetFormat()), &formatProperties);
		if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)
		{
			int mipWidth = vkTexture->GetSize().Width;
			int mipHeight = vkTexture->GetSize().Height;
			int mipDepth = vkTexture->GetSize().Depth;

			for (unsigned int level = subResource.BaseMipMapLevel; level <= subResource.MipMapLevelCount; level++)
			{
				VkImageBlit* blits = COCKTAIL_STACK_ALLOC(VkImageBlit, subResource.ArrayLayerCount);

				int nextMipWidth = std::max(1, mipWidth / 2);
				int nextMipHeight = std::max(1, mipHeight / 2);
				int nextMipDepth = std::max(1, mipDepth / 2);

				for (unsigned int arrayLayer = subResource.BaseArrayLayer; arrayLayer < subResource.ArrayLayerCount; arrayLayer++)
				{
					VkImageBlit blit = {};
					blit.srcOffsets[0] = { 0, 0, 0 };
					blit.srcOffsets[1] = { mipWidth, mipHeight, mipDepth };
					blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.srcSubresource.mipLevel = level - 1;
					blit.srcSubresource.baseArrayLayer = arrayLayer;
					blit.srcSubresource.layerCount = 1;
					blit.dstOffsets[0] = { 0, 0, 0 };
					blit.dstOffsets[1] = { nextMipWidth, nextMipHeight, nextMipDepth };
					blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.dstSubresource.mipLevel = level;
					blit.dstSubresource.baseArrayLayer = arrayLayer;
					blit.dstSubresource.layerCount = 1;

					blits[arrayLayer - subResource.BaseArrayLayer] = blit;
				}

				mipWidth = nextMipWidth;
				mipHeight = nextMipHeight;
				mipDepth = nextMipDepth;

				vkCmdBlitImage(
					mHandle,
					vkTexture->GetHandle(), VK_IMAGE_LAYOUT_GENERAL,
					vkTexture->GetHandle(), VK_IMAGE_LAYOUT_GENERAL,
					subResource.ArrayLayerCount, blits,
					VK_FILTER_LINEAR
				);
			}
		}

		Renderer::GpuBarrier postBarriers[] = {
			Renderer::GpuBarrier::Of(texture, Renderer::ResourceState::General, resourceState, rootSubResource),
			Renderer::GpuBarrier::Of(texture, Renderer::ResourceState::General, resourceState, subResource)
		};
		Barrier(2, postBarriers);
	}

	void CommandList::ExecuteCommandLists(unsigned int commandListCount, Renderer::CommandList** commandLists)
	{
		VkCommandBuffer* commandBufferHandles = COCKTAIL_STACK_ALLOC(VkCommandBuffer, commandListCount);
		for (unsigned int i = 0; i < commandListCount; i++)
		{
			CommandList* vkCommandList = static_cast<CommandList*>(commandLists[i]);
			assert(vkCommandList->IsSecondary());
			assert(vkCommandList->GetUsage() == mUsage);
			assert(vkCommandList->GetState() == Renderer::CommandListState::Executable);

			commandBufferHandles[i] = vkCommandList->GetHandle();
		}

		vkCmdExecuteCommands(mHandle, commandListCount, commandBufferHandles);
	}

	void CommandList::UploadBuffer(const Renderer::Buffer* buffer, unsigned int uploadCount, const Renderer::BufferUploadInfo* uploads)
	{
		assert(mState == Renderer::CommandListState::Recording);

		unsigned int regionCount = 0;
		VkBufferCopy* regions = COCKTAIL_STACK_ALLOC(VkBufferCopy, uploadCount);

		StagingBuffer* currentStagingBuffer = nullptr;
		for (unsigned int i = 0; i < uploadCount; i++)
		{
			StagingBuffer& stagingBuffer = mAllocator->AcquireStagingBuffer(0, uploads[i].Length);
			if (currentStagingBuffer != &stagingBuffer)
			{
				if (currentStagingBuffer)
				{
					assert(regionCount > 0);
					vkCmdCopyBuffer(mHandle, currentStagingBuffer->GetBuffer()->GetHandle(), static_cast<const Buffer*>(buffer)->GetHandle(), regionCount, regions);
					regionCount = 0;
				}

				currentStagingBuffer = &stagingBuffer;
			}

			VkBufferCopy region;
			region.srcOffset = currentStagingBuffer->PushData(0, uploads[i].Length, uploads[i].Data);
			region.dstOffset = uploads[i].Offset;
			region.size = uploads[i].Length;

			regions[regionCount++] = region;
		}

		assert(regionCount > 0);
		vkCmdCopyBuffer(mHandle, currentStagingBuffer->GetBuffer()->GetHandle(), static_cast<const Buffer*>(buffer)->GetHandle(), regionCount, regions);
	}

	void CommandList::UploadTexture(const Renderer::Texture* texture, Renderer::ResourceState resourceState, unsigned int uploadCount, const Renderer::TextureUploadInfo* uploads)
	{
		assert(mState == Renderer::CommandListState::Recording);
		assert(resourceState == Renderer::ResourceState::CopyDestination || resourceState == Renderer::ResourceState::General);

		PixelFormat format = texture->GetFormat();
		const std::size_t alignment = format.GetBlockSize();

		// Use undefined on purpose, why should be upload framebuffers attachments?
		VkImageLayout imageLayout = GetResourceStateImageLayout(resourceState, PixelFormat::Undefined()); 

		unsigned int regionCount = 0;
		VkBufferImageCopy* regions = COCKTAIL_STACK_ALLOC(VkBufferImageCopy, uploadCount);

		StagingBuffer* currentStagingBuffer = nullptr;
		for (unsigned int i = 0; i < uploadCount; i++)
		{
			Extent3D<unsigned int> levelSize = Renderer::ComputeTextureLevelSize(texture->GetSize(), uploads[i].Level);
			const size_t length = format.ComputeAllocationSize(levelSize);

			StagingBuffer& stagingBuffer = mAllocator->AcquireStagingBuffer(alignment, length);
			if (currentStagingBuffer != &stagingBuffer)
			{
				if (currentStagingBuffer)
				{
					assert(regionCount > 0);
					vkCmdCopyBufferToImage(mHandle, currentStagingBuffer->GetBuffer()->GetHandle(), static_cast<const Texture*>(texture)->GetHandle(), imageLayout, regionCount, regions);
					regionCount = 0;
				}

				currentStagingBuffer = &stagingBuffer;
			}

			VkImageSubresourceLayers subresourceLayers;
			subresourceLayers.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceLayers.mipLevel = uploads[i].Level;
			subresourceLayers.baseArrayLayer = uploads[i].ArrayLayer;
			subresourceLayers.layerCount = 1;

			VkBufferImageCopy region;
			region.bufferOffset = stagingBuffer.PushData(alignment, length, uploads[i].Pixels);
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;
			region.imageSubresource = subresourceLayers;
			region.imageOffset = { 0, 0, 0 };
			region.imageExtent = ToVkType(levelSize);

			regions[regionCount++] = region;
		}

		assert(regionCount > 0);
		vkCmdCopyBufferToImage(mHandle, currentStagingBuffer->GetBuffer()->GetHandle(), static_cast<const Texture*>(texture)->GetHandle(), imageLayout, regionCount, regions);
	}

	void CommandList::Dispatch(unsigned int groupCountX, unsigned int groupCountY, unsigned int groupCountZ)
	{
		FlushComputeState();
		vkCmdDispatch(mHandle, groupCountX, groupCountY, groupCountZ);
	}

	void CommandList::BeginRenderPass(const Renderer::RenderPassBeginInfo& begin)
	{
		mCurrentFramebuffer = static_cast<const Framebuffer*>(begin.Framebuffer);
		assert(mCurrentFramebuffer);

		Extent3D<unsigned int> size = mCurrentFramebuffer->GetSize();

		// Color attachments + DepthStencil attachment with resolve attachments
		unsigned int clearValueCount = 0;
		VkClearValue clearValues[(Renderer::Framebuffer::MaxColorAttachmentCount + 1) * 2];
		VkRenderPassBeginInfo beginInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO, nullptr };
		{
			beginInfo.renderPass = mCurrentFramebuffer->GetRenderPass()->GetHandle(begin.Mode);
			beginInfo.framebuffer = mCurrentFramebuffer->GetHandle();
			beginInfo.renderArea.offset = { 0, 0 };
			beginInfo.renderArea.extent = { size.Width, size.Height };

			if (begin.Mode == Renderer::RenderPassMode::Clear)
			{
				for (unsigned int i = 0; i < mCurrentFramebuffer->GetColorAttachmentCount(); i++)
				{
					VkClearValue clearValue;
					clearValue.color.float32[0] = begin.ColorClearValue[i].R;
					clearValue.color.float32[1] = begin.ColorClearValue[i].G;
					clearValue.color.float32[2] = begin.ColorClearValue[i].B;
					clearValue.color.float32[3] = begin.ColorClearValue[i].A;

					if (mCurrentFramebuffer->GetRenderPass()->GetSamples() != Renderer::RasterizationSamples::e1)
						clearValues[clearValueCount++] = clearValue;

					clearValues[clearValueCount++] = clearValue;
				}

				if (mCurrentFramebuffer->GetDepthStencilAttachment())
				{
					VkClearValue clearValue;
					clearValue.depthStencil.depth = begin.DepthClearValue;
					clearValue.depthStencil.stencil = begin.StencilClearValue;

					if (mCurrentFramebuffer->GetRenderPass()->GetSamples() != Renderer::RasterizationSamples::e1)
						clearValues[clearValueCount++] = clearValue;

					clearValues[clearValueCount++] = clearValue;
				}

				beginInfo.clearValueCount = clearValueCount;
				beginInfo.pClearValues = clearValues;
			}
			else
			{
				beginInfo.clearValueCount = 0;
				beginInfo.pClearValues = nullptr;
			}
		}

		vkCmdBeginRenderPass(mHandle, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

		Renderer::Viewport viewport;
		viewport.Width = size.Width;
		viewport.Height = size.Height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.f;

		Renderer::Scissor scissor;
		scissor.Position = { 0, 0 };
		scissor.Size = { size.Width, size.Height };

		mState = Renderer::CommandListState::RecordingRenderPass;

		mCurrentRenderPassMode = Optional<Renderer::RenderPassMode>::Of(begin.Mode);
		GetGraphicStateManager()->SetRenderPass(mCurrentFramebuffer->GetRenderPass());

		SetViewport(viewport);
		SetScissor(scissor);
	}

	void CommandList::EndRenderPass()
	{
		assert(mState == Renderer::CommandListState::RecordingRenderPass);

		vkCmdEndRenderPass(mHandle);
		mState = Renderer::CommandListState::Recording;

		if (mCurrentFramebuffer->GetSamples() != Renderer::RasterizationSamples::e1 && !mCurrentFramebuffer->GetRenderPass()->ResolveDepthStencil())
		{
			std::shared_ptr<TextureView> depthStencilAttachment = std::static_pointer_cast<TextureView>(mCurrentFramebuffer->GetDepthStencilAttachment());
			if (depthStencilAttachment)
			{
				PixelFormat attachmentFormat = depthStencilAttachment->GetFormat();

				Renderer::ResolveMode depthResolveMode = mCurrentFramebuffer->GetRenderPass()->GetDepthResolveMode();
				Renderer::ResolveMode stencilResolveMode = mCurrentFramebuffer->GetRenderPass()->GetStencilResolveMode();

				const bool shouldResolveDepth = (attachmentFormat.IsDepth() || attachmentFormat.IsDepthStencil()) && depthResolveMode != Renderer::ResolveMode::None;
				if (shouldResolveDepth)
				{
					mRenderDevice->Invoke([&](DepthResolver* depthStencilResolver) {
						auto depthStencilMultisampleAttachment = std::static_pointer_cast<TextureView>(mCurrentFramebuffer->GetDepthStencilMultisampleAttachment());
						depthStencilResolver->Resolve(*this, mCurrentRenderPassMode.Get(), depthStencilMultisampleAttachment, depthStencilAttachment, depthResolveMode);
					});
				}
			}
		}

		mCurrentFramebuffer = nullptr;
		mCurrentRenderPassMode = Optional<Renderer::RenderPassMode>::Empty();
	}

	void CommandList::BindShaderProgram(const Renderer::ShaderProgram* inShaderProgram)
	{
		const ShaderProgram* shaderProgram = static_cast<const ShaderProgram*>(inShaderProgram);
		Renderer::ShaderProgramType programType = shaderProgram->GetType();

		StateManager* stateManager = GetStateManager(programType);

		std::shared_ptr<Pipeline> pipeline = mCurrentPipelines[programType];
		if (pipeline)
		{
			std::shared_ptr<PipelineLayout> pipelineLayout = pipeline->GetLayout();
			std::shared_ptr<PipelineLayout> nextPipelineLayout = shaderProgram->GetPipelineLayout();
			for (unsigned int set = 0; set < pipelineLayout->GetDescriptorSetLayoutCount(); set++)
			{
				std::shared_ptr<DescriptorSetLayout> descriptorSetLayout = pipelineLayout->GetDescriptorSetLayout(set);
				std::shared_ptr<DescriptorSetLayout> nextDescriptorSetLayout = nextPipelineLayout->GetDescriptorSetLayout(set);

				if (!nextDescriptorSetLayout || !nextDescriptorSetLayout->IsCompatibleWith(*descriptorSetLayout))
					stateManager->ResetBindings(set);
			}
		}

		stateManager->SetShaderProgram(shaderProgram);
	}

	void CommandList::BindVertexBuffer(unsigned int binding, const Renderer::Buffer* inVertexBuffer, std::size_t offset, unsigned int stride, bool instanced, unsigned int divisor)
	{
		const Buffer* vertexBuffer = static_cast<const Buffer*>(inVertexBuffer);
		assert(vertexBuffer->GetUsage() & Renderer::BufferUsageFlagBits::Vertex);

		GraphicStateManager* graphicStateManager = GetGraphicStateManager();
		graphicStateManager->BindVertexBuffer(binding, vertexBuffer, offset);
		graphicStateManager->SetVertexInputBinding(binding, stride, instanced, divisor);
	}

	void CommandList::BindIndexBuffer(const Renderer::Buffer* inIndexBuffer, std::size_t offset, Renderer::IndexType indexType)
	{
		const Buffer* indexBuffer = static_cast<const Buffer*>(inIndexBuffer);
		assert(indexBuffer->GetUsage() & Renderer::BufferUsageFlagBits::Index);

		GetGraphicStateManager()->BindIndexBuffer(indexBuffer, offset, indexType);
	}

	void CommandList::BindSampler(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::Sampler* sampler)
	{
		assert(!slot->IsArray() || arrayIndex < slot->GetArrayLength());
		assert(slot->GetDescriptorType() == Renderer::DescriptorType::Sampler);

		UniformSlot* descriptorSetSlot = static_cast<UniformSlot*>(slot);
		unsigned int set = descriptorSetSlot->GetSet();
		unsigned int binding = descriptorSetSlot->GetBinding();
		mStateManagers[descriptorSetSlot->GetProgramType()]->BindSampler(set, binding, arrayIndex, static_cast<const Sampler*>(sampler));
	}

	void CommandList::BindTextureSampler(Renderer::UniformSlot* slot, unsigned int arrayIndex, const Renderer::TextureView* textureView, const Renderer::Sampler* sampler)
	{
		assert(!slot->IsArray() || arrayIndex < slot->GetArrayLength());
		assert(slot->GetDescriptorType() == Renderer::DescriptorType::TextureSampler);

		UniformSlot* descriptorSetSlot = static_cast<UniformSlot*>(slot);
		unsigned int set = descriptorSetSlot->GetSet();
		unsigned int binding = descriptorSetSlot->GetBinding();
		mStateManagers[descriptorSetSlot->GetProgramType()]->BindTextureSampler(set, binding, arrayIndex, static_cast<const TextureView*>(textureView), static_cast<const Sampler*>(sampler));
	}

	void CommandList::BindTexture(Renderer::UniformSlot* uniformSlot, unsigned int arrayIndex, const Renderer::TextureView* inTextureView)
	{
		const TextureView* textureView = static_cast<const TextureView*>(inTextureView);
		assert(!uniformSlot->IsArray() || arrayIndex < uniformSlot->GetArrayLength());

		UniformSlot* descriptorSetSlot = static_cast<UniformSlot*>(uniformSlot);
		unsigned int set = descriptorSetSlot->GetSet();
		unsigned int binding = descriptorSetSlot->GetBinding();
		StateManager* stateManager = GetStateManager(descriptorSetSlot->GetProgramType());

		if (uniformSlot->GetDescriptorType() == Renderer::DescriptorType::Texture)
		{
			stateManager->BindTexture(set, binding, arrayIndex, textureView);
		}
		else if (uniformSlot->GetDescriptorType() == Renderer::DescriptorType::StorageTexture)
		{
			stateManager->BindStorageTexture(set, binding, arrayIndex, textureView);
		}
	}

	void CommandList::BindBuffer(Renderer::UniformSlot* uniformSlot, unsigned int arrayIndex, const Renderer::Buffer* uniformBuffer, std::size_t offset, std::size_t range)
	{
		assert(!uniformSlot->IsArray() || arrayIndex < uniformSlot->GetArrayLength());

		UniformSlot* descriptorSetSlot = static_cast<UniformSlot*>(uniformSlot);
		unsigned int set = descriptorSetSlot->GetSet();
		unsigned int binding = descriptorSetSlot->GetBinding();
		StateManager* stateManager = GetStateManager(descriptorSetSlot->GetProgramType());

		if (uniformSlot->GetDescriptorType() == Renderer::DescriptorType::UniformBuffer)
		{
			assert(uniformBuffer->GetUsage() & Renderer::BufferUsageFlagBits::Uniform);
			stateManager->BindUniformBuffer(set, binding, arrayIndex, static_cast<const Buffer*>(uniformBuffer), offset, range);
		}
		else if (uniformSlot->GetDescriptorType() == Renderer::DescriptorType::StorageBuffer)
		{
			assert(uniformBuffer->GetUsage() & Renderer::BufferUsageFlagBits::Storage);
			stateManager->BindStorageBuffer(set, binding, arrayIndex, static_cast<const Buffer*>(uniformBuffer), offset, range);
		}
	}

	void CommandList::UpdatePipelineConstant(Renderer::ShaderType shaderType, unsigned int offset, unsigned int length, const void* data)
	{
		Renderer::ShaderProgramType programType = shaderType == Renderer::ShaderType::Compute ? Renderer::ShaderProgramType::Compute : Renderer::ShaderProgramType::Graphic;

		StateManager* stateManager = GetStateManager(programType);
		if (stateManager)
			stateManager->UpdatePipelineConstant(shaderType, offset, length, data);
	}

	void CommandList::EnableVertexBinding(unsigned int binding, bool enable)
	{
		GetGraphicStateManager()->EnableVertexBinding(binding, enable);
	}

	void CommandList::SetVertexInputAttributes(unsigned int binding, unsigned int attributeCount, const Renderer::VertexInputAttribute* attributes)
	{
		GetGraphicStateManager()->SetVertexInputAttributes(binding, attributeCount, attributes);
	}

	void CommandList::SetPrimitiveTopology(Renderer::PrimitiveTopology primitiveTopology)
	{
		GetGraphicStateManager()->SetPrimitiveTopology(primitiveTopology);
	}

	void CommandList::SetViewport(const Renderer::Viewport& viewport)
	{
		if (mDynamicState & Renderer::CommandListDynamicStateBits::Viewport)
		{
			VkViewport vkViewport = ToVkType(viewport);
			vkCmdSetViewport(mHandle, 0, 1, &vkViewport);
		}
		else
		{
			GetGraphicStateManager()->SetViewport(0, viewport);
		}
	}

	void CommandList::SetScissor(const Renderer::Scissor& scissor)
	{
		if (mDynamicState & Renderer::CommandListDynamicStateBits::Viewport)
		{
			VkRect2D vkScissor = ToVkType(scissor);
			vkCmdSetScissor(mHandle, 0, 1, &vkScissor);
		}
		else
		{
			GetGraphicStateManager()->SetScissor(0, scissor);
		}
	}

	void CommandList::EnableRasterizerDiscard(bool enable)
	{
		GetGraphicStateManager()->EnableRasterizerDiscard(enable);
	}

	void CommandList::SetPolygonMode(Renderer::PolygonMode polygonMode)
	{
		GetGraphicStateManager()->SetPolygonMode(polygonMode);
	}

	void CommandList::SetCullMode(Renderer::CullMode cullMode)
	{
			GetGraphicStateManager()->SetCullMode(cullMode);
	}

	void CommandList::SetFrontFace(Renderer::FrontFace frontFace)
	{
		GetGraphicStateManager()->SetFrontFace(frontFace);
	}

	void CommandList::EnableDepthBias(bool enable)
	{
		GetGraphicStateManager()->EnableDepthBias(enable);
	}

	void CommandList::SetDepthBias(float constantFactor, float clamp, float slopeFactor)
	{
		GetGraphicStateManager()->SetDepthBias(constantFactor, clamp, slopeFactor);
	}

	void CommandList::SetLineWidth(float lineWidth)
	{
		if (mRenderDevice->IsFeatureSupported(RenderDeviceFeature::WideLine))
		{
			if (mDynamicState & Renderer::CommandListDynamicStateBits::LineWidth)
			{
				vkCmdSetLineWidth(mHandle, lineWidth);
			}
			else
			{
				GetGraphicStateManager()->SetLineWidth(lineWidth);
			}
		}
	}

	void CommandList::EnableSampleShading(bool enable)
	{
		GetGraphicStateManager()->EnableSampleShading(enable);
	}

	void CommandList::SetMinSampleShading(float minSampleShading)
	{
		GetGraphicStateManager()->SetMinSampleShading(minSampleShading);
	}

	void CommandList::EnableAlphaToCoverage(bool enable)
	{
		GetGraphicStateManager()->EnableAlphaToCoverage(enable);
	}

	void CommandList::EnableAlphaToOne(bool enable)
	{
		GetGraphicStateManager()->EnableAlphaToOne(enable);
	}

	void CommandList::EnableDepthTest(bool enable)
	{
		GetGraphicStateManager()->EnableDepthTest(enable);
	}

	void CommandList::EnableDepthWrite(bool enable)
	{
		GetGraphicStateManager()->EnableDepthWrite(enable);
	}

	void CommandList::SetDepthCompareOp(Renderer::CompareOp compareOp)
	{
		GetGraphicStateManager()->SetDepthCompareOp(compareOp);
	}

	void CommandList::EnableLogicOp(bool enable)
	{
		GetGraphicStateManager()->EnableLogicOp(enable);
	}

	void CommandList::SetLogicOp(Renderer::LogicOp logicOp)
	{
		GetGraphicStateManager()->SetLogicOp(logicOp);
	}

	void CommandList::SetBlendingConstants(const LinearColor& blendingConstants)
	{
		GetGraphicStateManager()->SetBlendingConstants(blendingConstants);
	}

	void CommandList::EnableBlending(unsigned int attachmentIndex, bool enable)
	{
		GetGraphicStateManager()->EnableBlending(attachmentIndex, enable);
	}

	void CommandList::SetBlendingEquation(unsigned int attachmentIndex, Renderer::BlendOp colorBlendingOp, Renderer::BlendOp alphaBlendingOp)
	{
		GetGraphicStateManager()->SetBlendingEquation(attachmentIndex, colorBlendingOp, alphaBlendingOp);
	}

	void CommandList::SetBlendingFunction(unsigned int attachmentIndex, Renderer::BlendFactor sourceColor, Renderer::BlendFactor destinationColor, Renderer::BlendFactor sourceAlpha, Renderer::BlendFactor destinationAlpha)
	{
		GetGraphicStateManager()->SetBlendingFunction(attachmentIndex, sourceColor, destinationColor, sourceAlpha, destinationAlpha);
	}

	void CommandList::Draw(unsigned int vertexCount, unsigned int instanceCount, unsigned int firstVertex, unsigned int firstInstance)
	{
		FlushGraphicState();
		vkCmdDraw(mHandle, vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void CommandList::DrawIndexed(unsigned int indexCount, unsigned int instanceCount, unsigned int firstIndex, int indexOffset, unsigned int firstInstance)
	{
		FlushGraphicState();
		vkCmdDrawIndexed(mHandle, indexCount, instanceCount, firstIndex, indexOffset, firstInstance);
	}

	void CommandList::Reset(bool releaseResources)
	{
		assert(mAllocator->IsCommandListResetable());

		vkResetCommandBuffer(mHandle, releaseResources ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0);

		MarkInitial();
	}

	Renderer::CommandListState CommandList::GetState() const
	{
		return mState;
	}

	Renderer::CommandListUsage CommandList::GetUsage() const
	{
		return mUsage;
	}

	Renderer::CommandListDynamicState CommandList::GetDynamicState() const
	{
		return mDynamicState;
	}

	bool CommandList::IsSecondary() const
	{
		return false;
	}

	void CommandList::MarkInitial()
	{
		if (mUsage != Renderer::CommandListUsage::Transfer)
		{
			for (Renderer::ShaderProgramType programType : Enum<Renderer::ShaderProgramType>::Values)
			{
				mCurrentPipelines[programType] = nullptr;

				// We reset everything at the beginning to avoid to be polluted by the previous state
				if (mStateManagers[programType])
					mStateManagers[programType]->ResetBindings();
			}
		}

		mState = Renderer::CommandListState::Initial;

		mOneShot = false;
	}

	void CommandList::MarkPending()
	{
		mState = Renderer::CommandListState::Pending;
	}

	void CommandList::MarkCompleted()
	{
		mState = mOneShot ? Renderer::CommandListState::Invalid : Renderer::CommandListState::Initial;
	}

	VkCommandBuffer CommandList::GetHandle() const
	{
		return mHandle;
	}

	ComputeStateManager* CommandList::GetComputeStateManager() const
	{
		return static_cast<ComputeStateManager*>(GetStateManager(Renderer::ShaderProgramType::Compute));
	}

	GraphicStateManager* CommandList::GetGraphicStateManager() const
	{
		return static_cast<GraphicStateManager*>(GetStateManager(Renderer::ShaderProgramType::Graphic));
	}

	StateManager* CommandList::GetStateManager(Renderer::ShaderProgramType programType) const
	{
		return mStateManagers[programType].get();
	}

	void CommandList::FlushComputeState()
	{
		FlushState(Renderer::ShaderProgramType::Compute);
	}

	void CommandList::FlushGraphicState()
	{
		FlushState(Renderer::ShaderProgramType::Graphic);

		GraphicStateManager* graphicStateManager = GetGraphicStateManager();

		if (graphicStateManager->IsDirty(GraphicStateManager::GraphicDirtyFlagBits::IndexBuffer))
		{
			IndexBufferBinding indexBufferBinding = graphicStateManager->CompileIndexBuffer();
			vkCmdBindIndexBuffer(mHandle, indexBufferBinding.BufferHandle, indexBufferBinding.Offset, indexBufferBinding.IndexType);
		}

		if (graphicStateManager->IsDirty(GraphicStateManager::GraphicDirtyFlagBits::VertexBuffers))
		{
			for (unsigned int binding = 0; binding < MaxInputBindings; binding++)
			{
				if (!graphicStateManager->IsVertexBufferDirty(binding))
					continue;

				VertexBufferBindingRange bufferRange = graphicStateManager->CompileVertexBuffers(binding);
				vkCmdBindVertexBuffers(mHandle, binding, bufferRange.BindingCount, bufferRange.BufferHandles, bufferRange.BufferOffsets);
			}
		}
	}

	void CommandList::FlushState(Renderer::ShaderProgramType programType)
	{
		StateManager* stateManager = GetStateManager(programType);
		std::shared_ptr<Pipeline> currentPipeline = mCurrentPipelines[programType];

		if (!currentPipeline || stateManager->IsDirty(StateManager::DirtyFlagBits::Pipeline))
		{
			std::shared_ptr<Pipeline> pipeline = stateManager->CompilePipeline();
			if (currentPipeline != pipeline)
			{
				vkCmdBindPipeline(mHandle, pipeline->GetLayout()->GetBindPoint(), pipeline->GetHandle());
				mCurrentPipelines[programType] = pipeline;
				currentPipeline = pipeline;
			}
		}

		std::shared_ptr<PipelineLayout> pipelineLayout = currentPipeline->GetLayout();

		if (stateManager->IsDirty(StateManager::DirtyFlagBits::DescriptorSet))
		{
			for (unsigned int set = 0; set < pipelineLayout->GetDescriptorSetLayoutCount(); set++)
			{
				if (!stateManager->IsDescriptorSetDirty(set))
					continue;

				std::shared_ptr<DescriptorSetLayout> setLayout = pipelineLayout->GetDescriptorSetLayout(set);
				if (setLayout->SupportPushDescriptor())
				{
					if (auto descriptorUpdateTemplate = pipelineLayout->GetDescriptorUpdateTemplate(set))
					{
						unsigned char* descriptors = COCKTAIL_STACK_ALLOC(unsigned char, setLayout->GetDescriptorCount() * descriptorUpdateTemplate->GetElementStride());

						stateManager->CompilePushDescriptorsWithTemplate(setLayout, descriptorUpdateTemplate, set, descriptors);
						vkCmdPushDescriptorSetWithTemplateKHR(
							mHandle,
							descriptorUpdateTemplate->GetHandle(),
							pipelineLayout->GetHandle(),
							set,
							descriptors
						);
					}
					else
					{
						VkDescriptorImageInfo* imagesInfo = COCKTAIL_STACK_ALLOC(VkDescriptorImageInfo, setLayout->GetDescriptorCount());
						VkDescriptorBufferInfo* buffersInfo = COCKTAIL_STACK_ALLOC(VkDescriptorBufferInfo, setLayout->GetDescriptorCount());
						VkWriteDescriptorSet* writes = COCKTAIL_STACK_ALLOC(VkWriteDescriptorSet, setLayout->GetDescriptorCount());

						unsigned int writeCount = stateManager->CompilePushDescriptors(setLayout, set, imagesInfo, buffersInfo, writes);
						vkCmdPushDescriptorSetKHR(
							mHandle,
							pipelineLayout->GetBindPoint(),
							pipelineLayout->GetHandle(),
							set,
							writeCount,
							writes
						);
					}
				}
				else
				{
					DescriptorSetRange descriptorSetRange = stateManager->CompileDescriptorSets(pipelineLayout, set);
					vkCmdBindDescriptorSets(
						mHandle,
						pipelineLayout->GetBindPoint(),
						pipelineLayout->GetHandle(),
						set,
						descriptorSetRange.DescriptorSetCount, descriptorSetRange.DescriptorSetHandles,
						descriptorSetRange.DynamicOffsetCount, descriptorSetRange.DynamicOffsets
					);
				}
			}
		}

		if (stateManager->IsDirty(StateManager::DirtyFlagBits::PipelineConstant))
		{
			for (Renderer::ShaderType shaderType : Enum<Renderer::ShaderType>::Values)
			{
				Optional<PushConstantBlockInfo> pushConstantBlockInfo = pipelineLayout->GetPipelineConstantBlock(shaderType);
				if (pushConstantBlockInfo.IsEmpty() || !stateManager->IsDirtyPipelineConstants(shaderType))
					continue;

				PipelineConstantRange pipelineConstantRange = stateManager->CompilePipelineConstants(shaderType);
				vkCmdPushConstants(
					mHandle,
					pipelineLayout->GetHandle(),
					ToVkType(shaderType),
					pipelineConstantRange.Offset, pipelineConstantRange.Size,
					pipelineConstantRange.Data
				);
			}
		}
	}
}
