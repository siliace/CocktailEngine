#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Texture/AbstractTexture.hpp>

namespace Ck::Vulkan
{
	AbstractTexture::AbstractTexture(RenderDevice* renderDevice) :
		mRenderDevice(std::move(renderDevice)),
		mHasMutableFormat(false),
		mViewFormatCount(0)
	{
		/// Nothing
	}

	void AbstractTexture::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_IMAGE;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(GetHandle());
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Renderer::RenderDevice* AbstractTexture::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Renderer::ResourceType AbstractTexture::GetResourceType() const
	{
		return Renderer::ResourceType::Texture;
	}

	bool AbstractTexture::HasMutableFormat() const
	{
		return mHasMutableFormat;
	}

	Renderer::RasterizationSamples AbstractTexture::GetSamples() const
	{
		return Renderer::RasterizationSamples::e1;
	}

	void AbstractTexture::SetViewFormats(unsigned int viewFormatCount, const PixelFormat* viewFormats)
	{
		mHasMutableFormat = true;
		mViewFormatCount = viewFormatCount;
		mViewFormats = std::make_unique<PixelFormat[]>(mViewFormatCount);
		for (unsigned int i = 0; i < mViewFormatCount; i++)
			mViewFormats[i] = viewFormats[i];
	}
}
