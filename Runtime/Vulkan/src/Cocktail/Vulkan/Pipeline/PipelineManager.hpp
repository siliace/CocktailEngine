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
		std::shared_ptr<ComputePipeline> CreateComputePipeline(const ComputePipelineCreateInfo& createInfo);

		/**
		 * \brief
		 * \param createInfo
		 * \return
		 */
		std::shared_ptr<GraphicPipeline> CreateGraphicPipeline(const GraphicPipelineCreateInfo& createInfo);

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<PipelineCache> GetCache() const;

		/**
		 * \brief 
		 * \return 
		 */
		RenderDevice* GetRenderDevice() const;

	private:

		RenderDevice* mRenderDevice;
		std::unordered_map<PipelineStateHash, std::shared_ptr<GraphicPipeline>> mGraphicPipelines;
		std::unordered_map<PipelineStateHash, std::shared_ptr<ComputePipeline>> mComputePipelines;
		std::shared_ptr<PipelineCache> mCache;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_PIPELINEMANAGER_HPP
