#include <Cocktail/Renderer/Format.hpp>

namespace Ck::Renderer
{
	DisplayColorDepth FormatToColorDepth(const PixelFormat& pixelFormat)
	{
		switch (pixelFormat.GetBlockSize())
		{
		case 1:
			return DisplayColorDepth::e8;

		case 2:
			return DisplayColorDepth::e16;

		case 4:
			return DisplayColorDepth::e32;

		case 6:
			return DisplayColorDepth::e48;

		case 8:
			return DisplayColorDepth::e64;

		case 12:
			return DisplayColorDepth::e96;

		case 16:
			return DisplayColorDepth::e128;

		case 24:
			return DisplayColorDepth::e192;

		case 32:
			return DisplayColorDepth::e256;
		}

		return DisplayColorDepth::e0;
	}

	DisplayAlphaDepth FormatToAlphaDepth(const PixelFormat& pixelFormat)
	{
		switch (pixelFormat.GetLayout())
		{
			case PixelFormat::Layout::RGBA:
			case PixelFormat::Layout::BGRA:
			{
				const PixelFormat::Channel* alphaChannel = pixelFormat.GetChannel(3);
				switch (alphaChannel->Length)
				{
				case 2:
					return DisplayAlphaDepth::e2;

				case 8:
					return DisplayAlphaDepth::e8;

				case 16:
					return DisplayAlphaDepth::e16;

				case 32:
					return DisplayAlphaDepth::e32;

				case 64:
					return DisplayAlphaDepth::e64;
				}
			}

			case PixelFormat::Layout::ARGB:
			case PixelFormat::Layout::ABGR:
			{
				const PixelFormat::Channel* alphaChannel = pixelFormat.GetChannel(0);
				switch (alphaChannel->Length)
				{
				case 2:
					return DisplayAlphaDepth::e2;

				case 8:
					return DisplayAlphaDepth::e8;

				case 16:
					return DisplayAlphaDepth::e16;

				case 32:
					return DisplayAlphaDepth::e32;

				case 64:
					return DisplayAlphaDepth::e64;
				}
			}
		}

		return DisplayAlphaDepth::e0;
	}

	ColorSpace FormatToColorSpace(const PixelFormat& pixelFormat)
	{
		if (pixelFormat.GetEncoding() == PixelFormat::Encoding::Srgb)
			return ColorSpace::Srgb;
		
		return ColorSpace::Linear;
	}
}
