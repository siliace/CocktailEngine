#ifndef COCKTAILENGINE_VULKAN_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP
#define COCKTAILENGINE_VULKAN_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP

#include <CocktailEngine/Core/Utility/EnumMap.hpp>

#include <CocktailEngine/Vulkan/Command/Allocator/CommandPool.hpp>
#include <CocktailEngine/Vulkan/Command/Staging/StagingAllocator.hpp>
#include <CocktailEngine/Vulkan/DescriptorSet/Allocator/DescriptorSetAllocator.hpp>

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
	class CommandListPool : public SharedFromThis<CommandListPool>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		CommandListPool(RenderDevice* renderDevice, const CommandListPoolCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		SharedPtr<CommandList> CreateCommandList(const Renderer::CommandListCreateInfo& createInfo);

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
		SharedPtr<CommandPool> GetCommandPool(Renderer::CommandQueueType queueType) const;

		RenderDevice* mRenderDevice;
		EnumMap<Renderer::CommandQueueType, SharedPtr<CommandPool>> mCommandPools;
		DescriptorSetAllocator mDescriptorSetAllocator;
		UniquePtr<StagingAllocator> mStagingAllocator;
		bool mTransient;
		bool mCommandListResetable;
		Signal<> mOnReset;
	};
}

#endif // COCKTAILENGINE_VULKAN_COMMAND_ALLOCATOR_COMMANDLISTPOOL_HPP
