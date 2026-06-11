#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorSet.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorPool.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorSetAllocator.hpp>

namespace Ck::Vulkan
{
	DescriptorSetAllocator::DescriptorSetAllocator(RenderDevice* renderDevice) :
		mRenderDevice(renderDevice)
	{
		/// Nothing
	}

	DescriptorSetAllocator::~DescriptorSetAllocator()
	{
		mAcquiredSets.Clear();
		mVacantSets.Clear();
		for (const SharedPtr<DescriptorPool>& descriptorPool : mDescriptorPools)
			descriptorPool->Reset();
	}

	SharedPtr<DescriptorSet> DescriptorSetAllocator::CreateDescriptorSet(const DescriptorSetCreateInfo& createInfo, Uint64 stateHash, bool& cached)
	{
		cached = true;
        Optional<SharedPtr<DescriptorSet>&> set = mAcquiredSets.TryGet(stateHash);
        if (!set.IsEmpty())
            return set.Get();

		cached = false;
		SharedPtr<DescriptorSet> descriptorSet = mVacantSets.FindIndexIf([&](const SharedPtr<DescriptorSet>& set) {
			return createInfo.Layout->IsCompatibleWith(*set->GetLayout());
		}).Map([&](unsigned int index) {
			return mVacantSets.RemoveAt(index);
		}).GetOrElse([&]() {
			SharedPtr<DescriptorPool> descriptorPool = CreateDescriptorPool(createInfo.Layout);

			SharedPtr<DescriptorSet> descriptorSets[MaxSetPerDescriptorPool];
			for (unsigned int i = 0; i < MaxSetPerDescriptorPool; i++)
				descriptorSets[i] = mDescriptorSetPool.Allocate(mRenderDevice, descriptorPool, createInfo, nullptr);

			for (unsigned int i = 1; i < MaxSetPerDescriptorPool; i++)
				mVacantSets.Add(std::move(descriptorSets[i]));

			return descriptorSets[0];
		});

		mAcquiredSets.Put(stateHash, descriptorSet);

		return descriptorSet;
	}

	SharedPtr<DescriptorPool> DescriptorSetAllocator::CreateDescriptorPool(SharedPtr<DescriptorSetLayout> layout)
	{
		DescriptorPoolCreateInfo createInfo;
		createInfo.MaxSet = MaxSetPerDescriptorPool;
		createInfo.LayoutSignature = layout->ToSignature();

		SharedPtr<DescriptorPool> descriptorPool = mRenderDevice->CreateDescriptorPool(createInfo);
		mDescriptorPools.Add(descriptorPool);

		return descriptorPool;
	}
}
