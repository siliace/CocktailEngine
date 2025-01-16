#include <Cocktail/Vulkan/Texture/SwapchainTexture.hpp>

namespace Ck::Vulkan
{
	SwapchainTexture::SwapchainTexture(const Ref<RenderDevice>& renderDevice, PixelFormat format, const Extent2D<unsigned int>& size, VkImage handle):
		Extends<SwapchainTexture, AbstractTexture>(renderDevice),
		mFormat(format),
		mSize(size),
		mHandle(handle)
	{
		/// Nothing
	}
	
	void SwapchainTexture::Recycle(PixelFormat format, const Extent2D<unsigned>& size, VkImage handle)
	{
		mFormat = format;
		mSize = size;
		mHandle = handle;
	}

	bool SwapchainTexture::IsExclusive() const
	{
		return false;
	}

	Renderer::MemoryType SwapchainTexture::GetMemoryType() const
	{
		return Renderer::MemoryType::Static;
	}

	Renderer::TextureType SwapchainTexture::GetType() const
	{
		return Renderer::TextureType::e2D;
	}

	PixelFormat SwapchainTexture::GetFormat() const
	{
		return mFormat;
	}

	Extent3D<unsigned int> SwapchainTexture::GetSize() const
	{
		return MakeExtent(mSize, 1u);
	}

	unsigned int SwapchainTexture::GetMipMapCount() const
	{
		return 1;
	}

	unsigned int SwapchainTexture::GetArrayLayerCount() const
	{
		return 1;
	}

	VkImage SwapchainTexture::GetHandle() const
	{
		return mHandle;
	}
}
