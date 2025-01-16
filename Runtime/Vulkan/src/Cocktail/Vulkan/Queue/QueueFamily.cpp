#include <Cocktail/Vulkan/Queue/QueueFamily.hpp>

namespace Ck::Vulkan
{
	Renderer::CommandQueueType QueueFamily::GetType() const
	{
		return mType;
	}

	unsigned int QueueFamily::GetIndex() const
	{
		return mIndex;
	}

	const VkQueueFamilyProperties& QueueFamily::GetProperty() const
	{
		return mProperty;
	}

	QueueFamily::QueueFamily(Renderer::CommandQueueType type, unsigned int index, const VkQueueFamilyProperties& property) :
		mType(type),
		mIndex(index),
		mProperty(property)
	{
		/// Nothing
	}
}
