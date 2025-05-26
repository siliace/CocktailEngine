#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORSETALLOCATOR_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORSETALLOCATOR_HPP

#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Vulkan/DescriptorSet/DescriptorSetCreateInfo.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorSetAllocatorCreateInfo.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorPool.hpp>

namespace Ck::Vulkan
{
	class DescriptorSet;
	class RenderDevice;

	class DescriptorSetAllocator : public Renderer::RenderDeviceObject
	{
	public:

		/**
		 * \brief 
		 */
		static constexpr unsigned int MaxSetPerDescriptorPool = 16;

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		DescriptorSetAllocator(std::shared_ptr<RenderDevice> renderDevice, const DescriptorSetAllocatorCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~DescriptorSetAllocator() override;

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
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<DescriptorSet> CreateDescriptorSet(const DescriptorSetCreateInfo& createInfo);

		/**
		 * \brief 
		 */
		void Reset();

	private:

		/**
		 * \brief 
		 * \param layout 
		 * \return 
		 */
		std::shared_ptr<DescriptorPool> CreateDescriptorPool(std::shared_ptr<DescriptorSetLayout> layout);

		std::shared_ptr<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		std::vector<std::shared_ptr<DescriptorPool>> mDescriptorPools;
		std::vector<std::shared_ptr<DescriptorSet>> mAcquiredSets;
		std::vector<std::shared_ptr<DescriptorSet>> mVacantSets;
		TlsObjectPool<DescriptorSet> mDescriptorSetPool;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORSETALLOCATOR_HPP
