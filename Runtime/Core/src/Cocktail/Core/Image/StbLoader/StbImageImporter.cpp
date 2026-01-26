#include <stb_image/stb_image.h>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Image/ImageLoader.hpp>
#include <Cocktail/Core/Image/StbLoader/StbImageImporter.hpp>
#include <Cocktail/Core/Log/Log.hpp>

namespace Ck
{
    COCKTAIL_DECLARE_EXCEPTION_FROM(StbImportError, RuntimeException);

    namespace
    {
        int ChooseDesiredChannels(ImageImportParameters::ImportFormat importFormat)
        {
            switch (importFormat)
            {
                case ImageImportParameters::ImportFormat::Default: return STBI_default;
                case ImageImportParameters::ImportFormat::Grey: return STBI_grey;
                case ImageImportParameters::ImportFormat::GreyAlpha: return STBI_grey_alpha;
                case ImageImportParameters::ImportFormat::RGB: return STBI_rgb;
                case ImageImportParameters::ImportFormat::RGBA: return STBI_rgb_alpha;
            }

            COCKTAIL_UNREACHABLE();
        }

        ImageRawFormat::Type ChooseImageRawFormat(int channelCount, bool is16bits, bool isHdr)
        {
            switch (channelCount)
            {
                case 1:
                {
                    if (isHdr)
                        return ImageRawFormat::Type::Grey32F;

                    return is16bits ? ImageRawFormat::Type::Grey16 : ImageRawFormat::Type::Grey8;
                }

                case 2:
                {
                    if (isHdr)
                        return ImageRawFormat::Type::GreyAlpha32F;

                    return is16bits ? ImageRawFormat::Type::GreyAlpha16 : ImageRawFormat::Type::GreyAlpha8;
                }

                case 3:
                {
                    if (isHdr)
                        return ImageRawFormat::Type::RedGreenBlue32F;

                    return is16bits ? ImageRawFormat::Type::RedGreenBlue16 : ImageRawFormat::Type::RedGreenBlue8;
                }

                case 4:
                {
                    if (isHdr)
                        return ImageRawFormat::Type::RedGreenBlueAlpha32F;

                    return is16bits ? ImageRawFormat::Type::RedGreenBlueAlpha16 : ImageRawFormat::Type::RedGreenBlueAlpha8;
                }
            }

            COCKTAIL_UNREACHABLE();
        }
    }

    const Array<String, LinearAllocator<16>> StbImageImporter::SupportedExtensions = { CK_TEXT(".jpg"), CK_TEXT(".jpeg"), CK_TEXT(".png"), CK_TEXT(".tga"), CK_TEXT(".bmp"),
                                                                                       CK_TEXT(".psd"), CK_TEXT(".hdr"),  CK_TEXT(".pic"), CK_TEXT(".gif") };

    StbImageImporter::StbImageImporter() :
        mCallbacks()
    {
        mCallbacks.read = &StbImageImporter::ReadCallback;
        mCallbacks.skip = &StbImageImporter::SkipCallback;
        mCallbacks.eof = &StbImageImporter::EofCallback;
    }

    std::shared_ptr<Image> StbImageImporter::LoadFromPath(const Path& path, const ImageImportParameters& parameters)
    {
        CK_LOG(ImageLoaderLogCategory, LogLevel::Info, CK_TEXT("Loading image from %s"), path.ToString());
        return AssetImporter<Image, ImageImportParameters>::LoadFromPath(path, parameters);
    }

    std::shared_ptr<Image> StbImageImporter::LoadFromStream(InputStream<>& inputStream, const ImageImportParameters& parameters)
    {
        void* pixels;
        int width, height, channels;
        int desiredChannels = ChooseDesiredChannels(parameters.Format);

        bool is16bits = false;
        bool isHdr = stbi_is_hdr_from_callbacks(&mCallbacks, &inputStream) == 1;
        inputStream.Rewind();

        if (!isHdr)
        {
            is16bits = stbi_is_16_bit_from_callbacks(&mCallbacks, &inputStream) == 1;
            inputStream.Rewind();

            if (is16bits)
            {
                pixels = stbi_load_16_from_callbacks(&mCallbacks, &inputStream, &width, &height, &channels, desiredChannels);
            }
            else
            {
                pixels = stbi_load_from_callbacks(&mCallbacks, &inputStream, &width, &height, &channels, desiredChannels);
            }
        }
        else
        {
            pixels = stbi_loadf_from_callbacks(&mCallbacks, &inputStream, &width, &height, &channels, desiredChannels);
        }

        if (!pixels)
            throw StbImportError(CK_TEXT("Failed to import image: %s"), stbi_failure_reason());

        Extent2D<unsigned int> size = MakeExtent<unsigned int>(width, height);
        ImageRawFormat::Type format = ChooseImageRawFormat(parameters.Format == ImageImportParameters::ImportFormat::Default ? channels : desiredChannels, is16bits, isHdr);

        LargeByteArray pixelBuffer(static_cast<const Byte*>(pixels), ImageRawFormat::ComputeAllocationSize(size, format));
        stbi_image_free(pixels);

        return std::make_shared<Image>(size, format, isHdr ? GammaSpace::Linear : GammaSpace::sRGB, std::move(pixelBuffer));
    }

    bool StbImageImporter::SupportExtension(StringView extension) const
    {
        return SupportedExtensions.ContainsIf([&](const String& supportedExtension) {
            return supportedExtension == extension;
        });
    }

    bool StbImageImporter::SupportParameters(const ImageImportParameters& parameters) const
    {
        return true;
    }

    int StbImageImporter::ReadCallback(void* user, char* data, int size)
    {
        InputStream<>* stream = static_cast<InputStream<>*>(user);
        return static_cast<int>(stream->Read(reinterpret_cast<Byte*>(data), static_cast<std::size_t>(size)));
    }

    void StbImageImporter::SkipCallback(void* user, int n)
    {
        InputStream<>* stream = static_cast<InputStream<>*>(user);

        if (n < 0 && std::abs(n) > stream->Tell())
        {
            stream->Rewind();
        }
        else
        {
            stream->Seek(stream->Tell() + n);
        }
    }

    int StbImageImporter::EofCallback(void* user)
    {
        InputStream<>* stream = static_cast<InputStream<>*>(user);
        return stream->GetSize() == stream->Tell();
    }
}
