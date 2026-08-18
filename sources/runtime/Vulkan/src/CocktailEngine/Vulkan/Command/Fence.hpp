#ifndef COCKTAILENGINE_VULKAN_COMMAND_FENCE_HPP
#define COCKTAILENGINE_VULKAN_COMMAND_FENCE_HPP

#include <CocktailEngine/Renderer/Command/Synchronization/Fence.hpp>
#include <CocktailEngine/Renderer/Command/Synchronization/FenceCreateInfo.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class Fence : public Renderer::Fence, public SharedFromThis<Fence>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Fence(RenderDevice* renderDevice, const Renderer::FenceCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Renderer::RenderDevice* GetRenderDevice() const override;

		/**
		 * \brief 
		 */
		void Wait() override;

		/**
		 * \brief 
		 * \param timeout 
		 * \return 
		 */
		bool Wait(const Duration& timeout) override;

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

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkFence mHandle;
		bool mSignaled;
		Signal<> mOnSignaled;
	};
}

#endif // COCKTAILENGINE_VULKAN_COMMAND_FENCE_HPP
