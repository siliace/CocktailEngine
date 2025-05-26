#include <Cocktail/Core/Log/LogLevel.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Debug/DebugMessenger.hpp>

namespace Ck::Vulkan
{
	VkBool32 DebugMessenger::MessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		LogLevel logLevel;
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			logLevel = LogLevel::Debug;
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			logLevel = LogLevel::Info;
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			logLevel = LogLevel::Warning;
			break;

		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			logLevel = LogLevel::Error;
			break;
		}

		Renderer::MessageType messageType = {};
		switch (messageTypes)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			messageType = Renderer::MessageType::General;
			break;

		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			messageType = Renderer::MessageType::Validation;
			break;

		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			messageType = Renderer::MessageType::Performance;
			break;
		}

		static_cast<DebugMessenger*>(pUserData)->GetRenderDevice()->OnDebugMessage().Emit(logLevel, messageType, pCallbackData->pMessage);

		return VK_FALSE;
	}

	DebugMessenger::DebugMessenger(std::shared_ptr<RenderDevice> renderDevice, const DebugMessengerCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		VkDebugUtilsMessengerCreateInfoEXT vkCreateInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			vkCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
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

	std::shared_ptr<RenderDevice> DebugMessenger::GetRenderDevice() const
	{
		return mRenderDevice;
	}
}
