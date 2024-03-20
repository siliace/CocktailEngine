#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Image/ImageLoader.hpp>
#include <Cocktail/Core/Image/ImageServiceProvider.hpp>
#include <Cocktail/Core/Image/StbLoader/StbImageImporter.hpp>

namespace Ck
{
	ImageServiceProvider::ImageServiceProvider(const Ref<Application>& application) :
		Extends<ImageServiceProvider, ServiceProvider>(application)
	{
		/// Nothing
	}

	void ImageServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<ImageLoader>();
		application->Singleton<StbImageImporter>();
	}

	void ImageServiceProvider::DoBoot(const Ref<Application>& application)
	{
		application->Resolve<ImageLoader>()->RegisterImporter(
			application->Resolve<StbImageImporter>()
		);
	}
}
