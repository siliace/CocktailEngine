#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Renderer/Framebuffer/FramebufferCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/SamplerCreateInfo.hpp>

#include <Cocktail/Vulkan/ExtensionManager.hpp>
#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VolkService.hpp>
#include <Cocktail/Vulkan/Vulkan.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Buffer/Buffer.hpp>
#include <Cocktail/Vulkan/Buffer/BufferView.hpp>
#include <Cocktail/Vulkan/Command/Fence.hpp>
#include <Cocktail/Vulkan/Context/RenderContext.hpp>
#include <Cocktail/Vulkan/Context/RenderSurface.hpp>
#include <Cocktail/Vulkan/Context/Swapchain.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorSet.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorUpdateTemplate.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorPool.hpp>
#include <Cocktail/Vulkan/Framebuffer/DepthResolver.hpp>
#include <Cocktail/Vulkan/Framebuffer/Framebuffer.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderBuffer.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPass.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPassCreateInfo.hpp>
#include <Cocktail/Vulkan/Memory/DeviceMemory.hpp>
#include <Cocktail/Vulkan/Pipeline/GraphicPipelineCreateInfo.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCache.hpp>
#include <Cocktail/Vulkan/Shader/Shader.hpp>
#include <Cocktail/Vulkan/Shader/ShaderProgram.hpp>
#include <Cocktail/Vulkan/Shader/ValidationCache.hpp>
#include <Cocktail/Vulkan/Texture/Sampler.hpp>
#include <Cocktail/Vulkan/Texture/Texture.hpp>
#include <Cocktail/Vulkan/Texture/TextureView.hpp>

namespace Ck::Vulkan
{
	RenderDevice::RenderDevice(const RenderDeviceCreateInfo& createInfo) :
		mInstance(VK_NULL_HANDLE),
		mPhysicalDevice(VK_NULL_HANDLE),
		mHandle(VK_NULL_HANDLE)
	{
		CreateInstance(createInfo.ApplicationName, createInfo.ApplicationVersion, createInfo.ApiVersion, createInfo.EnableValidation);

		ChoosePhysicalDevice();

		CreateDevice();
	}

	RenderDevice::~RenderDevice()
	{
		vkDestroyDevice(mHandle, nullptr);
		vkDestroyInstance(mInstance, nullptr);
	}

	std::shared_ptr<Renderer::Buffer> RenderDevice::CreateBuffer(const Renderer::BufferCreateInfo& createInfo)
	{
		return mBufferPool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	std::shared_ptr<Renderer::BufferView> RenderDevice::CreateBufferView(const Renderer::BufferViewCreateInfo& createInfo)
	{
		return mBufferViewPool.Allocate(shared_from_this(), createInfo, nullptr);
	}

	std::shared_ptr<CommandList> RenderDevice::CreateCommandList(std::shared_ptr<CommandListPool> pool, DescriptorSetAllocator* descriptorSetAllocator, const Renderer::CommandListCreateInfo& createInfo)
	{
		return mCommandListPool.Allocate(shared_from_this(), pool, descriptorSetAllocator, createInfo);
	}

	std::shared_ptr<ComputePipeline> RenderDevice::CreateComputePipeline(const PipelineCache* pipelineCache, const ComputePipelineCreateInfo& createInfo)
	{
		return mComputePipelinePool.Allocate(shared_from_this(), pipelineCache, createInfo, nullptr);
	}

	std::shared_ptr<DescriptorPool> RenderDevice::CreateDescriptorPool(const DescriptorPoolCreateInfo& createInfo)
	{
		return mDescriptorPoolPool.Allocate(shared_from_this(), createInfo, nullptr);
	}

	std::shared_ptr<DescriptorSetLayout> RenderDevice::CreateDescriptorSetLayout(const DescriptorSetLayoutCreateInfo& createInfo)
	{
		return mDescriptorSetLayoutPool.Allocate(shared_from_this(), createInfo, nullptr);
	}

	std::shared_ptr<DescriptorUpdateTemplate> RenderDevice::CreateDescriptorUpdateTemplate(const DescriptorUpdateTemplateCreateInfo& createInfo)
	{
		return mDescriptorUpdateTemplatePool.Allocate(shared_from_this(), createInfo, nullptr);
	}

	std::shared_ptr<DeviceMemory> RenderDevice::CreateDeviceMemory(const DeviceMemoryCreateInfo& createInfo)
	{
		return mDeviceMemoryPool.Allocate(shared_from_this(), createInfo, nullptr);
	}

	std::shared_ptr<Renderer::Fence> RenderDevice::CreateFence(const Renderer::FenceCreateInfo& createInfo)
	{
		return mFencePool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	std::shared_ptr<Renderer::Framebuffer> RenderDevice::CreateFramebuffer(const Renderer::FramebufferCreateInfo& createInfo)
	{
		Renderer::FramebufferLayout framebufferLayout;
		framebufferLayout.Samples = createInfo.Samples;
		framebufferLayout.ColorAttachmentCount = createInfo.ColorAttachmentCount;
		for (unsigned int i = 0; i < createInfo.ColorAttachmentCount; i++)
			framebufferLayout.ColorAttachmentFormats[i] = createInfo.ColorAttachments[i]->GetFormat();

		if (createInfo.DepthStencilAttachment)
			framebufferLayout.DepthStencilAttachmentFormat = createInfo.DepthStencilAttachment->GetFormat();

		RenderPassCreateInfo renderPassCreateInfo;
		renderPassCreateInfo.FramebufferLayout = framebufferLayout;
		renderPassCreateInfo.DepthResolveMode = createInfo.DepthResolveMode;
		renderPassCreateInfo.StencilResolveMode = createInfo.StencilResolveMode;
		renderPassCreateInfo.Presentable = false;

		return CreateFramebuffer(CreateRenderPass(renderPassCreateInfo), createInfo);
	}

	std::shared_ptr<Renderer::Framebuffer> RenderDevice::CreateFramebuffer(std::shared_ptr<RenderPass> renderPass, const Renderer::FramebufferCreateInfo& createInfo)
	{
		return mFramebufferPool.Allocate(shared_from_this(), std::move(renderPass), createInfo, nullptr);
	}
	
	std::shared_ptr<GraphicPipeline> RenderDevice::CreateGraphicPipeline(const PipelineCache* pipelineCache, const GraphicPipelineCreateInfo& createInfo)
	{
		return mGraphicPipelinePool.Allocate(shared_from_this(), pipelineCache, createInfo, nullptr);
	}
	
	std::shared_ptr<PipelineLayout> RenderDevice::CreatePipelineLayout(const PipelineLayoutCreateInfo& createInfo)
	{
		return mPipelineLayoutPool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	std::shared_ptr<RenderBuffer> RenderDevice::CreateRenderBuffer(const RenderBufferCreateInfo& createInfo)
	{
		return mRenderBufferPool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	std::shared_ptr<Renderer::RenderContext> RenderDevice::CreateRenderContext(const Renderer::RenderContextCreateInfo& createInfo)
	{
		return mRenderContextPool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	std::shared_ptr<Renderer::RenderSurface> RenderDevice::CreateRenderSurface(const Renderer::RenderSurfaceCreateInfo& createInfo)
	{
		return mRenderSurfacePool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	std::shared_ptr<RenderPass> RenderDevice::CreateRenderPass(const RenderPassCreateInfo& createInfo)
	{
		return mRenderPassPool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	std::shared_ptr<Renderer::Sampler> RenderDevice::CreateSampler(const Renderer::SamplerCreateInfo& createInfo)
	{
		return mSamplerPool.Allocate(shared_from_this(), createInfo, nullptr);;
	}

	std::shared_ptr<Semaphore> RenderDevice::CreateSemaphore(const SemaphoreCreateInfo& createInfo)
	{
		return mSemaphorePool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	std::shared_ptr<Renderer::Shader> RenderDevice::CreateShader(const Renderer::ShaderCreateInfo& createInfo)
	{
		if (IsFeatureSupported(RenderDeviceFeature::ValidationCache))
		{
			ValidationCache* validationCache = Resolve<ValidationCache>();
			if (validationCache)
				return mShaderPool.Allocate(shared_from_this(), validationCache, createInfo, nullptr);
		}

		return mShaderPool.Allocate(shared_from_this(), nullptr, createInfo, nullptr);
	}
	
	std::shared_ptr<Renderer::ShaderProgram> RenderDevice::CreateShaderProgram(const Renderer::ShaderProgramCreateInfo& createInfo)
	{
		return mShaderProgramPool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	std::shared_ptr<Swapchain> RenderDevice::CreateSwapchain(const SwapchainCreateInfo& createInfo)
	{
		return mSwapchainPool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	std::shared_ptr<Renderer::Texture> RenderDevice::CreateTexture(const Renderer::TextureCreateInfo& createInfo)
	{
		return mTexturePool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	std::shared_ptr<Renderer::TextureView> RenderDevice::CreateTextureView(const Renderer::TextureViewCreateInfo& createInfo)
	{
		return mTextureViewPool.Allocate(shared_from_this(), createInfo, nullptr);
	}
	
	const QueueFamilyContext& RenderDevice::GetQueueFamilyContext() const
	{
		return *mQueueFamilyContext;
	}

	bool RenderDevice::IsFeatureSupported(RenderDeviceFeature feature) const
	{
		return mSupportedFeatures[feature];
	}

	bool RenderDevice::IsExtensionSupported(Renderer::RenderDeviceExtension extension) const
	{
		return mSupportedExtensions[extension];
	}

	Renderer::TextureUsageFlags RenderDevice::GetTextureFormatSupport(const PixelFormat& format, Renderer::MemoryType memoryType) const
	{
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(mPhysicalDevice, ToVkType(format), &formatProperties);

		Renderer::TextureUsageFlags usage;
		VkFormatFeatureFlags formatFeatures = memoryType == Renderer::MemoryType::Static ? formatProperties.optimalTilingFeatures : formatProperties.linearTilingFeatures;

		if (formatFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)
			usage |= Renderer::TextureUsageFlagBits::Sampled;

		if (formatFeatures & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT)
			usage |= Renderer::TextureUsageFlagBits::Storage;

		if (format.IsColor())
		{
			if (formatFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)
				usage |= Renderer::TextureUsageFlagBits::Attachment;
		}
		else
		{
			if (formatFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
				usage |= Renderer::TextureUsageFlagBits::Attachment;
		}

		return usage;
	}

	Signal<LogLevel, Renderer::MessageType, std::string_view>& RenderDevice::OnDebugMessage()
	{
		return mOnDebugMessage;
	}

	VkInstance RenderDevice::GetInstanceHandle() const
	{
		return mInstance;
	}

	VkPhysicalDevice RenderDevice::GetPhysicalDeviceHandle() const
	{
		return mPhysicalDevice;
	}

	VkDevice RenderDevice::GetHandle() const
	{
		return mHandle;
	}

	void RenderDevice::CreateInstance(std::string_view applicationName, const VersionDescriptor& applicationVersion, ApiVersion apiVersion, bool enableValidationLayer)
	{
		VkApplicationInfo applicationInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr };
		{
			applicationInfo.pApplicationName = applicationName.data();
			applicationInfo.applicationVersion = VK_MAKE_API_VERSION(0, applicationVersion.Major, applicationVersion.Minor, applicationVersion.Patch);
			applicationInfo.pEngineName = "CocktailEngine";
			applicationInfo.engineVersion = VK_MAKE_API_VERSION(0, COCKTAIL_MAJOR_VERSION, COCKTAIL_MAJOR_VERSION, COCKTAIL_MAJOR_VERSION);

			switch (apiVersion)
			{
			case ApiVersion::Version_1_0:
				applicationInfo.apiVersion = VK_API_VERSION_1_0;
				break;

			case ApiVersion::Version_1_1:
				applicationInfo.apiVersion = VK_API_VERSION_1_1;
				break;

			case ApiVersion::Version_1_2:
				applicationInfo.apiVersion = VK_API_VERSION_1_2;
				break;

			case ApiVersion::Version_1_3:
				applicationInfo.apiVersion = VK_API_VERSION_1_3;
				break;

			case ApiVersion::Version_1_4:
				applicationInfo.apiVersion = VK_API_VERSION_1_4;
				break;
			}
		}

		// Check for supported layers
		constexpr static const char* ValidationLayerName = "VK_LAYER_KHRONOS_validation";

		std::vector<const char*> enabledLayerNames;
		if (enableValidationLayer && ExtensionManager::IsLayerSupported(ValidationLayerName))
			enabledLayerNames.emplace_back(ValidationLayerName);

		for (Renderer::RenderDeviceExtension extension : Enum<Renderer::RenderDeviceExtension>::Values)
			mSupportedExtensions[extension] = mExtensionManager.EnableInstanceExtension(extension);

		for (RenderDeviceFeature feature : Enum<RenderDeviceFeature>::Values)
			mSupportedFeatures[feature] = mExtensionManager.EnableInstanceExtension(feature);

		std::vector<const char*> enabledExtensionNames = mExtensionManager.GetInstanceExtensions();
		VkInstanceCreateInfo createInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr };
		{
			createInfo.flags = 0;
			createInfo.pApplicationInfo = &applicationInfo;
			createInfo.enabledLayerCount = static_cast<unsigned int>(enabledLayerNames.size());
			createInfo.ppEnabledLayerNames = enabledLayerNames.data();
			createInfo.enabledExtensionCount = static_cast<unsigned int>(enabledExtensionNames.size());
			createInfo.ppEnabledExtensionNames = enabledExtensionNames.data();
		}

		COCKTAIL_VK_CHECK(vkCreateInstance(&createInfo, nullptr, &mInstance));

		App::Resolve<VolkService>()->LoadInstanceOnly(mInstance);
	}

	void RenderDevice::ChoosePhysicalDevice()
	{
		auto rankingFunction = [](VkPhysicalDevice physicalDevice) {
			std::size_t score = 0;

			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(physicalDevice, &properties);

			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

			for (unsigned int i = 0; i < memoryProperties.memoryHeapCount; i++)
			{
				std::size_t heapScore = memoryProperties.memoryHeaps[i].size / (1024llu * 1024llu);
				if (memoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
					heapScore *= 2;

				score += heapScore;
			}

			switch (properties.deviceType)
			{
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				score *= 100;
				break;

			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				score *= 10;
				break;

			default:
				score = 1;
				break;
			}

			return score;
		};

		unsigned int physicalDeviceCount = 0;
		COCKTAIL_VK_CHECK(vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr));

		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		COCKTAIL_VK_CHECK(vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, physicalDevices.data()));

		unsigned int bestScore = 0;
		for (VkPhysicalDevice physicalDevice : physicalDevices)
		{
			unsigned int physicalDeviceScore = rankingFunction(physicalDevice);
			if (physicalDeviceScore > bestScore)
			{
				bestScore = physicalDeviceScore;
				mPhysicalDevice = physicalDevice;
			}
		}

		mQueueFamilyContext = std::make_unique<QueueFamilyContext>(mPhysicalDevice);
	}

	void RenderDevice::CreateDevice()
	{
		const bool hasPhysicalDeviceFeatures2 = mExtensionManager.IsSupportedInstanceExtension(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);

		const float priorities = 1.f;
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		for (unsigned int family : mQueueFamilyContext->FindFamilyIndexes(true))
		{
			VkDeviceQueueCreateInfo queueCreateInfo{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, nullptr };
			queueCreateInfo.flags = 0;
			queueCreateInfo.queueFamilyIndex = family;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &priorities;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		for (Renderer::RenderDeviceExtension extension : Enum<Renderer::RenderDeviceExtension>::Values)
		{
			if (mSupportedExtensions[extension])
				mSupportedExtensions[extension] = mExtensionManager.EnableDeviceExtension(extension, mPhysicalDevice);
		}

		for (RenderDeviceFeature feature : Enum<RenderDeviceFeature>::Values)
		{
			if (mSupportedFeatures[feature])
				mSupportedFeatures[feature] = mExtensionManager.EnableDeviceExtension(feature, mPhysicalDevice);
		}

		VkPhysicalDeviceFeatures2KHR physicalDeviceFeatures{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR, nullptr };
		VkPhysicalDeviceSynchronization2FeaturesKHR synchronization2Features{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR, nullptr };
		VkPhysicalDeviceTimelineSemaphoreFeaturesKHR timelineSemaphoreFeatures{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR, nullptr };	
		if (hasPhysicalDeviceFeatures2)
		{
			if (mExtensionManager.IsSupportedDeviceExtension(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME))
				Chain(physicalDeviceFeatures, timelineSemaphoreFeatures);

			if (mExtensionManager.IsSupportedDeviceExtension(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME))
				Chain(physicalDeviceFeatures, synchronization2Features);

			vkGetPhysicalDeviceFeatures2KHR(mPhysicalDevice, &physicalDeviceFeatures);

			if (timelineSemaphoreFeatures.timelineSemaphore == VK_FALSE)
				mSupportedExtensions[Renderer::RenderDeviceExtension::TimelineSynchronization] = false;

			if (synchronization2Features.synchronization2 == VK_FALSE)
				mSupportedFeatures[RenderDeviceFeature::Synchronization2] = false;
		}
		else
		{
			VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(mPhysicalDevice, &features);
			physicalDeviceFeatures.features = features;
		}

		if (physicalDeviceFeatures.features.wideLines == VK_FALSE)
			mSupportedFeatures[RenderDeviceFeature::WideLine] = false;

		std::vector<const char*> enabledExtensionNames = mExtensionManager.GetDeviceExtensions();
		VkDeviceCreateInfo createInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, nullptr };
		{
			if (hasPhysicalDeviceFeatures2)
				Chain(createInfo, physicalDeviceFeatures);

			createInfo.flags = 0;
			createInfo.queueCreateInfoCount = static_cast<unsigned int>(queueCreateInfos.size());
			createInfo.pQueueCreateInfos = queueCreateInfos.data();
			createInfo.enabledLayerCount = 0;
			createInfo.ppEnabledLayerNames = nullptr;
			createInfo.enabledExtensionCount = static_cast<unsigned int>(enabledExtensionNames.size());
			createInfo.ppEnabledExtensionNames = enabledExtensionNames.data();
			createInfo.pEnabledFeatures = hasPhysicalDeviceFeatures2 ? nullptr : &physicalDeviceFeatures.features;
		}

		COCKTAIL_VK_CHECK(vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mHandle));

		App::Resolve<VolkService>()->LoadDevice(mHandle);
	}
}
