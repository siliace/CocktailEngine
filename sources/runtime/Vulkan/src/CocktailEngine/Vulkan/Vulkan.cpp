#include <CocktailEngine/Core/IO/Input/Stream/FileInputStream.hpp>
#include <CocktailEngine/Core/IO/Input/Stream/MemoryInputStream.hpp>
#include <CocktailEngine/Core/Log/Log.hpp>
#include <CocktailEngine/Core/Utility/FileUtils.hpp>

#include <CocktailEngine/Vulkan/ExtensionManager.hpp>
#include <CocktailEngine/Vulkan/RenderDevice.hpp>
#include <CocktailEngine/Vulkan/Vulkan.hpp>
#include <CocktailEngine/Vulkan/VulkanUtils.hpp>
#include <CocktailEngine/Vulkan/Debug/DebugMessenger.hpp>
#include <CocktailEngine/Vulkan/Framebuffer/DepthResolver.hpp>
#include <CocktailEngine/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>
#include <CocktailEngine/Vulkan/Pipeline/PipelineCacheSaver.hpp>
#include <CocktailEngine/Vulkan/Pipeline/PipelineManager.hpp>
#include <CocktailEngine/Vulkan/Shader/ValidationCache.hpp>
#include <CocktailEngine/Vulkan/Texture/StaticSamplerManager.hpp>

namespace Ck::Vulkan
{
	COCKTAIL_DEFINE_LOG_CATEGORY(VulkanLogCategory);

	bool IsLayerSupported(const char* layerName)
	{
		return ExtensionManager::IsLayerSupported(layerName);
	}

	VulkanApiVersion GetSupportedApiVersion()
	{
		unsigned int version = 0;
		COCKTAIL_VK_CHECK(vkEnumerateInstanceVersion(&version));

		if (VK_API_VERSION_MAJOR(version) == 1)
		{
			if (VK_API_VERSION_MINOR(version) == 0)
				return VulkanApiVersion::Version_1_0;

			if (VK_API_VERSION_MINOR(version) == 1)
				return VulkanApiVersion::Version_1_1;

			if (VK_API_VERSION_MINOR(version) == 2)
				return VulkanApiVersion::Version_1_2;

			if (VK_API_VERSION_MINOR(version) == 3)
				return VulkanApiVersion::Version_1_3;
		}

		// Fallback :
		// TODO: find a better way to handle this ? Is it even possible to reach this place ? Maybe with a shitty driver ?
		return VulkanApiVersion::Version_1_0;
	}

	UniquePtr<Renderer::RenderDevice> CreateRenderDevice(const RenderDeviceCreateInfo& createInfo)
	{
		UniquePtr<RenderDevice> renderDevice = MakeUnique<RenderDevice>(createInfo);

		if (createInfo.EnableValidation && renderDevice->IsExtensionSupported(Renderer::RenderDeviceExtension::Debug))
		{
			renderDevice->Singleton<DebugMessenger>([renderDevice = renderDevice.Get()]() {
				return MakeUnique<DebugMessenger>(renderDevice, DebugMessengerCreateInfo{}, nullptr);
			});
			renderDevice->Resolve<DebugMessenger>();
		}

		if (renderDevice->IsFeatureSupported(RenderDeviceFeature::ValidationCache))
		{
			renderDevice->Singleton<ValidationCache>([renderDevice = renderDevice.Get()]() {
				return MakeUnique<ValidationCache>(renderDevice, ValidationCacheCreateInfo{}, nullptr);
			});
		}

		renderDevice->Singleton<DepthResolver>([renderDevice = renderDevice.Get()]() {
			return MakeUnique<DepthResolver>(renderDevice);
		});

		renderDevice->Singleton<DeviceMemoryAllocator>([renderDevice = renderDevice.Get(), createInfo = createInfo]() {
			return MakeUnique<DeviceMemoryAllocator>(renderDevice, createInfo.DeviceMemoryBlockSize);
		});

		renderDevice->Singleton<PipelineManager>([renderDevice = renderDevice.Get()]() {
			return MakeUnique<PipelineManager>(renderDevice);
		});

		Path pipelineCachePath = PipelineCacheSaver::ComputePipelineCachePath(createInfo.ApplicationName, createInfo.ApplicationVersion);
		renderDevice->Singleton<PipelineCacheSaver>([pipelineCachePath = Move(pipelineCachePath)](PipelineManager* pipelineManager) {
			return MakeUnique<PipelineCacheSaver>(pipelineManager, Move(pipelineCachePath));
		}, false);

		renderDevice->Singleton<StaticSamplerManager>([renderDevice = renderDevice.Get()]() {
			return MakeUnique<StaticSamplerManager>(renderDevice);
		});

		return renderDevice;
	}
}
