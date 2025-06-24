#include <stb_image/stb_image.h>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Image/ImageLoader.hpp>
#include <Cocktail/Core/Image/StbLoader/StbImageImporter.hpp>
#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>
#include <Cocktail/Core/Log/Log.hpp>

namespace Ck
{
	CK_DECLARE_EXCEPTION_BASE(StbImportError, "Failed to import image", std::runtime_error);

	StbImageImporter::StbImageImporter() :
		mCallbacks()
	{
		mCallbacks.read = &StbImageImporter::ReadCallback;
		mCallbacks.skip = &StbImageImporter::SkipCallback;
		mCallbacks.eof = &StbImageImporter::EofCallback;
	}

	std::shared_ptr<Image> StbImageImporter::LoadFromPath(const std::filesystem::path& path, const ImageImportParameters& parameters)
	{
		CK_LOG(ImageLoaderLogCategory, LogLevel::Info, "Loading image from {}", path.string());
		return AssetImporter<Image, ImageImportParameters>::LoadFromPath(path, parameters);
	}

	std::shared_ptr<Image> StbImageImporter::LoadFromStream(InputStream& inputStream, const ImageImportParameters& parameters)
	{
		void* pixels;
		DataType dataType;
		int width, height, fileChannels;
		int desiredChannels = STBI_default;
		PixelFormat::Encoding pixelEncoding;

		switch (parameters.ImportFormat)
		{
			case ImageImportParameters::Format::Default:
				desiredChannels = STBI_default;
				break;

			case ImageImportParameters::Format::Grey:
				desiredChannels = STBI_grey;
				break;

			case ImageImportParameters::Format::GreyAlpha:
				desiredChannels = STBI_grey_alpha;
				break;

			case ImageImportParameters::Format::RedGreenBlue:
				desiredChannels = STBI_rgb;
				break;

			case ImageImportParameters::Format::RedGreenBlueAlpha:
				desiredChannels = STBI_rgb_alpha;
				break;
		}

		bool isHdr = stbi_is_hdr_from_callbacks(&mCallbacks, &inputStream) == 1;
		inputStream.Rewind();

		if (!isHdr)
		{
			bool is16bits = stbi_is_16_bit_from_callbacks(&mCallbacks, &inputStream) == 1;
			inputStream.Rewind();

			pixelEncoding = PixelFormat::Encoding::Normalized;

			if (is16bits)
			{
				dataType = DataType::UnsignedInt16;
				pixels = stbi_load_16_from_callbacks(&mCallbacks, &inputStream, &width, &height, &fileChannels, desiredChannels);
			}
			else
			{
				dataType = DataType::UnsignedInt8;
				pixels = stbi_load_from_callbacks(&mCallbacks, &inputStream, &width, &height, &fileChannels, desiredChannels);
			}
		}
		else
		{
			dataType = DataType::Float16;
			pixelEncoding = PixelFormat::Encoding::Raw;
			pixels = stbi_load_16_from_callbacks(&mCallbacks, &inputStream, &width, &height, &fileChannels, desiredChannels);
		}

		if (!pixels)
			throw StbImportError(stbi_failure_reason());
		
		Extent2D<unsigned int> size = MakeExtent<unsigned int>(width, height);
		unsigned int channels = desiredChannels == STBI_default ? fileChannels : desiredChannels;

		PixelFormat::Layout pixelLayout;
		switch (channels)
		{
		case 1:
			pixelLayout = PixelFormat::Layout::R;
			break;
		case 2:
			pixelLayout = PixelFormat::Layout::RG;
			break;

		case 3:
			pixelLayout = PixelFormat::Layout::RGB;
			break;

		default:
			pixelLayout = PixelFormat::Layout::RGBA;
			break;
		}

		PixelFormat pixelFormat = PixelFormat::Color(pixelLayout, dataType, pixelEncoding);
		std::shared_ptr<Image> image = std::make_shared<Image>(size, pixelFormat, pixels);

		stbi_image_free(pixels);

		return image;
	}

	bool StbImageImporter::SupportExtension(std::string_view extension) const
	{
		for (const std::string_view supportedExtension : {".jpg", ".jpeg", ".png", ".tga"})
		{
			if (supportedExtension == extension)
				return true;
		}

		return false;
	}

	bool StbImageImporter::SupportParameters(const ImageImportParameters& parameters) const
	{
		return true;
	}

	int StbImageImporter::ReadCallback(void* user, char* data, int size)
	{
		InputStream* stream = static_cast<InputStream*>(user);
		return static_cast<int>(stream->Read(data, static_cast<std::size_t>(size)));
	}

	void StbImageImporter::SkipCallback(void* user, int n)
	{
		InputStream* stream = static_cast<InputStream*>(user);
		stream->Seek(stream->Tell() + n);
	}

	int StbImageImporter::EofCallback(void* user)
	{
		InputStream* stream = static_cast<InputStream*>(user);
		return stream->GetSize() == stream->Tell();
	}
}
