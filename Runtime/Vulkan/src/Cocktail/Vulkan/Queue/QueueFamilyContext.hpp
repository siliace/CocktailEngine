#ifndef COCKTAIL_VULKAN_QUEUE_QUEUEFAMILYCONTEXT_HPP
#define COCKTAIL_VULKAN_QUEUE_QUEUEFAMILYCONTEXT_HPP

#include <vector>

#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Renderer/Command/CommandQueueType.hpp>

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/Queue/QueueFamily.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class QueueFamilyContext
	{
	public:

		/**
		 * \brief 
		 * \param physicalDevice
		 */
		explicit QueueFamilyContext(VkPhysicalDevice physicalDevice);

		/**
		 * \brief Tell whether the queue context is unified
		 * A QueueFamilyContext is unified when all its families are the same
		 * \return
		 */
		bool IsUnified() const;
		
		/**
		 * \brief 
		 * \param unique 
		 * \return 
		 */
		std::vector<unsigned int> FindFamilyIndexes(bool unique = true) const;

		/**
		 * \brief 
		 * \tparam Callable 
		 * \param callable 
		 * \param unique 
		 * \return 
		 */
		template <typename Callable>
		std::vector<unsigned int> FindFamilyIndexes(Callable&& callable, bool unique = true) const
		{
			std::vector<unsigned int> queueIndexes;
			for (Renderer::CommandQueueType queueFamilyType : Enum<Renderer::CommandQueueType>::Values)
			{
				const QueueFamily& family = GetFamily(queueFamilyType);
				unsigned int queueFamilyIndex = family.GetIndex();

				if (!callable(family))
					continue;

				if (unique)
				{
					auto it = std::find(queueIndexes.begin(), queueIndexes.end(), queueFamilyIndex);
					if (it != queueIndexes.end())
						continue;
				}
					
				queueIndexes.push_back(queueFamilyIndex);
			}

			return queueIndexes;
		}

		/**
		 * \brief 
		 * \param type 
		 * \return 
		 */
		const QueueFamily& GetFamily(Renderer::CommandQueueType type) const;

		/**
		 * \brief 
		 * \return 
		 */
		const QueueFamily& GetPresentationQueueFamily() const;

	private:

		/**
		 * \brief
		 * \param properties
		 * \return 
		 */
		static QueueFamily SelectGraphicQueueFamily(const std::vector<VkQueueFamilyProperties>& properties);

		/**
		 * \brief
		 * \param properties
		 * \return
		 */
		static QueueFamily SelectTransferQueueFamily(const std::vector<VkQueueFamilyProperties>& properties);

		/**
		 * \brief 
		 * \param properties 
		 * \return 
		 */
		static QueueFamily SelectComputeQueueFamily(const std::vector<VkQueueFamilyProperties>& properties);

		VkPhysicalDevice mPhysicalDevice;
		bool mUnified;
		EnumMap<Renderer::CommandQueueType, QueueFamily> mFamilies;
	};
}

#endif // COCKTAIL_VULKAN_QUEUE_QUEUEFAMILYCONTEXT_HPP
