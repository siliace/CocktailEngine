#include <Cocktail/Vulkan/Queue/QueueFamilyContext.hpp>
#include <Cocktail/Vulkan/WSI/WSI.hpp>

namespace Ck::Vulkan
{
	QueueFamilyContext::QueueFamilyContext(VkPhysicalDevice physicalDevice) :
		mPhysicalDevice(physicalDevice)
	{
		unsigned int propertyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &propertyCount, nullptr);

		Array<VkQueueFamilyProperties> properties(propertyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &propertyCount, properties.GetData());

		QueueFamily graphicQueueFamily = SelectGraphicQueueFamily(properties);
		mFamilies[Renderer::CommandQueueType::Graphic] = graphicQueueFamily;
		if (propertyCount == 1)
		{
			mFamilies[Renderer::CommandQueueType::Transfer] = QueueFamily(Renderer::CommandQueueType::Transfer, graphicQueueFamily.GetIndex(), graphicQueueFamily.GetProperty());
			mFamilies[Renderer::CommandQueueType::Compute] = QueueFamily(Renderer::CommandQueueType::Compute, graphicQueueFamily.GetIndex(), graphicQueueFamily.GetProperty());

			mUnified = true;
		}
		else
		{
			mFamilies[Renderer::CommandQueueType::Transfer] = SelectTransferQueueFamily(properties);
			mFamilies[Renderer::CommandQueueType::Compute] = SelectComputeQueueFamily(properties);

			mUnified = false;
		}
	}

	bool QueueFamilyContext::IsUnified() const
	{
		return mUnified;
	}

	Array<unsigned int> QueueFamilyContext::FindFamilyIndexes(bool unique) const
	{
		Array<unsigned int> queueIndexes;
		for (Renderer::CommandQueueType queueFamilyType : Enum<Renderer::CommandQueueType>::Values)
		{
			const QueueFamily& family = GetFamily(queueFamilyType);
			unsigned int queueFamilyIndex = family.GetIndex();

			if (unique)
			{
				auto it = std::find(queueIndexes.begin(), queueIndexes.end(), queueFamilyIndex);
				if (it != queueIndexes.end())
					continue;
			}

			queueIndexes.Add(queueFamilyIndex);
		}

		return queueIndexes;
	}

	const QueueFamily& QueueFamilyContext::GetFamily(Renderer::CommandQueueType type) const
	{
		return mFamilies[type];
	}

	const QueueFamily& QueueFamilyContext::GetPresentationQueueFamily() const
	{
		for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
		{
			const QueueFamily& queueFamily = GetFamily(queueType);
			if (WSI::GetPhysicalDevicePresentationSupport(mPhysicalDevice, queueFamily.GetIndex()))
				return queueFamily;
		}

		COCKTAIL_UNREACHABLE();
	}

	QueueFamily QueueFamilyContext::SelectGraphicQueueFamily(const Array<VkQueueFamilyProperties>& properties)
	{
		for (unsigned int i = 0; i < properties.GetSize(); i++)
		{
			const VkQueueFamilyProperties& property = properties[i];
			if (property.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				return QueueFamily(Renderer::CommandQueueType::Graphic, i, properties[i]);
		}

		COCKTAIL_UNREACHABLE();
	}

	QueueFamily QueueFamilyContext::SelectTransferQueueFamily(const Array<VkQueueFamilyProperties>& properties)
	{
		Optional<unsigned int> transferFamilyIndex;

		// First look for a family dedicated to transfer
		for (unsigned int i = 0; i < properties.GetSize() && transferFamilyIndex.IsEmpty(); i++)
		{
			const VkQueueFamilyProperties& property = properties[i];
			if ((property.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) == 0)
				transferFamilyIndex = Optional<unsigned int>::Of(i);
		}

		unsigned int queueIndex = transferFamilyIndex.GetOrElse([&]() {
			for (unsigned int i = 0; i < properties.GetSize(); i++)
			{
				const VkQueueFamilyProperties& property = properties[i];
				if ((property.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)
					return i;
			}

			for (unsigned int i = 0; i < properties.GetSize(); i++)
			{
				const VkQueueFamilyProperties& property = properties[i];
				if (property.queueFlags & VK_QUEUE_TRANSFER_BIT)
					return i;
			}

			COCKTAIL_UNREACHABLE();
		});

		return QueueFamily(Renderer::CommandQueueType::Transfer, queueIndex, properties[queueIndex]);
	}

	QueueFamily QueueFamilyContext::SelectComputeQueueFamily(const Array<VkQueueFamilyProperties>& properties)
	{
		Optional<unsigned int> computeFamilyIndex;

		// First look for a family dedicated to compute
		for (unsigned int i = 0; i < properties.GetSize() && computeFamilyIndex.IsEmpty(); i++)
		{
			const VkQueueFamilyProperties& property = properties[i];
			if ((property.queueFlags & VK_QUEUE_COMPUTE_BIT) > 0 && (property.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)
				computeFamilyIndex = Optional<unsigned int>::Of(i);
		}

		unsigned int queueIndex = computeFamilyIndex.GetOrElse([&]() {
			// If there is no dedicated compute family, fallback on the first family with compute capabilities
			for (unsigned int i = 0; i < properties.GetSize() && computeFamilyIndex.IsEmpty(); i++)
			{
				const VkQueueFamilyProperties& property = properties[i];
				if (property.queueFlags & VK_QUEUE_COMPUTE_BIT)
					return i;
			}

			COCKTAIL_UNREACHABLE();
		});

		return QueueFamily(Renderer::CommandQueueType::Compute, queueIndex, properties[queueIndex]);
	}
}
