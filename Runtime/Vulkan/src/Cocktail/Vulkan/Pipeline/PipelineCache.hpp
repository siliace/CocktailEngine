#ifndef COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP
#define COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

#include <Cocktail/Vulkan/Pipeline/ComputePipeline.hpp>
#include <Cocktail/Vulkan/Pipeline/GraphicPipeline.hpp>
#include <Cocktail/Vulkan/Pipeline/GraphicPipelineCreateInfo.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCacheCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	using PipelineStateHash = Uint32;

	/**
	 * \brief 
	 */
	class PipelineCache : public Inherit<PipelineCache, Object, Renderer::RenderDeviceObject>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		PipelineCache(const Ref<RenderDevice>& renderDevice, const PipelineCacheCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~PipelineCache() override;
		
		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<ComputePipeline> CreateComputePipeline(const ComputePipelineCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<GraphicPipeline> CreateGraphicPipeline(const GraphicPipelineCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::vector<unsigned char> GetCacheData() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkPipelineCache GetHandle() const;

	private:

		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkPipelineCache mHandle;
		std::unordered_map<PipelineStateHash, Ref<Pipeline>> mCache;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_PIPELINECACHE_HPP
