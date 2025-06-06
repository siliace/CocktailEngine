#ifndef COCKTAIL_VULAKN_PIPELINE_GRAPHICPIPELINE_HPP
#define COCKTAIL_VULAKN_PIPELINE_GRAPHICPIPELINE_HPP

#include <Cocktail/Vulkan/Pipeline/GraphicPipelineCreateInfo.hpp>
#include <Cocktail/Vulkan/Pipeline/Pipeline.hpp>

namespace Ck::Vulkan
{
	class PipelineCache;
	class RenderDevice;

	/**
	 * \brief 
	 */
	class GraphicPipeline : public Pipeline
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice
		 * \param pipelineCache
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		GraphicPipeline(std::shared_ptr<RenderDevice> renderDevice, const PipelineCache* pipelineCache, const GraphicPipelineCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~GraphicPipeline() override;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<PipelineLayout> GetLayout() const override;

		/**
		 * \brief 
		 * \return 
		 */
		VkPipeline GetHandle() const override;

	private:

		std::shared_ptr<PipelineLayout> mLayout;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkPipeline mHandle;
	};
}

#endif // COCKTAIL_VULAKN_PIPELINE_GRAPHICPIPELINE_HPP
