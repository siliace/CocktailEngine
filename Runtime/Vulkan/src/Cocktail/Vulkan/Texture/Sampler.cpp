#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Texture/Sampler.hpp>

namespace Ck::Vulkan
{
	Sampler::Sampler(const Ref<RenderDevice>& renderDevice, const Renderer::SamplerCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE),
		mMagFilter(createInfo.MagFilter),
		mMinFilter(createInfo.MinFilter),
		mMipmapMode(createInfo.MipmapMode),
		mAddressModeU(createInfo.AddressModeU),
		mAddressModeV(createInfo.AddressModeV),
		mAddressModeW(createInfo.AddressModeW),
		mAnisotropyEnable(createInfo.EnableAnisotropy),
		mAnisotropy(createInfo.MaxAnisotropy),
		mMinLoad(createInfo.MinLod),
		mMaxLoad(createInfo.MaxLod)
	{
		VkSamplerCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.magFilter = ToVkType(mMagFilter);
			vkCreateInfo.minFilter = ToVkType(mMinFilter);
			vkCreateInfo.mipmapMode = ToVkType(mMipmapMode);
			vkCreateInfo.addressModeU = ToVkType(mAddressModeU);
			vkCreateInfo.addressModeV = ToVkType(mAddressModeV);
			vkCreateInfo.addressModeW = ToVkType(mAddressModeW);
			vkCreateInfo.mipLodBias = 0.f;
			vkCreateInfo.anisotropyEnable = mAnisotropyEnable;
			vkCreateInfo.maxAnisotropy = mAnisotropy;
			vkCreateInfo.compareEnable = VK_FALSE;
			vkCreateInfo.compareOp = VK_COMPARE_OP_LESS;
			vkCreateInfo.minLod = mMinLoad;
			vkCreateInfo.maxLod = mMaxLoad;
			vkCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
			vkCreateInfo.unnormalizedCoordinates = VK_FALSE;
		}

		COCKTAIL_VK_CHECK(vkCreateSampler(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		Sampler::SetObjectName(createInfo.Name);
	}

	Sampler::~Sampler()
	{
		vkDestroySampler(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void Sampler::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_SAMPLER;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(GetHandle());
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Ref<Renderer::RenderDevice> Sampler::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Renderer::Filter Sampler::GetMagFilter() const
	{
		return mMagFilter;
	}

	Renderer::Filter Sampler::GetMinFilter() const
	{
		return mMinFilter;
	}

	Renderer::SamplerMipmapMode Sampler::GetMipmapMode() const
	{
		return mMipmapMode;
	}

	bool Sampler::IsAnisotropyEnabled() const
	{
		return mAnisotropyEnable;
	}

	float Sampler::GetAnisotropy() const
	{
		return mAnisotropy;
	}

	float Sampler::GetMinLod() const
	{
		return mMinLoad;
	}

	float Sampler::GetMaxLod() const
	{
		return mMaxLoad;
	}

	VkSampler Sampler::GetHandle() const
	{
		return mHandle;
	}
}
