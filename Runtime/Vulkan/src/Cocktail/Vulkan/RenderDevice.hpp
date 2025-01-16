#ifndef COCKTAIL_VULKAN_RENDERDEVICE_HPP
#define COCKTAIL_VULKAN_RENDERDEVICE_HPP

#include <Cocktail/Core/Application/ServiceContainer.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>

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
	class RenderDevice : public ServiceContainer, public Inherit<RenderDevice, Object, Renderer::RenderDevice>
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
		Ref<Renderer::Buffer> CreateBuffer(const Renderer::BufferCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::BufferView> CreateBufferView(const Renderer::BufferViewCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param pool 
		 * \param descriptorSetAllocator 
		 * \param createInfo 
		 * \return 
		 */
		Ref<CommandList> CreateCommandList(const Ref<CommandListPool>& pool, const Ref<DescriptorSetAllocator>& descriptorSetAllocator, const Renderer::CommandListCreateInfo& createInfo);

		/**
		 * \brief
		 * \return
		 */
		Ref<ComputePipeline> CreateComputePipeline(const PipelineCache* pipelineCache, const ComputePipelineCreateInfo& createInfo);

		/**
		 * \brief
		 * \param createInfo
		 * \return
		 */
		Ref<DescriptorPool> CreateDescriptorPool(const DescriptorPoolCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<DescriptorSetAllocator> CreateDescriptorSetAllocator(const DescriptorSetAllocatorCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<DescriptorSetLayout> CreateDescriptorSetLayout(const DescriptorSetLayoutCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<DescriptorUpdateTemplate> CreateDescriptorUpdateTemplate(const DescriptorUpdateTemplateCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<DeviceMemory> CreateDeviceMemory(const DeviceMemoryCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::Fence> CreateFence(const Renderer::FenceCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::Framebuffer> CreateFramebuffer(const Renderer::FramebufferCreateInfo& createInfo) override;

		/**
		 * \brief
		 * \param renderPass
		 * \param createInfo
		 * \return
		 */
		Ref<Renderer::Framebuffer> CreateFramebuffer(Ref<RenderPass> renderPass, const Renderer::FramebufferCreateInfo& createInfo);

		/**
		 * \brief 
		 * \return 
		 */
		Ref<GraphicPipeline> CreateGraphicPipeline(const PipelineCache* pipelineCache, const GraphicPipelineCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<PipelineLayout> CreatePipelineLayout(const PipelineLayoutCreateInfo& createInfo);

		/**
		 * \brief
		 * \param createInfo 
		 * \return 
		 */
		Ref<RenderBuffer> CreateRenderBuffer(const RenderBufferCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::RenderContext> CreateRenderContext(const Renderer::RenderContextCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::RenderSurface> CreateRenderSurface(const Renderer::RenderSurfaceCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<RenderPass> CreateRenderPass(const RenderPassCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::Sampler> CreateSampler(const Renderer::SamplerCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Semaphore> CreateSemaphore(const SemaphoreCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::Shader> CreateShader(const Renderer::ShaderCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::ShaderProgram> CreateShaderProgram(const Renderer::ShaderProgramCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Swapchain> CreateSwapchain(const SwapchainCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::Texture> CreateTexture(const Renderer::TextureCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::TextureView> CreateTextureView(const Renderer::TextureViewCreateInfo& createInfo) override;

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
		Ref<QueueFamilyContext> mQueueFamilyContext;
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
		std::unordered_map<Renderer::SamplerCreateInfo, Ref<Sampler>> mSamplerCache;
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
