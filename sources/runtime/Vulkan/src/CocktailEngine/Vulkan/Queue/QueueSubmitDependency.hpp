#ifndef COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITDEPENDENCY_HPP
#define COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITDEPENDENCY_HPP

#include <CocktailEngine/Vulkan/Semaphore.hpp>

namespace Ck::Vulkan
{
	struct QueueSubmitDependency
	{
		SharedPtr<Semaphore> WaitSemaphore;
		VkPipelineStageFlags WaitDstStages;
	};
}

#endif // COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITDEPENDENCY_HPP
