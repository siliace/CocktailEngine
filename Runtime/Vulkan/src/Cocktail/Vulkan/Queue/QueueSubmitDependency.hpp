#ifndef COCKTAIL_VULKAN_QUEUE_QUEUESUBMITDEPENDENCY_HPP
#define COCKTAIL_VULKAN_QUEUE_QUEUESUBMITDEPENDENCY_HPP

#include <Cocktail/Vulkan/Semaphore.hpp>

namespace Ck::Vulkan
{
	struct QueueSubmitDependency
	{
		Ref<Semaphore> WaitSemaphore;
		VkPipelineStageFlags WaitDstStages;
	};
}

#endif // COCKTAIL_VULKAN_QUEUE_QUEUESUBMITDEPENDENCY_HPP
