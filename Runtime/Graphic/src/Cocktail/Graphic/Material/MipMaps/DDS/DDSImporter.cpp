#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Graphic/Material/MipMaps/CubeMipMaps.hpp>
#include <Cocktail/Graphic/Material/MipMaps/MipMapsLoader.hpp>
#include <Cocktail/Graphic/Material/MipMaps/DDS/DDSHeader.hpp>
#include <Cocktail/Graphic/Material/MipMaps/DDS/DDSHeaderDxt10.hpp>
#include <Cocktail/Graphic/Material/MipMaps/DDS/DDSImporter.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_EXCEPTION_FROM(DDSImportException, RuntimeException);

	namespace
	{
		Extent3D<unsigned int> GetTextureSize(const DDSHeader& header)
		{
			const bool hasDepth = header.FileFlags & DDSHeader::FlagBits::Depth;
			return MakeExtent(header.Width, header.Height, hasDepth ? std::max(1u, header.Depth) : 1u);
		}

		PixelFormat GetTextureFormat(const DDSPixelFormat& pixelFormat, Optional<DDSHeaderDxt10> dxt10Header)
		{
			const bool hasAlpha = pixelFormat.Flags & DDSPixelFormat::FlagBits::AlphaPixels || pixelFormat.Flags & DDSPixelFormat::FlagBits::Alpha;

			if (pixelFormat.Flags & DDSPixelFormat::FlagBits::FourCC)
			{
				if (dxt10Header.IsEmpty())
				{
					if (pixelFormat.FourCC == MakeFourCC('D', 'X', 'T', '1'))
						return PixelFormat::Compressed(hasAlpha ? PixelFormat::CompressionScheme::AlphaDxt1 : PixelFormat::CompressionScheme::Dxt1, false);

					if (pixelFormat.FourCC == MakeFourCC('D', 'X', 'T', '3'))
						return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt3, false);

					if (pixelFormat.FourCC == MakeFourCC('D', 'X', 'T', '5'))
						return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt5, false);

					throw DDSImportException("Unknown texture compression format (FourCC {})", pixelFormat.FourCC);
				}

				throw DDSImportException("Unknown texture compression format (FourCC {})", pixelFormat.FourCC);
			}

			switch (pixelFormat.RGBBitCount)
			{
			case 24:
				{
					if (pixelFormat.RedBitMask == 0x00FF0000 && pixelFormat.GreenBitMask == 0x0000FF00 && pixelFormat.BlueBitMask == 0x000000FF)
						return PixelFormat::Color(PixelFormat::Layout::BGR, DataType::UnsignedInt8);

					if (pixelFormat.RedBitMask == 0x000000FF && pixelFormat.GreenBitMask == 0x0000FF00 && pixelFormat.BlueBitMask == 0x00FF0000)
						return PixelFormat::Color(PixelFormat::Layout::RGB, DataType::UnsignedInt8);
				}
				break;

			case 32:
				{
					if (!hasAlpha)
						throw DDSImportException("Texture format has 32 bits but does not support alpha");

					if (pixelFormat.RedBitMask == 0x00FF0000 && pixelFormat.GreenBitMask == 0x0000FF00 && pixelFormat.BlueBitMask == 0x000000FF && pixelFormat.AlphaBitMask == 0xFF000000)
						return PixelFormat::Color(PixelFormat::Layout::BGRA, DataType::UnsignedInt8);

					if (pixelFormat.RedBitMask == 0x000000FF && pixelFormat.GreenBitMask == 0x0000FF00 && pixelFormat.BlueBitMask == 0x00FF0000 && pixelFormat.AlphaBitMask == 0xFF000000)
						return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8);
				}
				break;
			}

			throw DDSImportException("Unknown texture format");
		}

		unsigned int GetTextureLayerCount(Optional<DDSHeaderDxt10> dxt10Header)
		{
			return dxt10Header.Map([](const DDSHeaderDxt10& dxt10Header) {
				return dxt10Header.ArraySize;
			}).GetOr(1);
		}

		unsigned int GetTextureLevelCount(const DDSHeader& header)
		{
			return header.FileFlags & DDSHeader::FlagBits::MipMapCount ? header.MipMapCount : 1;
		}
	}

	std::shared_ptr<MipMaps> DDSImporter::LoadFromPath(const std::filesystem::path& path, const MipMapsImportParameters& parameters)
	{
		CK_LOG(MipMapsLoaderLogCategory, LogLevel::Info, "Loading mipmaps from {}", path.string());
		return AssetImporter<MipMaps, MipMapsImportParameters>::LoadFromPath(path, parameters);
	}

	std::shared_ptr<MipMaps> DDSImporter::LoadFromStream(InputStream& inputStream, const MipMapsImportParameters& parameters)
	{
		static Flags RequiredFileFlags = DDSHeader::FlagBits::Caps | DDSHeader::FlagBits::Height |
			DDSHeader::FlagBits::Width | DDSHeader::FlagBits::PixelFormat;

		unsigned int magicKey;
		std::size_t magicKeySize = inputStream.Read(&magicKey, sizeof(unsigned int));
		if (magicKeySize != sizeof(unsigned int) || magicKey != DdsMagic)
			throw DDSImportException("Invalid DDS MagicKey");

		DDSHeader header;
		std::size_t headerSize = inputStream.Read(&header, sizeof(DDSHeader));
		if (headerSize != sizeof(DDSHeader) || header.Size != sizeof(DDSHeader) || !(header.FileFlags & RequiredFileFlags))
			throw DDSImportException("Invalid DDS Header");

		if (header.PixelFormat.Size != sizeof(DDSPixelFormat) || header.PixelFormat.Flags & DDSPixelFormat::FlagBits::YUV || header.PixelFormat.Flags & DDSPixelFormat::FlagBits::Luminance)
			throw DDSImportException("Invalid DDS PixelFormat");

		Optional<DDSHeaderDxt10> dxt10Header;
		if (header.PixelFormat.Flags & DDSPixelFormat::FlagBits::FourCC && header.PixelFormat.FourCC == MakeFourCC('D', 'X', '1', '0'))
		{
			DDSHeaderDxt10 ddsDxt10Header;
			std::size_t dxt10HeaderSize = inputStream.Read(&ddsDxt10Header, sizeof(DDSHeaderDxt10));
			if (dxt10HeaderSize != sizeof(DDSHeaderDxt10))
				throw DDSImportException("Invalid DDS Dxt10Header");

			dxt10Header = Optional<DDSHeaderDxt10>::Of(ddsDxt10Header);
		}

		Extent3D<unsigned int> baseSize = GetTextureSize(header);
		PixelFormat pixelFormat = GetTextureFormat(header.PixelFormat, dxt10Header);
		unsigned int arrayLayerCount = GetTextureLayerCount(dxt10Header);
		unsigned int mipMapCount = parameters.OneLevel ? 1 : GetTextureLevelCount(header);

		ByteArray pixels(pixelFormat.ComputeAllocationSize(baseSize));
		if (header.Caps2 & DDSHeader::Cap2Bits::Cubemap)
		{
			static Flags<DDSHeader::Cap2Bits> RequiredCubeFlags =
				DDSHeader::Cap2Bits::CubemapPositiveX | DDSHeader::Cap2Bits::CubemapNegativeX |
				DDSHeader::Cap2Bits::CubemapPositiveY | DDSHeader::Cap2Bits::CubemapNegativeY |
				DDSHeader::Cap2Bits::CubemapPositiveZ | DDSHeader::Cap2Bits::CubemapNegativeZ;

			if (!(header.Caps2 & RequiredCubeFlags))
				throw DDSImportException("Missing face in DDS cubemap");

			std::shared_ptr<CubeMipMaps> mipMaps = std::make_shared<CubeMipMaps>(MakeExtent(baseSize.Width, baseSize.Height), pixelFormat, arrayLayerCount, mipMapCount);
			for (unsigned int slice = 0; slice < arrayLayerCount; slice++)
			{
				for (CubeMipMaps::Face face : Enum<CubeMipMaps::Face>::Values)
				{
					for (unsigned level = 0; level < mipMapCount; level++)
					{
						MipMapLevel& mipMapLevel = mipMaps->GetFaceLevel(face, slice, level);
						std::size_t levelAllocationSize = pixelFormat.ComputeAllocationSize(mipMapLevel.GetSize());

						std::size_t levelPixelSize = inputStream.Read(pixels.GetData(), levelAllocationSize);
						if (levelPixelSize != levelAllocationSize)
							throw DDSImportException("Invalid pixel memory length at slice {}, level {}", slice, level);

						mipMapLevel.CopyPixels(pixels.GetData());
					}
				}
			}

			return mipMaps;
		}

		std::shared_ptr<MipMaps> mipMaps = std::make_shared<MipMaps>(baseSize, pixelFormat, arrayLayerCount, mipMapCount);
		for (unsigned int slice = 0; slice < arrayLayerCount; slice++)
		{
			for (unsigned level = 0; level < mipMapCount; level++)
			{
				MipMapLevel& mipMapLevel = mipMaps->GetLevel(slice, level);
				std::size_t levelAllocationSize = pixelFormat.ComputeAllocationSize(mipMapLevel.GetSize());

				std::size_t levelPixelSize = inputStream.Read(pixels.GetData(), levelAllocationSize);
				if (levelPixelSize != levelAllocationSize)
					throw DDSImportException("Invalid pixel memory length at slice {}, level {}", slice, level);

				mipMapLevel.CopyPixels(pixels.GetData());
			}
		}

		return mipMaps;
	}

	bool DDSImporter::SupportExtension(std::string_view extension) const
	{
		return extension == ".dds";
	}

	bool DDSImporter::SupportParameters(const MipMapsImportParameters& parameters) const
	{
		return true;
	}
}
