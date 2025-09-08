#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

#include <Cocktail/Graphic/Scene/Container/Gltf/GltfImporter.hpp>
#include <Cocktail/Graphic/Scene/Container/Gltf/GltfSceneContainer.hpp>

namespace Ck
{
    COCKTAIL_DECLARE_EXCEPTION_FROM(GltfParseError, RuntimeException);

    namespace
    {
        void ToGltfImage(const Image& inImage, tinygltf::Image& outImage)
        {
            const PixelFormat& format = inImage.GetFormat();
            outImage.component = static_cast<int>(format.GetChannelCount());

            const PixelFormat::Channel* channel = format.GetChannel(0);
            assert(channel);
            assert(channel->Length > 0);

            if (channel->IsUnsigned)
            {
                switch (channel->Length)
                {
                case 8:
                    outImage.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
                    break;

                case 16:
                    outImage.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
                    break;

                case 32:
                    outImage.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
                    break;
                }
            }
            else
            {
                if (channel->IsFloatingPoint)
                {
                    outImage.pixel_type = channel->Length == 64 ? TINYGLTF_COMPONENT_TYPE_DOUBLE : TINYGLTF_COMPONENT_TYPE_FLOAT;
                }
                else
                {
                    switch (channel->Length)
                    {
                    case 8:
                        outImage.pixel_type = TINYGLTF_COMPONENT_TYPE_BYTE;
                        break;

                    case 16:
                        outImage.pixel_type = TINYGLTF_COMPONENT_TYPE_SHORT;
                        break;

                    case 32:
                        outImage.pixel_type = TINYGLTF_COMPONENT_TYPE_INT;
                        break;
                    }
                }
            }
            outImage.bits = static_cast<int>(channel->Length);

            Extent2D<unsigned int> size = inImage.GetSize();
            outImage.width = static_cast<int>(size.Width);
            outImage.height = static_cast<int>(size.Height);
            
            const unsigned char* inPixels = inImage.GetPixels().GetData();
            std::size_t allocationSize = format.ComputeAllocationSize(size);
            outImage.image.resize(allocationSize, 0);
            std::memcpy(outImage.image.data(), inPixels, allocationSize);
        }
    }

    GltfImporter::GltfImporter(ImageLoader* imageLoader) :
        mImageLoader(imageLoader)
    {
        mLoader.SetImageLoader(GltfImporter::LoadImageData, this);
    }

    std::shared_ptr<SceneContainer> GltfImporter::LoadFromPath(const Path& path, const SceneImportParameters& parameters)
    {
        tinygltf::Model model;
        std::string errors, warnings;

        CK_LOG(SceneLoaderLogCategory, LogLevel::Info, CK_TEXT("Loading scene from %s"), path.ToString());
        bool success = mLoader.LoadASCIIFromFile(&model, &errors, &warnings, CK_TEXT_TO_ANSI(path.ToString().GetData()));
        if (!success)
            throw GltfParseError(CK_ANSI_TO_TEXT(errors.c_str()));

        if (!warnings.empty())
            CK_LOG(SceneLoaderLogCategory, LogLevel::Error, CK_TEXT("Scene %s loaded with warnings: %s"), path.ToString(), warnings.c_str());

        return std::make_shared<GltfSceneContainer>(model);
    }

    std::shared_ptr<SceneContainer> GltfImporter::LoadFromStream(InputStream& inputStream, const SceneImportParameters& parameters)
    {
        tinygltf::Model model;
        std::string errors, warnings;

        InputStreamReader InputStreamReader(inputStream);
        BufferedReader reader(InputStreamReader);
        
        String buffer = reader.ReadAll();
        bool success = mLoader.LoadASCIIFromString(
            &model,
            &errors, &warnings,
            CK_TEXT_TO_ANSI(buffer.GetData()), buffer.GetLength(),
            CK_TEXT_TO_ANSI(parameters.BaseDirectory.ToString().GetData())
        );

        if (!success)
            return nullptr;

        return std::make_shared<GltfSceneContainer>(model);
    }
    
    bool GltfImporter::SupportExtension(StringView extension) const
    {
        return extension == CK_TEXT(".gltf");
    }
    
    bool GltfImporter::SupportParameters(const SceneImportParameters& parameters) const
    {
        return parameters.FlipUpUV == false; // Not supposed to be needed
    }

    bool GltfImporter::LoadImageData(tinygltf::Image* gltfImage, const int imageIndex, std::string* errors, std::string* warnings, int requestWidth, int requestHeight, const unsigned char* data, int size, void* userData)
    {
        GltfImporter* self = static_cast<GltfImporter*>(userData);

        std::shared_ptr<Image> image;
        try 
        {
            image = self->mImageLoader->LoadFromMemory(data, size, { ImageImportParameters::Format::RedGreenBlueAlpha });
        }
        catch (const std::exception& exception)
        {
            if (errors)
                errors->append(exception.what());
        }

        if (!image)
            return false;

        ToGltfImage(*image, *gltfImage);

        return true;
    }
}
