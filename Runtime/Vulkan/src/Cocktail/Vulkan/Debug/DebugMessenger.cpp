#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Debug/DebugMessenger.hpp>

namespace Ck::Vulkan
{
	DebugMessenger::DebugMessenger(Ref<RenderDevice> renderDevice, const DebugMessengerCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		VkDebugUtilsMessengerCreateInfoEXT vkCreateInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			vkCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
			vkCreateInfo.pfnUserCallback = &DebugMessenger::MessageCallback;
			vkCreateInfo.pUserData = this;
		}

		vkCreateDebugUtilsMessengerEXT(mRenderDevice->GetInstanceHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle);

		DebugMessenger::SetObjectName(createInfo.Name);
	}

	DebugMessenger::~DebugMessenger()
	{
		vkDestroyDebugUtilsMessengerEXT(mRenderDevice->GetInstanceHandle(), mHandle, mAllocationCallbacks);
	}

	void DebugMessenger::SetObjectName(const char* name) const
	{
	}

	Ref<Renderer::RenderDevice> DebugMessenger::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	VkBool32 DebugMessenger::MessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		Ref<Renderer::RenderDevice> renderDevice = static_cast<DebugMessenger*>(pUserData)->GetRenderDevice();
		renderDevice->OnDebugMessage().Emit(pCallbackData->pMessage);

		return VK_FALSE;
	}
}
