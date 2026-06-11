#ifndef COCKTAIL_VULKAN_PIPELINE_PIPELINEMANAGER_HPP
#define COCKTAIL_VULKAN_PIPELINE_PIPELINEMANAGER_HPP

#include <Cocktail/Vulkan/Pipeline/ComputePipeline.hpp>
#include <Cocktail/Vulkan/Pipeline/GraphicPipeline.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCache.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class PipelineManager
	{
	public:

		/**
		 * \brief 
		 */
		explicit PipelineManager(RenderDevice* renderDevice);

		/**
		 * \brief
		 * \param createInfo
		 * \return
		 */
		SharedPtr<ComputePipeline> CreateComputePipeline(const ComputePipelineCreateInfo& createInfo);

		/**
		 * \brief
		 * \param createInfo
		 * \return
		 */
		SharedPtr<GraphicPipeline> CreateGraphicPipeline(const GraphicPipelineCreateInfo& createInfo);

		/**
		 * \brief 
		 * \return 
		 */
		SharedPtr<PipelineCache> GetCache() const;

		/**
		 * \brief 
		 * \return 
		 */
		RenderDevice* GetRenderDevice() const;

	private:

		RenderDevice* mRenderDevice;
		HashMap<PipelineStateHash, SharedPtr<GraphicPipeline>> mGraphicPipelines;
		HashMap<PipelineStateHash, SharedPtr<ComputePipeline>> mComputePipelines;
		SharedPtr<PipelineCache> mCache;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_PIPELINEMANAGER_HPP
