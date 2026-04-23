#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Graphic/Material/MipMaps/CubeMipMaps.hpp>
#include <Cocktail/Graphic/Material/MipMaps/DDS/DDSHeader.hpp>
#include <Cocktail/Graphic/Material/MipMaps/DDS/DDSHeaderDxt10.hpp>
#include <Cocktail/Graphic/Material/MipMaps/DDS/DDSImporter.hpp>
#include <Cocktail/Graphic/Material/MipMaps/MipMapsLoader.hpp>

namespace Ck
{
    COCKTAIL_DECLARE_EXCEPTION_FROM(DDSImportException, RuntimeException);

    namespace
    {
        PixelFormat ConvertDxgiFormat(DXGI_FORMAT format)
        {
            switch (format)
            {
                case DXGI_FORMAT_R32G32B32A32_FLOAT: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Float32, PixelFormat::Encoding::Raw); ;
                case DXGI_FORMAT_R32G32B32A32_UINT: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt32, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R32G32B32A32_SINT: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Int32, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R32G32B32_FLOAT: return PixelFormat::Color(PixelFormat::Layout::RGB, DataType::Float32, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R32G32B32_UINT: return PixelFormat::Color(PixelFormat::Layout::RGB, DataType::UnsignedInt32, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R32G32B32_SINT: return PixelFormat::Color(PixelFormat::Layout::RGB, DataType::Int32, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R16G16B16A16_FLOAT: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Float16, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R16G16B16A16_UNORM: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt16, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R16G16B16A16_UINT: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt16, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R16G16B16A16_SNORM: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Int16, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R16G16B16A16_SINT: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Int16, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R32G32_FLOAT: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::Float32, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R32G32_UINT: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::UnsignedInt32, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R32G32_SINT: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::Int32, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: return PixelFormat::DepthStencil(32, 8);
                case DXGI_FORMAT_R8G8B8A8_UNORM: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8, PixelFormat::Encoding::Srgb);
                case DXGI_FORMAT_R8G8B8A8_UINT: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R8G8B8A8_SNORM: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Int8, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R8G8B8A8_SINT: return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::Int8, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R16G16_FLOAT: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::Float16, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R16G16_UNORM: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::UnsignedInt16, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R16G16_UINT: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::UnsignedInt16, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R16G16_SNORM: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::Int16, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R16G16_SINT: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::Int16, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_D32_FLOAT: return PixelFormat::DepthStencil(32, 0);
                case DXGI_FORMAT_R32_FLOAT: return PixelFormat::Color(PixelFormat::Layout::R, DataType::Float32, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R32_UINT: return PixelFormat::Color(PixelFormat::Layout::R, DataType::UnsignedInt32, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R32_SINT: return PixelFormat::Color(PixelFormat::Layout::R, DataType::Int32, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_D24_UNORM_S8_UINT: return PixelFormat::DepthStencil(24, 8);
                case DXGI_FORMAT_R8G8_UNORM: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::UnsignedInt8, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R8G8_UINT: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::UnsignedInt8, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R8G8_SNORM: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::Int8, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R8G8_SINT: return PixelFormat::Color(PixelFormat::Layout::RG, DataType::Int8, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R16_FLOAT: return PixelFormat::Color(PixelFormat::Layout::R, DataType::Float16, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_D16_UNORM: return PixelFormat::DepthStencil(16, 0);
                case DXGI_FORMAT_R16_UNORM: return PixelFormat::Color(PixelFormat::Layout::R, DataType::UnsignedInt16, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R16_UINT: return PixelFormat::Color(PixelFormat::Layout::R, DataType::UnsignedInt16, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R16_SNORM: return PixelFormat::Color(PixelFormat::Layout::R, DataType::Int16, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R16_SINT: return PixelFormat::Color(PixelFormat::Layout::R, DataType::Int16, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R8_UNORM: return PixelFormat::Color(PixelFormat::Layout::R, DataType::UnsignedInt8, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R8_UINT: return PixelFormat::Color(PixelFormat::Layout::R, DataType::UnsignedInt8, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_R8_SNORM: return PixelFormat::Color(PixelFormat::Layout::R, DataType::Int8, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_R8_SINT: return PixelFormat::Color(PixelFormat::Layout::R, DataType::Int8, PixelFormat::Encoding::Raw);
                case DXGI_FORMAT_BC1_UNORM: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt1, false);
                case DXGI_FORMAT_BC1_UNORM_SRGB: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt1, true);
                case DXGI_FORMAT_BC2_UNORM: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt3, false);
                case DXGI_FORMAT_BC2_UNORM_SRGB: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt3, true);
                case DXGI_FORMAT_BC3_UNORM: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt5, false);
                case DXGI_FORMAT_BC3_UNORM_SRGB: return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt5, true);
                case DXGI_FORMAT_B8G8R8A8_UNORM: return PixelFormat::Color(PixelFormat::Layout::BGRA, DataType::UnsignedInt8, PixelFormat::Encoding::Normalized);
                case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: return PixelFormat::Color(PixelFormat::Layout::BGRA, DataType::UnsignedInt8, PixelFormat::Encoding::Srgb);
                default: return PixelFormat::Undefined();
            }

            return PixelFormat::Undefined();
        }

        Extent3D<unsigned int> GetTextureSize(const DDSHeader& header, const Optional<DDSHeaderDxt10>& dxt10Header)
        {
            return dxt10Header
                .Map([&](const DDSHeaderDxt10& dxt10Header) {
                    bool hasDepth = false;
                    switch (dxt10Header.ResourceDimension)
                    {
                        case D3D10_RESOURCE_DIMENSION_TEXTURE1D:
                        {
                            if (header.Height != 1)
                                throw DDSImportException(CK_TEXT("Invalid height (%d) of texture with dimension %s"), header.Height, dxt10Header.ResourceDimension);

                            if (header.Depth != 1)
                                throw DDSImportException(CK_TEXT("Invalid depth (%d) of texture with dimension %s"), header.Depth, dxt10Header.ResourceDimension);
                        }
                        break;

                        case D3D10_RESOURCE_DIMENSION_TEXTURE2D:
                        {
                            if (header.Depth != 1)
                                throw DDSImportException(CK_TEXT("Invalid depth (%d) of texture with dimension %s"), header.Depth, dxt10Header.ResourceDimension);
                        }
                        break;

                        case D3D10_RESOURCE_DIMENSION_TEXTURE3D:
                        {
                            hasDepth = true;
                            if (header.Depth == 0)
                                throw DDSImportException(CK_TEXT("Invalid depth (%d) of texture with dimension %s"), header.Depth, dxt10Header.ResourceDimension);
                        }
                        break;
                    }

                    return MakeExtent(header.Width, header.Height, hasDepth ? std::max(1u, header.Depth) : 1u);
                })
                .GetOrElse([&]() {
                    const bool hasDepth = header.FileFlags & DDSHeader::FlagBits::Depth;
                    return MakeExtent(header.Width, header.Height, hasDepth ? std::max(1u, header.Depth) : 1u);
                });
        }

        PixelFormat GetTextureFormat(const DDSPixelFormat& pixelFormat, const Optional<DDSHeaderDxt10>& dxt10Header)
        {
            const bool hasAlpha = pixelFormat.Flags & DDSPixelFormat::FlagBits::AlphaPixels || pixelFormat.Flags & DDSPixelFormat::FlagBits::Alpha;

            if (!dxt10Header.IsEmpty())
            {
                PixelFormat dxgiPixelFormat = ConvertDxgiFormat(dxt10Header.Get().DxgiFormat);
                if (dxgiPixelFormat == PixelFormat::Undefined())
                    throw DDSImportException(CK_TEXT("Incompatible DXGI format %d"), dxt10Header.Get().DxgiFormat);

                return dxgiPixelFormat;
            }

            if (pixelFormat.Flags & DDSPixelFormat::FlagBits::FourCC)
            {
                if (pixelFormat.FourCC == MakeFourCC('D', 'X', 'T', '1'))
                    return PixelFormat::Compressed(hasAlpha ? PixelFormat::CompressionScheme::AlphaDxt1 : PixelFormat::CompressionScheme::Dxt1, false);

                if (pixelFormat.FourCC == MakeFourCC('D', 'X', 'T', '3'))
                    return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt3, false);

                if (pixelFormat.FourCC == MakeFourCC('D', 'X', 'T', '5'))
                    return PixelFormat::Compressed(PixelFormat::CompressionScheme::Dxt5, false);

                throw DDSImportException(CK_TEXT("Unknown texture compression format (FourCC %d)"), pixelFormat.FourCC);
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
                        throw DDSImportException(CK_TEXT("Texture format has 32 bits but does not support alpha"));

                    if (pixelFormat.RedBitMask == 0x00FF0000 && pixelFormat.GreenBitMask == 0x0000FF00 && pixelFormat.BlueBitMask == 0x000000FF &&
                        pixelFormat.AlphaBitMask == 0xFF000000)
                        return PixelFormat::Color(PixelFormat::Layout::BGRA, DataType::UnsignedInt8);

                    if (pixelFormat.RedBitMask == 0x000000FF && pixelFormat.GreenBitMask == 0x0000FF00 && pixelFormat.BlueBitMask == 0x00FF0000 &&
                        pixelFormat.AlphaBitMask == 0xFF000000)
                        return PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8);
                }
                break;
            }

            throw DDSImportException(CK_TEXT("Unknown texture format"));
        }

        unsigned int GetTextureLayerCount(const Optional<DDSHeaderDxt10>& dxt10Header)
        {
            return dxt10Header
                .Map([](const DDSHeaderDxt10& dxt10Header) {
                    return dxt10Header.ArraySize;
                })
                .GetOr(1);
        }

        unsigned int GetTextureLevelCount(const DDSHeader& header)
        {
            return header.FileFlags & DDSHeader::FlagBits::MipMapCount ? header.MipMapCount : 1;
        }

        bool IsCubeMap(const DDSHeader& header, const Optional<DDSHeaderDxt10>& dxt10Header)
        {
            return dxt10Header
                .Map([](const DDSHeaderDxt10& dxt10Header) -> bool {
                    return dxt10Header.MiscFlag & DDSHeaderDxt10::MiscFlagBits::TextureCube;
                })
                .GetOrElse([&]() -> bool {
                    return header.Caps2 & DDSHeader::Cap2Bits::Cubemap;
                });
        }
    }

    std::shared_ptr<MipMaps> DDSImporter::LoadFromPath(const Path& path, const MipMapsImportParameters& parameters)
    {
        CK_LOG(MipMapsLoaderLogCategory, LogLevel::Info, CK_TEXT("Loading mipmaps from %s"), path.ToString());
        return AssetImporter<MipMaps, MipMapsImportParameters>::LoadFromPath(path, parameters);
    }

    std::shared_ptr<MipMaps> DDSImporter::LoadFromStream(InputStream<>& inputStream, const MipMapsImportParameters& parameters)
    {
        static Flags RequiredFileFlags = DDSHeader::FlagBits::Caps | DDSHeader::FlagBits::Height | DDSHeader::FlagBits::Width | DDSHeader::FlagBits::PixelFormat;

        unsigned int magicKey;
        std::size_t magicKeySize = inputStream.Read(reinterpret_cast<Byte*>(&magicKey), sizeof(unsigned int));
        if (magicKeySize != sizeof(unsigned int) || magicKey != DdsMagic)
            throw DDSImportException(CK_TEXT("Invalid DDS MagicKey"));

        DDSHeader header;
        std::size_t headerSize = inputStream.Read(reinterpret_cast<Byte*>(&header), sizeof(DDSHeader));
        if (headerSize != sizeof(DDSHeader) || header.Size != sizeof(DDSHeader) || !(header.FileFlags & RequiredFileFlags))
            throw DDSImportException(CK_TEXT("Invalid DDS Header"));

        if (header.PixelFormat.Size != sizeof(DDSPixelFormat) || header.PixelFormat.Flags & DDSPixelFormat::FlagBits::YUV ||
            header.PixelFormat.Flags & DDSPixelFormat::FlagBits::Luminance)
            throw DDSImportException(CK_TEXT("Invalid DDS PixelFormat"));

        Optional<DDSHeaderDxt10> dxt10Header;
        if (header.PixelFormat.Flags & DDSPixelFormat::FlagBits::FourCC && header.PixelFormat.FourCC == MakeFourCC('D', 'X', '1', '0'))
        {
            DDSHeaderDxt10 ddsDxt10Header;
            std::size_t dxt10HeaderSize = inputStream.Read(reinterpret_cast<Byte*>(&ddsDxt10Header), sizeof(DDSHeaderDxt10));
            if (dxt10HeaderSize != sizeof(DDSHeaderDxt10))
                throw DDSImportException(CK_TEXT("Invalid DDS Dxt10Header"));

            dxt10Header = Optional<DDSHeaderDxt10>::Of(ddsDxt10Header);
        }

        Extent3D<unsigned int> baseSize = GetTextureSize(header, dxt10Header);
        PixelFormat pixelFormat = GetTextureFormat(header.PixelFormat, dxt10Header);
        unsigned int arrayLayerCount = GetTextureLayerCount(dxt10Header);
        unsigned int mipMapCount = parameters.OneLevel ? 1 : GetTextureLevelCount(header);

        ByteArray pixels(pixelFormat.ComputeAllocationSize(baseSize));
        if (IsCubeMap(header, dxt10Header))
        {
            static Flags<DDSHeader::Cap2Bits> RequiredCubeFlags = DDSHeader::Cap2Bits::CubemapPositiveX | DDSHeader::Cap2Bits::CubemapNegativeX |
                                                                  DDSHeader::Cap2Bits::CubemapPositiveY | DDSHeader::Cap2Bits::CubemapNegativeY |
                                                                  DDSHeader::Cap2Bits::CubemapPositiveZ | DDSHeader::Cap2Bits::CubemapNegativeZ;

            if (!(header.Caps2 & RequiredCubeFlags))
                throw DDSImportException(CK_TEXT("Missing face in DDS cubemap"));

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
                            throw DDSImportException(CK_TEXT("Invalid pixel memory length at slice %d, level %d"), slice, level);

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
                    throw DDSImportException(CK_TEXT("Invalid pixel memory length at slice %d, level %d"), slice, level);

                mipMapLevel.CopyPixels(pixels.GetData());
            }
        }

        return mipMaps;
    }

    bool DDSImporter::SupportExtension(StringView extension) const
    {
        return extension == CK_TEXT(".dds");
    }

    bool DDSImporter::SupportParameters(const MipMapsImportParameters& parameters) const
    {
        return true;
    }
}
