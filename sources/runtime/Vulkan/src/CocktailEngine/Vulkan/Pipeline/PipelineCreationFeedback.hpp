#ifndef COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECREATIONFEEDBACK_HPP
#define COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECREATIONFEEDBACK_HPP

#include <CocktailEngine/Core/Enum.hpp>

#include <CocktailEngine/Renderer/Shader/ShaderType.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief Maximum number of shader stages a single pipeline can report feedback for
	 *
	 * A GraphicPipeline has at most one stage per Renderer::ShaderType and a ComputePipeline
	 * exactly one, so the graphic bound covers both kinds of pipeline.
	 */
	constexpr unsigned int MaxPipelineStageCreationFeedbacks = Enum<Renderer::ShaderType>::ValueCount;

	/**
	 * \brief Creation feedback reported by the driver for a pipeline, or for one of its shader stages
	 *
	 * Mirrors VkPipelineCreationFeedback. A driver is free to report nothing at all even when
	 * VK_EXT_pipeline_creation_feedback is enabled, hence \p Valid: every other member is
	 * meaningless when it is false.
	 *
	 * \see RenderDeviceFeature::PipelineCreationFeedback
	 */
	struct PipelineCreationFeedbackEntry
	{
		/**
		 * \brief Whether the driver actually filled this feedback
		 *
		 * False when the RenderDeviceFeature::PipelineCreationFeedback feature is unsupported,
		 * and false when the driver declined to report anything for this pipeline or stage.
		 */
		bool Valid = false;

		/**
		 * \brief Whether the pipeline or stage was reused from the PipelineCache given at creation
		 *
		 * When true, \p Duration accounts for a cache lookup rather than a shader compilation.
		 * Only meaningful when \p Valid is true.
		 */
		bool ApplicationCacheHit = false;

		/**
		 * \brief Whether the creation was made faster by the base pipeline it was derived from
		 *
		 * Only reported for a pipeline created with a base pipeline, and only meaningful when
		 * \p Valid is true.
		 */
		bool BasePipelineAcceleration = false;

		/**
		 * \brief Duration of the creation, in nanoseconds
		 *
		 * Only meaningful when \p Valid is true.
		 */
		Uint64 Duration = 0;
	};

	/**
	 * \brief Creation feedback of a whole pipeline, shader stages included
	 *
	 * \see Pipeline::GetCreationFeedback
	 */
	struct PipelineCreationFeedback
	{
		/**
		 * \brief Feedback for the pipeline taken as a whole
		 */
		PipelineCreationFeedbackEntry Overall;

		/**
		 * \brief Feedback for each shader stage of the pipeline
		 *
		 * Entries follow the order the stages were given to vkCreateGraphicsPipelines or
		 * vkCreateComputePipelines, which is the Enum<Renderer::ShaderType>::Values order
		 * restricted to the stages the pipeline actually has. Only the first \p StageCount
		 * entries are meaningful.
		 */
		PipelineCreationFeedbackEntry Stages[MaxPipelineStageCreationFeedbacks];

		/**
		 * \brief Number of meaningful entries in \p Stages
		 */
		unsigned int StageCount = 0;
	};

	/**
	 * \brief Collects the creation feedback of a single pipeline
	 *
	 * Owns the storage the driver writes into, so an instance must outlive the vkCreate*Pipelines
	 * call it was chained into. Chaining is a no-op when the
	 * RenderDeviceFeature::PipelineCreationFeedback feature is unsupported, in which case Collect
	 * returns an all-invalid feedback and the pipeline is created exactly as it was before.
	 *
	 * The type is neither copyable nor movable: the create info it hands to Vulkan points into it.
	 */
	class PipelineCreationFeedbackCollector
	{
	public:

		/**
		 * \brief
		 * \param renderDevice The RenderDevice creating the pipeline
		 * \param stageCount Number of shader stages of the pipeline being created
		 */
		PipelineCreationFeedbackCollector(const RenderDevice* renderDevice, unsigned int stageCount);

		PipelineCreationFeedbackCollector(const PipelineCreationFeedbackCollector&) = delete;

		PipelineCreationFeedbackCollector& operator=(const PipelineCreationFeedbackCollector&) = delete;

		/**
		 * \brief Chains the feedback create info into a graphic pipeline create info
		 *
		 * Does nothing when the feature is unsupported.
		 *
		 * \param createInfo The create info to chain into
		 */
		void ChainTo(VkGraphicsPipelineCreateInfo& createInfo);

		/**
		 * \brief Chains the feedback create info into a compute pipeline create info
		 *
		 * Does nothing when the feature is unsupported.
		 *
		 * \param createInfo The create info to chain into
		 */
		void ChainTo(VkComputePipelineCreateInfo& createInfo);

		/**
		 * \brief Converts what the driver wrote into a PipelineCreationFeedback
		 *
		 * Must be called after the vkCreate*Pipelines call this collector was chained into
		 * returned, otherwise the reported feedback is meaningless.
		 *
		 * \return The collected feedback
		 */
		PipelineCreationFeedback Collect() const;

	private:

		bool mEnabled;
		unsigned int mStageCount;
		VkPipelineCreationFeedbackEXT mOverallFeedback;
		VkPipelineCreationFeedbackEXT mStageFeedbacks[MaxPipelineStageCreationFeedbacks];
		VkPipelineCreationFeedbackCreateInfoEXT mCreateInfo;
	};
}

#endif // COCKTAILENGINE_VULKAN_PIPELINE_PIPELINECREATIONFEEDBACK_HPP
