#ifndef COCKTAIL_VULKAN_PIPELINE_COMPUTEPIPELINE_HPP
#define COCKTAIL_VULKAN_PIPELINE_COMPUTEPIPELINE_HPP

#include <Cocktail/Vulkan/Pipeline/ComputePipelineCreateInfo.hpp>
#include <Cocktail/Vulkan/Pipeline/Pipeline.hpp>

namespace Ck::Vulkan
{
	class PipelineCache;
	class RenderDevice;

	/**
	 * \brief 
	 */
	class ComputePipeline : public Pipeline
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param pipelineCache
		 * \param createInfo
		 * \param allocationCallbacks
		 */
		ComputePipeline(std::shared_ptr<RenderDevice> renderDevice, const PipelineCache* pipelineCache, const ComputePipelineCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~ComputePipeline() override;

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
#endif // COCKTAIL_VULKAN_PIPELINE_COMPUTEPIPELINE_HPP
