#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Image/ImageLoader.hpp>
#include <Cocktail/Core/Image/ImageServiceProvider.hpp>
#include <Cocktail/Core/Image/StbLoader/StbImageExporter.hpp>
#include <Cocktail/Core/Image/StbLoader/StbImageImporter.hpp>

namespace Ck
{
    ImageServiceProvider::ImageServiceProvider(Application* application) :
        ServiceProvider(application)
    {
        /// Nothing
    }

    void ImageServiceProvider::DoRegister(Application* application)
    {
        application->Singleton<ImageLoader>();
        application->Singleton<StbImageImporter>();
        application->Singleton<StbImageExporter>();
    }

    void ImageServiceProvider::DoBoot(Application* application)
    {
        ImageLoader* imageLoader = application->Resolve<ImageLoader>();

        imageLoader->RegisterImporter(application->Resolve<StbImageImporter>());
        imageLoader->RegisterExporter(application->Resolve<StbImageExporter>());
    }
}
