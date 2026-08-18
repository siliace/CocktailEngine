#ifndef COCKTAILENGINE_VULKAN_VOLKSERVICE_HPP
#define COCKTAILENGINE_VULKAN_VOLKSERVICE_HPP

#include <CocktailEngine/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class VolkService
	{
	public:

		/**
		 * \brief 
		 */
		VolkService();

		/**
		 * \brief 
		 * \param instance 
		 */
		void LoadInstanceOnly(VkInstance instance) const;

		/**
		 * \brief 
		 * \param device 
		 */
		void LoadDevice(VkDevice device) const;
	};
}

#endif // COCKTAILENGINE_VULKAN_VOLKSERVICE_HPP
