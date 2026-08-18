#ifndef COCKTAILENGINE_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOL_HPP
#define COCKTAILENGINE_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOL_HPP

#include <CocktailEngine/Renderer/RenderDeviceObject.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>
#include <CocktailEngine/Vulkan/Command/Allocator/CommandPoolCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class CommandPool : public Renderer::RenderDeviceObject
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param createInfo
		 * \param allocationCallbacks
		 */
		CommandPool(RenderDevice* renderDevice, const CommandPoolCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Renderer::RenderDevice* GetRenderDevice() const override;

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

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkCommandPool mHandle;
		Renderer::CommandQueueType mQueueType;
		bool mTransient;
		bool mCommandListResetable;
	};
}

#endif // COCKTAILENGINE_VULKAN_COMMANDLIST_ALLOCATOR_COMMANDPOOL_HPP
