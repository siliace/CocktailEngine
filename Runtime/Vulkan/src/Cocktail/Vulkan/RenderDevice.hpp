#ifndef COCKTAIL_VULKAN_RENDERDEVICE_HPP
#define COCKTAIL_VULKAN_RENDERDEVICE_HPP

#include <Cocktail/Core/Application/ServiceContainer.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>
#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/Command/CommandListCreateInfo.hpp>

#include <Cocktail/Vulkan/ExtensionManager.hpp>
#include <Cocktail/Vulkan/RenderDeviceCreateInfo.hpp>
#include <Cocktail/Vulkan/Queue/QueueFamilyContext.hpp>

namespace Ck::Vulkan
{
	class CommandListPool;
	class Buffer;
	class BufferView;
	class CommandList;
	class ComputePipeline;
	struct ComputePipelineCreateInfo;
	class DescriptorPool;
	struct DescriptorPoolCreateInfo;
	class DescriptorSetAllocator;
	class DescriptorSetLayout;
	struct DescriptorSetLayoutCreateInfo;
	class DescriptorUpdateTemplate;
	struct DescriptorUpdateTemplateCreateInfo;
	class DeviceMemory;
	struct DeviceMemoryCreateInfo;
	class Fence;
	class Framebuffer;
	class GraphicPipeline;
	struct GraphicPipelineCreateInfo;
	class PipelineCache;
	class PipelineLayout;
	struct PipelineLayoutCreateInfo;
	class RenderBuffer;
	struct RenderBufferCreateInfo;
	class RenderContext;
	class RenderPass;
	struct RenderPassCreateInfo;
	class RenderSurface;
	class Sampler;
	class Semaphore;
	struct SemaphoreCreateInfo;
	class Shader;
	class ShaderProgram;
	class Swapchain;
	struct SwapchainCreateInfo;
	class Texture;
	class TextureView;

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
		std::shared_ptr<CommandList> CreateCommandList(std::shared_ptr<CommandListPool> pool, DescriptorSetAllocator* descriptorSetAllocator, const Renderer::CommandListCreateInfo& createInfo);

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
		Signal<LogLevel, Renderer::MessageType, AsciiStringView>& OnDebugMessage() override;

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
		void CreateInstance(AsciiStringView applicationName, const VersionDescriptor& applicationVersion, ApiVersion apiVersion, bool enableValidationLayer);

		/**
		 * \brief 
		 */
		void ChoosePhysicalDevice();

		/**
		 * \brief 
		 */
		void CreateDevice();

		ExtensionManager mExtensionManager;
		VkAllocationCallbacks mAllocationCallbacks;
		VkInstance mInstance;
		VkPhysicalDevice mPhysicalDevice;
		UniquePtr<QueueFamilyContext> mQueueFamilyContext;
		VkDevice mHandle;
		ObjectPool<Buffer> mBufferPool;
		ObjectPool<BufferView> mBufferViewPool;
		ObjectPool<CommandList> mCommandListPool;
		ObjectPool<ComputePipeline> mComputePipelinePool;
		ObjectPool<DescriptorPool> mDescriptorPoolPool;
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
		ObjectPool<Semaphore> mSemaphorePool;
		ObjectPool<Shader> mShaderPool;
		ObjectPool<ShaderProgram> mShaderProgramPool;
		ObjectPool<Swapchain> mSwapchainPool;
		ObjectPool<Texture> mTexturePool;
		ObjectPool<TextureView> mTextureViewPool;
		EnumMap<RenderDeviceFeature, bool> mSupportedFeatures;
		EnumMap<Renderer::RenderDeviceExtension, bool> mSupportedExtensions;
		Signal<LogLevel, Renderer::MessageType, AsciiStringView> mOnDebugMessage;
	};
}

#endif // COCKTAIL_VULKAN_RENDERDEVICE_HPP
