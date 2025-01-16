#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSPIXELFORMAT_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSPIXELFORMAT_HPP

#include <Cocktail/Core/Flags.hpp>

namespace Ck
{
	struct DDSPixelFormat
	{
		enum class FlagBits
		{
			AlphaPixels = 0x1,
			Alpha = 0x2,
			FourCC = 0x4,
			RGB = 0x40,
			YUV = 0x200,
			Luminance = 0x20000,
		};

		Uint32 Size;
		Flags<FlagBits> Flags;
		Uint32 FourCC;
		Uint32 RGBBitCount;
		Uint32 RedBitMask;
		Uint32 GreenBitMask;
		Uint32 BlueBitMask;
		Uint32 AlphaBitMask;
	};

	static_assert(sizeof(DDSPixelFormat) == 32, "DDSPixelFormat structure size must be 32 bytes");
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSPIXELFORMAT_HPP
