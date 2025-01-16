#ifndef COCKTAIL_VULKAN_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP
#define COCKTAIL_VULKAN_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP

#include <Cocktail/Core/Utility/EnumMap.hpp>
#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Renderer/Command/Allocator/CommandListPool.hpp>
#include <Cocktail/Renderer/Command/Allocator/CommandListPoolCreateInfo.hpp>

#include <Cocktail/Vulkan/Command/Allocator/CommandPool.hpp>
#include <Cocktail/Vulkan/Command/Staging/StagingAllocator.hpp>

namespace Ck::Vulkan
{
	class CommandList;
	class DescriptorSetAllocator;
	class RenderDevice;

	/**
	 * \brief 
	 */
	class CommandListPool : public Inherit<CommandListPool, Object, Renderer::CommandListPool>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		CommandListPool(Ref<RenderDevice> renderDevice, const Renderer::CommandListPoolCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~CommandListPool() override;

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
		 * \param alignment
		 * \param length 
		 * \return 
		 */
		Ref<StagingBuffer> AcquireStagingBuffer(std::size_t alignment, std::size_t length) const;

		/**
		 * \brief 
		 * \param createInfo 
		 */
		Ref<Renderer::CommandList> CreateCommandList(const Renderer::CommandListCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \param releaseResources 
		 */
		void Reset(bool releaseResources) override;

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

	private:

		friend class CommandList;

		/**
		 * \brief 
		 * \param queueType 
		 * \return 
		 */
		Ref<CommandPool> GetCommandPool(Renderer::CommandQueueType queueType) const;

		Ref<RenderDevice> mRenderDevice;
		EnumMap<Renderer::CommandQueueType, Ref<CommandPool>> mCommandPools;
		Ref<DescriptorSetAllocator> mDescriptorSetAllocator;
		Ref<StagingAllocator> mStagingAllocator;
		bool mTransient;
		bool mCommandListResetable;
		Signal<> mOnReset;
	};
}

#endif // COCKTAIL_VULKAN_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP
