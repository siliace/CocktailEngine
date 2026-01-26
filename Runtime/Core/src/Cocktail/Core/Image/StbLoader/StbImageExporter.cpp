#include <stb_image_write/stb_image_write.h>

#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Image/StbLoader/StbImageExporter.hpp>
#include <Cocktail/Core/IO/Output/Stream/FileOutputStream.hpp>
#include <Cocktail/Core/IO/Output/Stream/MemoryOutputStream.hpp>
#include <Cocktail/Core/System/FileSystem/Storage.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_EXCEPTION_FROM(StbExportError, RuntimeException);

	namespace
	{
		void imageWriteFunc(void* context, void* data, int size)
		{
			static_cast<OutputStream<>*>(context)->Write(static_cast<const Byte*>(data), size);
		}
	}

	void StbImageExporter::SaveToPath(const Image& asset, const Path& path, const ImageExportParameters& parameters) const
	{
		ImageExportParameters streamExportParameters = parameters;
		streamExportParameters.Format = path.GetExtension();

		FileOutputStream outputStream(path, true);
		SaveToStream(asset, outputStream, streamExportParameters);
	}

	void StbImageExporter::SaveToStream(const Image& asset, OutputStream<>& outputStream, const ImageExportParameters& parameters) const
	{
		Extent2D<unsigned int> size = asset.GetSize();
		unsigned int channelCount = ImageRawFormat::GetChannelCount(asset.GetRawFormat());
		const void* pixels = asset.GetPixels().GetData();

		int err = 0;
		if (parameters.Format == CK_TEXT("png"))
		{
			err = stbi_write_png_to_func(imageWriteFunc, &outputStream, size.Width, size.Height, channelCount, pixels, 0);
		}
		else if (parameters.Format == CK_TEXT("bmp"))
		{
			err = stbi_write_bmp_to_func(imageWriteFunc, &outputStream, size.Width, size.Height, channelCount, pixels);
		}
		else if (parameters.Format == CK_TEXT("tga"))
		{
			err = stbi_write_tga_to_func(imageWriteFunc, &outputStream, size.Width, size.Height, channelCount, pixels);
		}
		else if (parameters.Format == CK_TEXT("jpg") || parameters.Format == CK_TEXT("jpeg"))
		{
			err = stbi_write_jpg_to_func(imageWriteFunc, &outputStream, size.Width, size.Height, channelCount, pixels, 90);
		}

		if (err == 0)
			throw StbExportError(CK_TEXT("Failed to export image"));
	}

	ByteArray StbImageExporter::SaveToMemory(const Image& asset, const ImageExportParameters& parameters) const
	{
		ByteArray byteArray;
		MemoryOutputStream outputStream(byteArray);
		SaveToStream(asset, outputStream, parameters);

		return byteArray;
	}

	bool StbImageExporter::SupportExtension(StringView extension) const
	{
		for (StringView supportedExtension : { CK_TEXT(".jpg"), CK_TEXT(".jpeg"), CK_TEXT(".png"), CK_TEXT(".tga"), CK_TEXT(".bmp") })
		{
			if (supportedExtension == extension)
				return true;
		}

		return false;
	}

	bool StbImageExporter::SupportParameters(const ImageExportParameters& parameters) const
	{
		return true;
	}
}
