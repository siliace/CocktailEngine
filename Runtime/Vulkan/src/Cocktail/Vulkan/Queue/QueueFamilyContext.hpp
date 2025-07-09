#ifndef COCKTAIL_VULKAN_QUEUE_QUEUEFAMILYCONTEXT_HPP
#define COCKTAIL_VULKAN_QUEUE_QUEUEFAMILYCONTEXT_HPP

#include <Cocktail/Core/Array.hpp>
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
		Array<unsigned int> FindFamilyIndexes(bool unique = true) const;

		/**
		 * \brief 
		 * \tparam Callable 
		 * \param callable 
		 * \param unique 
		 * \return 
		 */
		template <typename Callable>
		Array<unsigned int> FindFamilyIndexes(Callable&& callable, bool unique = true) const
		{
			Array<unsigned int> queueIndexes;
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
					
				queueIndexes.Add(queueFamilyIndex);
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
		static QueueFamily SelectGraphicQueueFamily(const Array<VkQueueFamilyProperties>& properties);

		/**
		 * \brief
		 * \param properties
		 * \return
		 */
		static QueueFamily SelectTransferQueueFamily(const Array<VkQueueFamilyProperties>& properties);

		/**
		 * \brief 
		 * \param properties 
		 * \return 
		 */
		static QueueFamily SelectComputeQueueFamily(const Array<VkQueueFamilyProperties>& properties);

		VkPhysicalDevice mPhysicalDevice;
		bool mUnified;
		EnumMap<Renderer::CommandQueueType, QueueFamily> mFamilies;
	};
}

#endif // COCKTAIL_VULKAN_QUEUE_QUEUEFAMILYCONTEXT_HPP
