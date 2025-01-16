#ifndef COCKTAIL_VULKAN_QUEUE_QUEUEFAMILY_HPP
#define COCKTAIL_VULKAN_QUEUE_QUEUEFAMILY_HPP

#include <Cocktail/Renderer/Command/CommandQueueType.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class QueueFamily
	{
	public:

		/**
		 * \brief 
		 */
		QueueFamily() = default;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::CommandQueueType GetType() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetIndex() const;

		/**
		 * \brief 
		 * \return 
		 */
		const VkQueueFamilyProperties& GetProperty() const;

	private:

		friend class QueueFamilyContext;

		/**
		 * \brief 
		 * \param type 
		 * \param index 
		 * \param property 
		 */
		QueueFamily(Renderer::CommandQueueType type, unsigned int index, const VkQueueFamilyProperties& property);

		Renderer::CommandQueueType mType;
		unsigned int mIndex;
		VkQueueFamilyProperties mProperty;
	};
}

#endif // COCKTAIL_VULKAN_QUEUE_QUEUEFAMILY_HPP

