#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Log/LogLevel.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Vulkan.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCache.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCacheCreateInfo.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCacheSaver.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineManager.hpp>

namespace Ck::Vulkan
{
	std::filesystem::path PipelineCacheSaver::ComputePipelineCachePath(std::string_view applicationName, const VersionDescriptor& applicationVersion)
	{
		std::filesystem::path path = std::filesystem::temp_directory_path();
		path = path / applicationName;
		path = path / std::to_string(applicationVersion.ToInteger());
		path = path / "pipelinecache.bin";

		return path;
	}

	PipelineCacheSaver::PipelineCacheSaver(PipelineManager* pipelineManager, std::filesystem::path pipelineCachePath) :
		mPipelineManager(pipelineManager),
		mPipelineCachePath(std::move(pipelineCachePath))
	{
		ByteArray pipelineBinaryCache = FileUtils::ReadFile(mPipelineCachePath);
		RenderDevice* renderDevice = mPipelineManager->GetRenderDevice();

		if (!pipelineBinaryCache.IsEmpty())
		{
			PipelineCacheHeader cacheHeader;
			MemoryInputStream inputStream(pipelineBinaryCache.GetData(), pipelineBinaryCache.GetSize());
			inputStream.Read(&cacheHeader, sizeof(PipelineCacheHeader));

			if (cacheHeader.Length == sizeof(PipelineCacheHeader) && cacheHeader.Version == VK_PIPELINE_CACHE_HEADER_VERSION_ONE)
			{
				VkPhysicalDeviceProperties physicalDeviceProperties;
				vkGetPhysicalDeviceProperties(renderDevice->GetPhysicalDeviceHandle(), &physicalDeviceProperties);

				const bool sameDeviceId = cacheHeader.DeviceId == physicalDeviceProperties.deviceID;
				const bool sameVendorId = cacheHeader.VendorId == physicalDeviceProperties.vendorID;

				if (sameDeviceId && sameVendorId)
				{
					PipelineCacheCreateInfo pipelineCacheCreateInfo;
					pipelineCacheCreateInfo.InitialData = pipelineBinaryCache.GetData();
					pipelineCacheCreateInfo.InitialDataSize = pipelineBinaryCache.GetSize();

					PipelineCache pipelineCache(renderDevice, pipelineCacheCreateInfo, nullptr);
					pipelineManager->GetCache()->Merge(pipelineCache);

					CK_LOG(VulkanLogCategory, LogLevel::Info, "Pipeline cache loaded from {}", mPipelineCachePath.string());
				}
			}
		}
	}

	void PipelineCacheSaver::DumpPipelineCache() const
	{
		FileUtils::WriteFile(mPipelineCachePath, mPipelineManager->GetCache()->GetCacheData(), false);
	}
}
