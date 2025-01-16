#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorSet.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorSetAllocator.hpp>

namespace Ck::Vulkan
{
	DescriptorSetAllocator::DescriptorSetAllocator(const Ref<RenderDevice>& renderDevice, const DescriptorSetAllocatorCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mAllocationCallbacks(allocationCallbacks)
	{
		DescriptorSetAllocator::SetObjectName(createInfo.Name);
	}

	DescriptorSetAllocator::~DescriptorSetAllocator()
	{
		mAcquiredSets.clear();
		mVacantSets.clear();
		for (const Ref<DescriptorPool>& descriptorPool : mDescriptorPools)
			descriptorPool->Reset();
	}

	void DescriptorSetAllocator::SetObjectName(const char* name) const
	{
	}

	Ref<Renderer::RenderDevice> DescriptorSetAllocator::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Ref<DescriptorSet> DescriptorSetAllocator::CreateDescriptorSet(const DescriptorSetCreateInfo& createInfo)
	{
		auto it = std::find_if(mVacantSets.begin(), mVacantSets.end(), [&](const Ref<DescriptorSet>& set) {
			return createInfo.Layout->IsCompatibleWith(*set->GetLayout());
		});

		Ref<DescriptorSet> descriptorSet;
		if (it != mVacantSets.end())
		{
			descriptorSet = std::move(*it);

			mVacantSets.erase(it);
		}
		else
		{
			Ref<DescriptorPool> descriptorPool = CreateDescriptorPool(createInfo.Layout);

			Ref<DescriptorSet> descriptorSets[MaxSetPerDescriptorPool];
			for (unsigned int i = 0; i < MaxSetPerDescriptorPool; i++)
				descriptorSets[i] = mDescriptorSetPool.Allocate(mRenderDevice, descriptorPool, createInfo, mAllocationCallbacks);

			descriptorSet = descriptorSets[0];
			for (unsigned int i = 1; i < MaxSetPerDescriptorPool; i++)
				mVacantSets.push_back(std::move(descriptorSets[i]));
		}

		mAcquiredSets.push_back(descriptorSet);

		return descriptorSet;
	}

	void DescriptorSetAllocator::Reset()
	{
		for (Ref<DescriptorSet>& descriptorSet : mAcquiredSets)
			mVacantSets.push_back(std::move(descriptorSet));

		mAcquiredSets.clear();
	}

	Ref<DescriptorPool> DescriptorSetAllocator::CreateDescriptorPool(const Ref<DescriptorSetLayout>& layout)
	{
		DescriptorPoolCreateInfo createInfo;
		createInfo.MaxSet = MaxSetPerDescriptorPool;
		createInfo.LayoutSignature = layout->ToSignature();

		Ref<DescriptorPool> descriptorPool = mRenderDevice->CreateDescriptorPool(createInfo);
		mDescriptorPools.push_back(descriptorPool);

		return descriptorPool;
	}
}
