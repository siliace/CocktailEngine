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
	Path PipelineCacheSaver::ComputePipelineCachePath(AnsiStringView applicationName, const VersionDescriptor& applicationVersion)
	{
		return LocalFileSystem::GetTempDirectoryPath()
				.Join(String::Convert(applicationName))
				.Join(TranslatorCast<String>(applicationVersion.ToInteger()))
				.Join(CK_TEXT("pipelinecache.bin"));
	}

	PipelineCacheSaver::PipelineCacheSaver(PipelineManager* pipelineManager, Path pipelineCachePath) :
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

					CK_LOG(VulkanLogCategory, LogLevel::Info, CK_TEXT("Pipeline cache loaded from %s"), mPipelineCachePath.ToString());
				}
			}
		}
	}

	void PipelineCacheSaver::DumpPipelineCache() const
	{
		FileUtils::WriteFile(mPipelineCachePath, mPipelineManager->GetCache()->GetCacheData(), false);
	}
}
