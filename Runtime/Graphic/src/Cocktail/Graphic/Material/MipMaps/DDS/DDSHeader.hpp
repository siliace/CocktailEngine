#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSHEADER_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSHEADER_HPP

#include <Cocktail/Graphic/Material/MipMaps/DDS/DDSPixelFormat.hpp>

namespace Ck
{
	constexpr Uint32 MakeFourCC(char ch0, char ch1, char ch2, char ch3)
	{
		return static_cast<Uint32>(static_cast<Uint8>(ch0)) |
			static_cast<Uint32>(static_cast<Uint8>(ch1)) << 8 |
			static_cast<Uint32>(static_cast<Uint8>(ch2)) << 16 |
			static_cast<Uint32>(static_cast<Uint8>(ch3)) << 24;
	}

	static constexpr Uint32 DdsMagic = MakeFourCC('D', 'D', 'S', ' ');

	struct DDSHeader
	{
		enum class FlagBits
		{
			Caps = 0x1,
			Height = 0x2,
			Width = 0x4,
			Pitch = 0x8,
			PixelFormat = 0x1000,
			MipMapCount = 0x20000,
			LinearSize = 0x80000,
			Depth = 0x800000,
		};

		enum class CapBits
		{
			Complex = 0x8,
			MipMap = 0x400000,
			Texture = 0x1000,
		};

		enum class Cap2Bits
		{
			Cubemap = 0x200,
			CubemapPositiveX = 0x400,
			CubemapNegativeX = 0x800,
			CubemapPositiveY = 0x1000,
			CubemapNegativeY = 0x2000,
			CubemapPositiveZ = 0x4000,
			CubemapNegativeZ = 0x8000,
			Volume = 0x200000,
		};

		enum class Cap3Bits
		{
		};

		enum class Cap4Bits
		{
		};


		Uint32 Size;
		Flags<FlagBits> FileFlags;
		Uint32 Height;
		Uint32 Width;
		Uint32 PitchOrLinearSize;
		Uint32 Depth;
		Uint32 MipMapCount;
		Uint32 Reserved1[11];
		DDSPixelFormat PixelFormat;
		Flags<CapBits> Caps;
		Flags<Cap2Bits> Caps2;
		Flags<Cap3Bits> Caps3;
		Flags<Cap4Bits> Caps4;
		Uint32 Reserved2;
	};

	static_assert(sizeof(DDSHeader) == 124, "DDSHeader structure size must be 124 bytes");
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSHEADER_HPP
