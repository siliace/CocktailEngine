#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>

#include <Cocktail/Graphic/Scene/Gltf/GltfImporter.hpp>
#include <Cocktail/Graphic/Scene/Gltf/GltfSceneContainer.hpp>

namespace Ck
{    
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
            outImage.bits = (int)channel->Length;

            Extent2D<unsigned int> size = inImage.GetSize();
            outImage.width = static_cast<int>(size.Width);
            outImage.height = static_cast<int>(size.Height);
            
            const unsigned char* inPixels = (const unsigned char*)inImage.GetPixels();
            std::size_t allocationSize = format.ComputeAllocationSize(size);
            outImage.image.resize(allocationSize, 0);
            std::memcpy(outImage.image.data(), inPixels, allocationSize);
        }
    }

    GltfImporter::GltfImporter(const Ref<ImageLoader>& imageLoader) :
        mImageLoader(imageLoader)
    {
        mLoader.SetImageLoader(GltfImporter::LoadImageData, this);
    }

    Ref<SceneContainer> GltfImporter::LoadFromPath(const std::filesystem::path& path, const SceneImportParameters& parameters)
    {
        tinygltf::Model model;
        std::string errors, warnings;
        
        bool success = mLoader.LoadASCIIFromFile(&model, &errors, &warnings, path.string());
        if (!success)
            return nullptr;

        return GltfSceneContainer::New(model);
    }

    Ref<SceneContainer> GltfImporter::LoadFromStream(const Ref<InputStream>& inputStream, const SceneImportParameters& parameters)
    {
        tinygltf::Model model;
        std::string errors, warnings;

        Ref<BufferedReader> reader = BufferedReader::New(
            InputStreamReader::New(inputStream)
        );
        
        std::string buffer = reader->ReadAll();
        bool success = mLoader.LoadASCIIFromString(&model, &errors, &warnings, buffer.c_str(), buffer.length(), parameters.BaseDirectory.string());
        if (!success)
            return nullptr;

        return GltfSceneContainer::New(model);
    }
    
    bool GltfImporter::SupportExtension(std::string_view extension) const
    {
        return extension == ".gltf";
    }
    
    bool GltfImporter::SupportParameters(const SceneImportParameters& parameters) const
    {
        return parameters.FlipUp == false; // Not supposed to be needed
    }

    bool GltfImporter::LoadImageData(tinygltf::Image* gltfImage, const int imageIndex, std::string* errors, std::string* warnings, int requestWidth, int requestHeight, const unsigned char* data, int size, void* userData)
    {
        GltfImporter* self = static_cast<GltfImporter*>(userData);

        Ref<Image> image;
        try 
        {
            image = App::Resolve<ImageLoader>()->LoadFromMemory(data, size, { ImageImportParameters::Format::RedGreenBlueAlpha });
        }
        catch (const std::exception& exception)
        {
            if (errors)
                errors->append(exception.what());

            return false;
        }

        ToGltfImage(*image, *gltfImage);

        return true;
    }
}