#ifndef COCKTAILENGINE_VULKAN_DEBUG_DEBUGMESSENGER_HPP
#define COCKTAILENGINE_VULKAN_DEBUG_DEBUGMESSENGER_HPP

#include <CocktailEngine/Vulkan/Volk.hpp>
#include <CocktailEngine/Vulkan/Debug/DebugMessengerCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief
	 */
	class DebugMessenger
	{
	public:

		/**
		 * \brief
		 * \param messageSeverity
		 * \param messageTypes
		 * \param pCallbackData
		 * \param pUserData
		 * \return
		 */
		static VkBool32 VKAPI_PTR MessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

		/**
		 * \brief
		 * \param renderDevice
		 * \param createInfo
		 * \param allocationCallbacks
		 */
		DebugMessenger(RenderDevice* renderDevice, const DebugMessengerCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief
		 */
		~DebugMessenger();

		/**
		 * \brief
		 * \param name
		 */
		void SetObjectName(const char* name) const;

		/**
		 * \brief
		 * \return
		 */
		RenderDevice* GetRenderDevice() const;

	private:

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkDebugUtilsMessengerEXT mHandle;
	};
}

#endif // COCKTAILENGINE_VULKAN_DEBUG_DEBUGMESSENGER_HPP
