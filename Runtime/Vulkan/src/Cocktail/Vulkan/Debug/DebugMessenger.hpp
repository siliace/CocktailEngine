#ifndef COCKTAIL_VULKAN_DEBUG_DEBUGMESSENGER_HPP
#define COCKTAIL_VULKAN_DEBUG_DEBUGMESSENGER_HPP

#include <volk.h>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

#include <Cocktail/Vulkan/Debug/DebugMessengerCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class DebugMessenger : public Inherit<DebugMessenger, Object, Renderer::RenderDeviceObject>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		DebugMessenger(Ref<RenderDevice> renderDevice, const DebugMessengerCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~DebugMessenger() override;

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

	private:

		/**
		 * \brief 
		 * \param messageSeverity 
		 * \param messageTypes 
		 * \param pCallbackData 
		 * \param pUserData 
		 * \return 
		 */
		static VkBool32 VKAPI_PTR MessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkDebugUtilsMessengerEXT mHandle;
	};
}

#endif // COCKTAIL_VULKAN_DEBUG_DEBUGMESSENGER_HPP
