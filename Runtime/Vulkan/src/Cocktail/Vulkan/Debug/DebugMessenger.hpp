#ifndef COCKTAIL_VULKAN_DEBUG_DEBUGMESSENGER_HPP
#define COCKTAIL_VULKAN_DEBUG_DEBUGMESSENGER_HPP

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/Debug/DebugMessengerCreateInfo.hpp>

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
		DebugMessenger(std::shared_ptr<RenderDevice> renderDevice, const DebugMessengerCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		std::shared_ptr<RenderDevice> GetRenderDevice() const;

	private:

		std::shared_ptr<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkDebugUtilsMessengerEXT mHandle;
	};
}

#endif // COCKTAIL_VULKAN_DEBUG_DEBUGMESSENGER_HPP
