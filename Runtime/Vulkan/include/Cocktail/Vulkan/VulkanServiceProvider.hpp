#ifndef COCKTAIL_VULKAN_VULKANSERVICEPROVIDER_HPP
#define COCKTAIL_VULKAN_VULKANSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/Application.hpp>

#include <Cocktail/Vulkan/Export.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class COCKTAIL_VULKAN_API VulkanServiceProvider : public Extends<VulkanServiceProvider, ServiceProvider>
	{
	public:

		/**
		 * \brief
		 * \param application
		 */
		explicit VulkanServiceProvider(const Ref<Application>& application);

	protected:

		/**
		 * \brief 
		 * \param application 
		 */
		void DoRegister(const Ref<Application>& application) override;

		/**
		 * \brief 
		 * \param application 
		 */
		void DoBoot(const Ref<Application>& application) override;
	};
}

#endif // COCKTAIL_VULKAN_VULKANSERVICEPROVIDER_HPP
