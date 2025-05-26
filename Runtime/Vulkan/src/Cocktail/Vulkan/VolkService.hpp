#ifndef COCKTAIL_VULKAN_VOLKSERVICE_HPP
#define COCKTAIL_VULKAN_VOLKSERVICE_HPP

#include <Cocktail/Vulkan/Volk.hpp>

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

#endif // COCKTAIL_VULKAN_VOLKSERVICE_HPP
