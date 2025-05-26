#ifndef COCKTAIL_VULKAN_COMMAND_FENCE_HPP
#define COCKTAIL_VULKAN_COMMAND_FENCE_HPP

#include <Cocktail/Renderer/Command/Synchronization/Fence.hpp>
#include <Cocktail/Renderer/Command/Synchronization/FenceCreateInfo.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class Fence : public Renderer::Fence, public std::enable_shared_from_this<Fence>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Fence(std::shared_ptr<RenderDevice> renderDevice, const Renderer::FenceCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		std::shared_ptr<Renderer::RenderDevice> GetRenderDevice() const override;

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

		std::shared_ptr<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkFence mHandle;
		bool mSignaled;
		Signal<> mOnSignaled;
	};
}

#endif // COCKTAIL_VULKAN_COMMAND_FENCE_HPP
