#ifndef COCKTAIL_VULKAN_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP
#define COCKTAIL_VULKAN_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP

#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Vulkan/Command/Allocator/CommandPool.hpp>
#include <Cocktail/Vulkan/Command/Staging/StagingAllocator.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorSetAllocator.hpp>

namespace Ck::Vulkan
{
	class CommandList;
	class RenderDevice;

	struct CommandListPoolCreateInfo
	{
		bool Transient = true;
		bool Reset = false;
	};

	/**
	 * \brief 
	 */
	class CommandListPool : public std::enable_shared_from_this<CommandListPool>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		CommandListPool(std::shared_ptr<RenderDevice> renderDevice, const CommandListPoolCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~CommandListPool();

		/**
		 * \brief 
		 * \param alignment
		 * \param length 
		 * \return 
		 */
		StagingBuffer* AcquireStagingBuffer(std::size_t alignment, std::size_t length) const;

		/**
		 * \brief 
		 * \param createInfo 
		 */
		std::shared_ptr<CommandList> CreateCommandList(const Renderer::CommandListCreateInfo& createInfo);

		/**
		 * \brief 
		 * \param releaseResources 
		 */
		void Reset(bool releaseResources);

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
		std::shared_ptr<CommandPool> GetCommandPool(Renderer::CommandQueueType queueType) const;

		std::shared_ptr<RenderDevice> mRenderDevice;
		EnumMap<Renderer::CommandQueueType, std::shared_ptr<CommandPool>> mCommandPools;
		DescriptorSetAllocator mDescriptorSetAllocator;
		std::unique_ptr<StagingAllocator> mStagingAllocator;
		bool mTransient;
		bool mCommandListResetable;
		Signal<> mOnReset;
	};
}

#endif // COCKTAIL_VULKAN_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP
