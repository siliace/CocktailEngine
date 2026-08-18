#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Image/ImageLoader.hpp>
#include <CocktailEngine/Core/Image/ImageServiceProvider.hpp>
#include <CocktailEngine/Core/Image/StbLoader/StbImageExporter.hpp>
#include <CocktailEngine/Core/Image/StbLoader/StbImageImporter.hpp>

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
