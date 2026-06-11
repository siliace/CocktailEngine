#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORSETALLOCATOR_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORSETALLOCATOR_HPP

#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Vulkan/DescriptorSet/DescriptorSetCreateInfo.hpp>

namespace Ck::Vulkan
{
	class DescriptorPool;
	class DescriptorSet;
	class RenderDevice;

	class DescriptorSetAllocator 
	{
	public:

		/**
		 * \brief 
		 */
		static constexpr unsigned int MaxSetPerDescriptorPool = 16;

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit DescriptorSetAllocator(RenderDevice* renderDevice);

		/**
		 * \brief 
		 */
		~DescriptorSetAllocator();

		/**
		 * \brief 
		 * \param createInfo
		 * \param stateHash
		 * \param cached 
		 * \return 
		 */
		SharedPtr<DescriptorSet> CreateDescriptorSet(const DescriptorSetCreateInfo& createInfo, Uint64 stateHash, bool& cached);

	private:

		/**
		 * \brief 
		 * \param layout 
		 * \return 
		 */
		SharedPtr<DescriptorPool> CreateDescriptorPool(SharedPtr<DescriptorSetLayout> layout);

		RenderDevice* mRenderDevice;
		Array<SharedPtr<DescriptorPool>> mDescriptorPools;
		HashMap<Uint64, SharedPtr<DescriptorSet>> mAcquiredSets;
		Array<SharedPtr<DescriptorSet>> mVacantSets;
		TlsObjectPool<DescriptorSet> mDescriptorSetPool;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORSETALLOCATOR_HPP
