#include <Cocktail/Renderer/Format.hpp>

namespace Ck::Renderer
{
	DisplayColorDepth FormatToColorDepth(const PixelFormat& pixelFormat)
	{
		assert(!pixelFormat.IsCompressed());

		switch (pixelFormat.GetBlockSize())
		{
		case 3:	return DisplayColorDepth::e24;
		case 4:	return DisplayColorDepth::e32;
		}

		COCKTAIL_UNREACHABLE();
	}

	DisplayAlphaDepth FormatToAlphaDepth(const PixelFormat& pixelFormat)
	{
		assert(!pixelFormat.IsCompressed());

		switch (pixelFormat.GetLayout())
		{
			case PixelFormat::Layout::RGBA:
			case PixelFormat::Layout::BGRA:
			{
				const PixelFormat::Channel* alphaChannel = pixelFormat.GetChannel(3);
				switch (alphaChannel->Length)
				{
				case 8:	return DisplayAlphaDepth::e8;
				}
			}

			case PixelFormat::Layout::ARGB:
			case PixelFormat::Layout::ABGR:
			{
				const PixelFormat::Channel* alphaChannel = pixelFormat.GetChannel(0);
				switch (alphaChannel->Length)
				{
				case 8:	return DisplayAlphaDepth::e8;
				}
			}
		}

		return DisplayAlphaDepth::e0;
	}

	ColorSpace FormatToColorSpace(const PixelFormat& pixelFormat)
	{
		assert(!pixelFormat.IsCompressed());
		
		if (pixelFormat.GetEncoding() == PixelFormat::Encoding::Srgb)
			return ColorSpace::Srgb;
		
		return ColorSpace::Linear;
	}
}
