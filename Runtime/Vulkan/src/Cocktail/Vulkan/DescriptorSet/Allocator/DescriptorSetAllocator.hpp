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
		explicit DescriptorSetAllocator(std::shared_ptr<RenderDevice> renderDevice);

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
		std::shared_ptr<DescriptorSet> CreateDescriptorSet(const DescriptorSetCreateInfo& createInfo, Uint64 stateHash, bool& cached);

	private:

		/**
		 * \brief 
		 * \param layout 
		 * \return 
		 */
		std::shared_ptr<DescriptorPool> CreateDescriptorPool(std::shared_ptr<DescriptorSetLayout> layout);

		std::shared_ptr<RenderDevice> mRenderDevice;
		Array<std::shared_ptr<DescriptorPool>> mDescriptorPools;
		std::unordered_map<Uint64, std::shared_ptr<DescriptorSet>> mAcquiredSets;
		Array<std::shared_ptr<DescriptorSet>> mVacantSets;
		TlsObjectPool<DescriptorSet> mDescriptorSetPool;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORSETALLOCATOR_HPP
