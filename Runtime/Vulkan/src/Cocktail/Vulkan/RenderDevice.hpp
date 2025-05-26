#ifndef COCKTAIL_VULKAN_RENDERDEVICE_HPP
#define COCKTAIL_VULKAN_RENDERDEVICE_HPP

#include <Cocktail/Core/Application/ServiceContainer.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>
#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/Command/CommandListCreateInfo.hpp>

#include <Cocktail/Vulkan/ExtensionManager.hpp>
#include <Cocktail/Vulkan/RenderDeviceCreateInfo.hpp>
#include <Cocktail/Vulkan/Buffer/Buffer.hpp>
#include <Cocktail/Vulkan/Buffer/BufferView.hpp>
#include <Cocktail/Vulkan/Command/Fence.hpp>
#include <Cocktail/Vulkan/Context/RenderContext.hpp>
#include <Cocktail/Vulkan/Context/Swapchain.hpp>
#include <Cocktail/Vulkan/Framebuffer/Framebuffer.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPass.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPassCreateInfo.hpp>
#include <Cocktail/Vulkan/Memory/DeviceMemory.hpp>
#include <Cocktail/Vulkan/Pipeline/ComputePipelineCreateInfo.hpp>
#include <Cocktail/Vulkan/Pipeline/GraphicPipelineCreateInfo.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCache.hpp>
#include <Cocktail/Vulkan/Queue/QueueFamilyContext.hpp>
#include <Cocktail/Vulkan/Shader/Shader.hpp>
#include <Cocktail/Vulkan/Shader/ShaderProgram.hpp>
#include <Cocktail/Vulkan/Texture/Sampler.hpp>
#include <Cocktail/Vulkan/Texture/Texture.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class RenderDevice : public Renderer::RenderDevice, public ServiceContainer, public std::enable_shared_from_this<RenderDevice>
	{
	public:

		/**
		 * \brief 
		 */
		static constexpr unsigned int MaxParallelRenderContext = 1;

		/**
		 * \brief 
		 * \param createInfo 
		 */
		explicit RenderDevice(const RenderDeviceCreateInfo& createInfo);

		/**
		 * \brief 
		 */
		~RenderDevice() override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::Buffer> CreateBuffer(const Renderer::BufferCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::BufferView> CreateBufferView(const Renderer::BufferViewCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param pool 
		 * \param descriptorSetAllocator 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<CommandList> CreateCommandList(std::shared_ptr<CommandListPool> pool, std::shared_ptr<DescriptorSetAllocator> descriptorSetAllocator, const Renderer::CommandListCreateInfo& createInfo);

		/**
		 * \brief
		 * \return
		 */
		std::shared_ptr<ComputePipeline> CreateComputePipeline(const PipelineCache* pipelineCache, const ComputePipelineCreateInfo& createInfo);

		/**
		 * \brief
		 * \param createInfo
		 * \return
		 */
		std::shared_ptr<DescriptorPool> CreateDescriptorPool(const DescriptorPoolCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<DescriptorSetAllocator> CreateDescriptorSetAllocator(const DescriptorSetAllocatorCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<DescriptorSetLayout> CreateDescriptorSetLayout(const DescriptorSetLayoutCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<DescriptorUpdateTemplate> CreateDescriptorUpdateTemplate(const DescriptorUpdateTemplateCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<DeviceMemory> CreateDeviceMemory(const DeviceMemoryCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::Fence> CreateFence(const Renderer::FenceCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::Framebuffer> CreateFramebuffer(const Renderer::FramebufferCreateInfo& createInfo) override;

		/**
		 * \brief
		 * \param renderPass
		 * \param createInfo
		 * \return
		 */
		std::shared_ptr<Renderer::Framebuffer> CreateFramebuffer(std::shared_ptr<RenderPass> renderPass, const Renderer::FramebufferCreateInfo& createInfo);

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<GraphicPipeline> CreateGraphicPipeline(const PipelineCache* pipelineCache, const GraphicPipelineCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<PipelineLayout> CreatePipelineLayout(const PipelineLayoutCreateInfo& createInfo);

		/**
		 * \brief
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<RenderBuffer> CreateRenderBuffer(const RenderBufferCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::RenderContext> CreateRenderContext(const Renderer::RenderContextCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::RenderSurface> CreateRenderSurface(const Renderer::RenderSurfaceCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<RenderPass> CreateRenderPass(const RenderPassCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::Sampler> CreateSampler(const Renderer::SamplerCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Semaphore> CreateSemaphore(const SemaphoreCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::Shader> CreateShader(const Renderer::ShaderCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::ShaderProgram> CreateShaderProgram(const Renderer::ShaderProgramCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Swapchain> CreateSwapchain(const SwapchainCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::Texture> CreateTexture(const Renderer::TextureCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::TextureView> CreateTextureView(const Renderer::TextureViewCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \return 
		 */
		const QueueFamilyContext& GetQueueFamilyContext() const;

		/**
		 * \brief 
		 * \param feature 
		 * \return 
		 */
		bool IsFeatureSupported(RenderDeviceFeature feature) const;

		/**
		 * \brief 
		 * \param extension 
		 * \return 
		 */
		bool IsExtensionSupported(Renderer::RenderDeviceExtension extension) const override;

		/**
		 * \brief 
		 * \param format 
		 * \param memoryType 
		 * \return 
		 */
		Renderer::TextureUsageFlags GetTextureFormatSupport(const PixelFormat& format, Renderer::MemoryType memoryType) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<LogLevel, Renderer::MessageType, std::string_view>& OnDebugMessage() override;

		/**
		 * \brief 
		 * \return 
		 */
		VkInstance GetInstanceHandle() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkPhysicalDevice GetPhysicalDeviceHandle() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkDevice GetHandle() const;

	private:

		/**
		 * \brief 
		 * \param applicationName 
		 * \param applicationVersion 
		 * \param apiVersion 
		 * \param enableValidationLayer 
		 */
		void CreateInstance(std::string_view applicationName, const VersionDescriptor& applicationVersion, ApiVersion apiVersion, bool enableValidationLayer);

		/**
		 * \brief 
		 */
		void ChoosePhysicalDevice();

		/**
		 * \brief 
		 */
		void CreateDevice();

		ExtensionManager mExtensionManager;
		VkInstance mInstance;
		VkPhysicalDevice mPhysicalDevice;
		std::unique_ptr<QueueFamilyContext> mQueueFamilyContext;
		VkDevice mHandle;
		ObjectPool<Buffer> mBufferPool;
		ObjectPool<BufferView> mBufferViewPool;
		ObjectPool<CommandList> mCommandListPool;
		ObjectPool<ComputePipeline> mComputePipelinePool;
		ObjectPool<DescriptorPool> mDescriptorPoolPool;
		ObjectPool<DescriptorSetAllocator> mDescriptorSetAllocatorPool;
		ObjectPool<DescriptorSetLayout> mDescriptorSetLayoutPool;
		ObjectPool<DescriptorUpdateTemplate> mDescriptorUpdateTemplatePool;
		ObjectPool<DeviceMemory> mDeviceMemoryPool;
		ObjectPool<Fence> mFencePool;
		ObjectPool<Framebuffer> mFramebufferPool;
		ObjectPool<GraphicPipeline> mGraphicPipelinePool;
		ObjectPool<PipelineLayout> mPipelineLayoutPool;
		ObjectPool<RenderBuffer> mRenderBufferPool;
		ObjectPool<RenderContext> mRenderContextPool;
		ObjectPool<RenderPass> mRenderPassPool;
		ObjectPool<RenderSurface> mRenderSurfacePool;
		ObjectPool<Sampler> mSamplerPool;
		std::unordered_map<Renderer::SamplerCreateInfo, std::shared_ptr<Sampler>> mSamplerCache;
		ObjectPool<Semaphore> mSemaphorePool;
		ObjectPool<Shader> mShaderPool;
		ObjectPool<ShaderProgram> mShaderProgramPool;
		ObjectPool<Swapchain> mSwapchainPool;
		ObjectPool<Texture> mTexturePool;
		ObjectPool<TextureView> mTextureViewPool;
		EnumMap<RenderDeviceFeature, bool> mSupportedFeatures;
		EnumMap<Renderer::RenderDeviceExtension, bool> mSupportedExtensions;
		Signal<LogLevel, Renderer::MessageType, std::string_view> mOnDebugMessage;
	};
}

#endif // COCKTAIL_VULKAN_RENDERDEVICE_HPP
