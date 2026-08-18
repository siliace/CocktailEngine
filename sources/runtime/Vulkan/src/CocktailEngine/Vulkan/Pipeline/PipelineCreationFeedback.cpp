#include <cassert>

#include <CocktailEngine/Vulkan/RenderDevice.hpp>
#include <CocktailEngine/Vulkan/VulkanUtils.hpp>
#include <CocktailEngine/Vulkan/Pipeline/PipelineCreationFeedback.hpp>

namespace Ck::Vulkan
{
	namespace
	{
		PipelineCreationFeedbackEntry FromVkType(const VkPipelineCreationFeedbackEXT& feedback)
		{
			PipelineCreationFeedbackEntry entry;

			// A driver that reports nothing leaves every other bit unset, and the duration it did
			// not measure must not be read as a measure of zero.
			if (!(feedback.flags & VK_PIPELINE_CREATION_FEEDBACK_VALID_BIT_EXT))
				return entry;

			entry.Valid = true;
			entry.ApplicationCacheHit = (feedback.flags & VK_PIPELINE_CREATION_FEEDBACK_APPLICATION_PIPELINE_CACHE_HIT_BIT_EXT) != 0;
			entry.BasePipelineAcceleration = (feedback.flags & VK_PIPELINE_CREATION_FEEDBACK_BASE_PIPELINE_ACCELERATION_BIT_EXT) != 0;
			entry.Duration = feedback.duration;

			return entry;
		}
	}

	PipelineCreationFeedbackCollector::PipelineCreationFeedbackCollector(const RenderDevice* renderDevice, unsigned int stageCount) :
		mEnabled(renderDevice->IsFeatureSupported(RenderDeviceFeature::PipelineCreationFeedback)),
		mStageCount(stageCount),
		mOverallFeedback{},
		mStageFeedbacks{},
		mCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_CREATION_FEEDBACK_CREATE_INFO_EXT, nullptr }
	{
		assert(stageCount <= MaxPipelineStageCreationFeedbacks);

		// Vulkan requires the per stage count to match the stage count of the pipeline exactly, so
		// the array is handed over whole rather than clamped.
		mCreateInfo.pPipelineCreationFeedback = &mOverallFeedback;
		mCreateInfo.pipelineStageCreationFeedbackCount = mStageCount;
		mCreateInfo.pPipelineStageCreationFeedbacks = mStageFeedbacks;
	}

	void PipelineCreationFeedbackCollector::ChainTo(VkGraphicsPipelineCreateInfo& createInfo)
	{
		if (!mEnabled)
			return;

		assert(createInfo.stageCount == mStageCount);

		Chain(createInfo, mCreateInfo);
	}

	void PipelineCreationFeedbackCollector::ChainTo(VkComputePipelineCreateInfo& createInfo)
	{
		if (!mEnabled)
			return;

		assert(mStageCount == 1);

		Chain(createInfo, mCreateInfo);
	}

	PipelineCreationFeedback PipelineCreationFeedbackCollector::Collect() const
	{
		PipelineCreationFeedback feedback;
		if (!mEnabled)
			return feedback;

		feedback.Overall = FromVkType(mOverallFeedback);
		feedback.StageCount = mStageCount;
		for (unsigned int i = 0; i < mStageCount; i++)
			feedback.Stages[i] = FromVkType(mStageFeedbacks[i]);

		return feedback;
	}
}
