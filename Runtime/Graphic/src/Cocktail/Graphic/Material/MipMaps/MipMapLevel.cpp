#include <Cocktail/Graphic/Material/MipMaps/MipMapLevel.hpp>

namespace Ck
{
	void MipMapLevel::CopyPixels(const void* pixels)
	{
		mComplete = true;
		std::memcpy(mPixels.GetData(), pixels, mPixelFormat.ComputeAllocationSize(mSize));
	}

	void MipMapLevel::CopyPixels(Extent3D<unsigned int> offset, Extent3D<unsigned int> size, const void* pixels)
	{
		assert(!mPixelFormat.IsCompressed()); /// TODO: handle compressed formats

		assert(offset.Width + size.Width < mSize.Width);
		assert(offset.Height + size.Height < mSize.Height);
		assert(offset.Depth + size.Depth < mSize.Depth);

		mComplete = true;
		
		std::size_t dstSliceAllocationSize = mPixelFormat.ComputeAllocationSize(MakeExtent(mSize.Width, mSize.Height));
		std::size_t srcSliceAllocationSize = mPixelFormat.ComputeAllocationSize(MakeExtent(size.Width, size.Height));

		std::size_t dstRowAllocationSize = mPixelFormat.ComputeAllocationSize(MakeExtent(mSize.Width, 1u));
		std::size_t srcRowAllocationSize = mPixelFormat.ComputeAllocationSize(MakeExtent(size.Width, 1u));

		for (unsigned int i = 0; i < size.Depth; i++)
		{
			for (unsigned int j = 0; j < size.Height; j++)
			{
				std::size_t sliceOffset = (offset.Depth + i) * dstSliceAllocationSize;
				std::size_t rawOffset = (offset.Height + j) * dstRowAllocationSize;
				std::size_t dstOffset = sliceOffset + rawOffset + mPixelFormat.GetBlockSize() * offset.Width;

				std::size_t srcOffset = i * srcSliceAllocationSize + j * srcRowAllocationSize;

				std::memcpy(mPixels.GetData() + dstOffset, (const Uint8*)pixels + srcOffset, srcRowAllocationSize);
			}
		}
	}

    bool MipMapLevel::IsComplete() const
    {
        return mComplete;
    }

    Extent3D<unsigned int> MipMapLevel::GetSize() const
    {
		return mSize;
	}

    PixelFormat MipMapLevel::GetPixelFormat() const
    {
		return mPixelFormat;
    }

	ByteArrayView MipMapLevel::GetPixels() const
	{
		return mPixels;
	}

	MipMapLevel::MipMapLevel(Extent3D<unsigned int> size, const PixelFormat& pixelFormat, std::size_t allocationSize) :
		mSize(size),
		mPixelFormat(pixelFormat),
		mPixels(allocationSize),
		mComplete(false)
	{
		/// Nothing
	}
}
