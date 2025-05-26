#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorSet.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorSetAllocator.hpp>

namespace Ck::Vulkan
{
	DescriptorSetAllocator::DescriptorSetAllocator(std::shared_ptr<RenderDevice> renderDevice, const DescriptorSetAllocatorCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mAllocationCallbacks(allocationCallbacks)
	{
		DescriptorSetAllocator::SetObjectName(createInfo.Name);
	}

	DescriptorSetAllocator::~DescriptorSetAllocator()
	{
		mAcquiredSets.clear();
		mVacantSets.clear();
		for (std::shared_ptr<DescriptorPool> descriptorPool : mDescriptorPools)
			descriptorPool->Reset();
	}

	void DescriptorSetAllocator::SetObjectName(const char* name) const
	{
	}

	std::shared_ptr<Renderer::RenderDevice> DescriptorSetAllocator::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	std::shared_ptr<DescriptorSet> DescriptorSetAllocator::CreateDescriptorSet(const DescriptorSetCreateInfo& createInfo)
	{
		auto it = std::find_if(mVacantSets.begin(), mVacantSets.end(), [&](std::shared_ptr<DescriptorSet> set) {
			return createInfo.Layout->IsCompatibleWith(*set->GetLayout());
		});

		std::shared_ptr<DescriptorSet> descriptorSet;
		if (it != mVacantSets.end())
		{
			descriptorSet = std::move(*it);

			mVacantSets.erase(it);
		}
		else
		{
			std::shared_ptr<DescriptorPool> descriptorPool = CreateDescriptorPool(createInfo.Layout);

			std::shared_ptr<DescriptorSet> descriptorSets[MaxSetPerDescriptorPool];
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
		for (std::shared_ptr<DescriptorSet>& descriptorSet : mAcquiredSets)
			mVacantSets.push_back(std::move(descriptorSet));

		mAcquiredSets.clear();
	}

	std::shared_ptr<DescriptorPool> DescriptorSetAllocator::CreateDescriptorPool(std::shared_ptr<DescriptorSetLayout> layout)
	{
		DescriptorPoolCreateInfo createInfo;
		createInfo.MaxSet = MaxSetPerDescriptorPool;
		createInfo.LayoutSignature = layout->ToSignature();

		std::shared_ptr<DescriptorPool> descriptorPool = mRenderDevice->CreateDescriptorPool(createInfo);
		mDescriptorPools.push_back(descriptorPool);

		return descriptorPool;
	}
}
