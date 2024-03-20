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

	class DescriptorSetAllocator : public Inherit<DescriptorSetAllocator, Object, Renderer::RenderDeviceObject>
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
		DescriptorSetAllocator(const Ref<RenderDevice>& renderDevice, const DescriptorSetAllocatorCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<DescriptorSet> CreateDescriptorSet(const DescriptorSetCreateInfo& createInfo);

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
		Ref<DescriptorPool> CreateDescriptorPool(const Ref<DescriptorSetLayout>& layout);

		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		std::vector<Ref<DescriptorPool>> mDescriptorPools;
		std::vector<Ref<DescriptorSet>> mAcquiredSets;
		std::vector<Ref<DescriptorSet>> mVacantSets;
		TlsObjectPool<DescriptorSet> mDescriptorSetPool;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORSETALLOCATOR_HPP
