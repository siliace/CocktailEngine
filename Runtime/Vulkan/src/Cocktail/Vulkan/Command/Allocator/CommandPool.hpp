#ifndef COCKTAIL_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOL_HPP
#define COCKTAIL_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOL_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandPoolCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class CommandPool : public Inherit<CommandPool, Object, Renderer::RenderDeviceObject>
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param createInfo
		 * \param allocationCallbacks
		 */
		CommandPool(const Ref<RenderDevice>& renderDevice, const CommandPoolCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~CommandPool() override;

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

		/**
		 * \brief
		 * \param releaseResources 
		 */
		void Reset(bool releaseResources) const;

		/**
		 * \brief
		 * \return
		 */
		bool IsTransient() const;

		/**
		 * \brief
		 * \return
		 */
		bool IsCommandListResetable() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkCommandPool GetHandle() const;

	private:

		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkCommandPool mHandle;
		Renderer::CommandQueueType mQueueType;
		bool mTransient;
		bool mCommandListResetable;
	};
}

#endif // COCKTAIL_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOL_HPP
