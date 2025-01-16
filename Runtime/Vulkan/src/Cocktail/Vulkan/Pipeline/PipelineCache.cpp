#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCache.hpp>

namespace Ck::Vulkan
{
	namespace
	{
		uint32_t MurmurHash(const void* key, uint32_t len, uint32_t seed)
		{
			uint32_t c1 = 0xcc9e2d51;
			uint32_t c2 = 0x1b873593;
			uint32_t r1 = 15;
			uint32_t r2 = 13;
			uint32_t m = 5;
			uint32_t n = 0xe6546b64;
			uint32_t h = 0;
			uint32_t k = 0;
			uint8_t* d = (uint8_t*)key; // 32 bit extract from `key'
			const uint32_t* chunks = NULL;
			const uint8_t* tail = NULL; // tail - last 8 bytes
			int i = 0;
			int l = len / 4; // chunk length

			h = seed;

			chunks = (const uint32_t*)(d + l * 4); // body
			tail = (const uint8_t*)(d + l * 4); // last 8 byte chunk of `key'

			// for each 4 byte chunk of `key'
			for (i = -l; i != 0; ++i)
			{
				// next 4 byte chunk of `key'
				k = chunks[i];

				// encode next 4 byte chunk of `key'
				k *= c1;
				k = (k << r1) | (k >> (32 - r1));
				k *= c2;

				// append to hash
				h ^= k;
				h = (h << r2) | (h >> (32 - r2));
				h = h * m + n;
			}

			k = 0;

			// remainder
			switch (len & 3)
			{
			// `len % 4'
			case 3: k ^= (tail[2] << 16);
			case 2: k ^= (tail[1] << 8);

			case 1:
				k ^= tail[0];
				k *= c1;
				k = (k << r1) | (k >> (32 - r1));
				k *= c2;
				h ^= k;
			}

			h ^= len;

			h ^= (h >> 16);
			h *= 0x85ebca6b;
			h ^= (h >> 13);
			h *= 0xc2b2ae35;
			h ^= (h >> 16);

			return h;
		}
	}

	PipelineCache::PipelineCache(const Ref<RenderDevice>& renderDevice, const PipelineCacheCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		VkPipelineCacheCreateInfo vkCreateInfo{VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO, nullptr};
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.initialDataSize = createInfo.InitialDataSize;
			vkCreateInfo.pInitialData = createInfo.InitialData;
		}

		COCKTAIL_VK_CHECK(vkCreatePipelineCache(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		PipelineCache::SetObjectName(createInfo.Name);
	}

	PipelineCache::~PipelineCache()
	{
		vkDestroyPipelineCache(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	Ref<ComputePipeline> PipelineCache::CreateComputePipeline(const ComputePipelineCreateInfo& createInfo)
	{
		PipelineStateHash stateHash = MurmurHash(&createInfo.ComputeState, sizeof(ComputeState), sizeof(ComputeState));
		if (auto it = mCache.find(stateHash); it != mCache.end())
			return ComputePipeline::Cast(it->second);

		Ref<ComputePipeline> pipeline = mRenderDevice->CreateComputePipeline(this, createInfo);
		mCache[stateHash] = pipeline;

		return pipeline;
	}

	Ref<GraphicPipeline> PipelineCache::CreateGraphicPipeline(const GraphicPipelineCreateInfo& createInfo)
	{
		PipelineStateHash stateHash = MurmurHash(&createInfo.GraphicState, sizeof(GraphicState), sizeof(GraphicState));
		if (auto it = mCache.find(stateHash); it != mCache.end())
			return GraphicPipeline::Cast(it->second);

		Ref<GraphicPipeline> pipeline = mRenderDevice->CreateGraphicPipeline(this, createInfo);
		mCache[stateHash] = pipeline;

		return pipeline;
	}
	
	void PipelineCache::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_PIPELINE_CACHE;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Ref<RenderDevice> PipelineCache::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	ByteArray PipelineCache::GetCacheData() const
	{
		std::size_t binarySize;
		COCKTAIL_VK_CHECK(vkGetPipelineCacheData(mRenderDevice->GetHandle(), mHandle, &binarySize, nullptr));

		ByteArray binary(binarySize);
		COCKTAIL_VK_CHECK(vkGetPipelineCacheData(mRenderDevice->GetHandle(), mHandle, &binarySize, binary.GetData()));

		return binary;
	}

	VkPipelineCache PipelineCache::GetHandle() const
	{
		return mHandle;
	}

}
