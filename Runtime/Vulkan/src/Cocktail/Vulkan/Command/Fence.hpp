#ifndef COCKTAIL_VULKAN_COMMAND_FENCE_HPP
#define COCKTAIL_VULKAN_COMMAND_FENCE_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/Command/Synchronization/Fence.hpp>
#include <Cocktail/Renderer/Command/Synchronization/FenceCreateInfo.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class Fence : public Inherit<Fence, Object, Renderer::Fence>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Fence(const Ref<RenderDevice>& renderDevice, const Renderer::FenceCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~Fence() override;

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
		 */
		void Wait() override;

		/**
		 * \brief 
		 * \param timeout 
		 * \return 
		 */
		bool Wait(Uint64 timeout) override;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsSignaled() const override;

		/**
		 * \brief 
		 */
		void Reset() override;

		/**
		 * \brief 
		 * \return 
		 */
		VkFence GetHandle() const;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<>& OnSignaled() override;

	private:

		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkFence mHandle;
		bool mSignaled;
		Signal<> mOnSignaled;
	};
}

#endif // COCKTAIL_VULKAN_COMMAND_FENCE_HPP
