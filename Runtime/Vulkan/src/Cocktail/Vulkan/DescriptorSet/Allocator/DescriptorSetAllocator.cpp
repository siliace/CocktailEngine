#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorSet.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorPool.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorSetAllocator.hpp>

namespace Ck::Vulkan
{
	DescriptorSetAllocator::DescriptorSetAllocator(std::shared_ptr<RenderDevice> renderDevice) :
		mRenderDevice(std::move(renderDevice))
	{
		/// Nothing
	}

	DescriptorSetAllocator::~DescriptorSetAllocator()
	{
		mAcquiredSets.Clear();
		mVacantSets.Clear();
		for (const std::shared_ptr<DescriptorPool>& descriptorPool : mDescriptorPools)
			descriptorPool->Reset();
	}

	std::shared_ptr<DescriptorSet> DescriptorSetAllocator::CreateDescriptorSet(const DescriptorSetCreateInfo& createInfo)
	{
		std::shared_ptr<DescriptorSet> descriptorSet = mVacantSets.FindIndexIf([&](const std::shared_ptr<DescriptorSet> &set) {
			return createInfo.Layout->IsCompatibleWith(*set->GetLayout());
		}).Map([&](unsigned int index) {
			return mVacantSets.RemoveAt(index);
		}).GetOrElse([&]() {
			std::shared_ptr<DescriptorPool> descriptorPool = CreateDescriptorPool(createInfo.Layout);

			std::shared_ptr<DescriptorSet> descriptorSets[MaxSetPerDescriptorPool];
			for (unsigned int i = 0; i < MaxSetPerDescriptorPool; i++)
				descriptorSets[i] = mDescriptorSetPool.Allocate(mRenderDevice, descriptorPool, createInfo, nullptr);

			for (unsigned int i = 1; i < MaxSetPerDescriptorPool; i++)
				mVacantSets.Add(std::move(descriptorSets[i]));

			return descriptorSets[0];
		});

		mAcquiredSets.Add(descriptorSet);

		return descriptorSet;
	}

	void DescriptorSetAllocator::Reset()
	{
		for (std::shared_ptr<DescriptorSet>& descriptorSet : mAcquiredSets)
			mVacantSets.Add(std::move(descriptorSet));

		mAcquiredSets.Clear();
	}

	std::shared_ptr<DescriptorPool> DescriptorSetAllocator::CreateDescriptorPool(std::shared_ptr<DescriptorSetLayout> layout)
	{
		DescriptorPoolCreateInfo createInfo;
		createInfo.MaxSet = MaxSetPerDescriptorPool;
		createInfo.LayoutSignature = layout->ToSignature();

		std::shared_ptr<DescriptorPool> descriptorPool = mRenderDevice->CreateDescriptorPool(createInfo);
		mDescriptorPools.Add(descriptorPool);

		return descriptorPool;
	}
}
