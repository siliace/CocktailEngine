#ifndef COCKTAILENGINE_VULKAN_VULKANSERVICEPROVIDER_HPP
#define COCKTAILENGINE_VULKAN_VULKANSERVICEPROVIDER_HPP

#include <CocktailEngine/Core/Application/Application.hpp>

#include <CocktailEngine/Vulkan/Export.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_VULKAN_API VulkanServiceProvider : public ServiceProvider
	{
	public:

		/**
		 * \brief
		 * \param application
		 */
		explicit VulkanServiceProvider(Application* application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoRegister(Application* application) override;
	};
}

#endif // COCKTAILENGINE_VULKAN_VULKANSERVICEPROVIDER_HPP
