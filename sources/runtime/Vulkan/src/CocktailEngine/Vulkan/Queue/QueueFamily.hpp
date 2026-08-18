#ifndef COCKTAILENGINE_VULKAN_QUEUE_QUEUEFAMILY_HPP
#define COCKTAILENGINE_VULKAN_QUEUE_QUEUEFAMILY_HPP

#include <CocktailEngine/Renderer/Command/CommandQueueType.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>

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

#endif // COCKTAILENGINE_VULKAN_QUEUE_QUEUEFAMILY_HPP

