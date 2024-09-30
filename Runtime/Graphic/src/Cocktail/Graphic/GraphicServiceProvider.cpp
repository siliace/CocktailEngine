#include <cmrc/cmrc.hpp>

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>

#include <Cocktail/Graphic/GraphicServiceProvider.hpp>

CMRC_DECLARE(Graphic);

namespace Ck
{
	GraphicServiceProvider::GraphicServiceProvider(Ref<Application> application) :
		Super(std::move(application))
	{
		/// Nothing
	}

	void GraphicServiceProvider::DoBoot(const Ref<Application>& application)
	{
		application->Invoke([](Ref<EmbeddedFileSystemDriver> embeddedDriver) {
			embeddedDriver->Register(cmrc::Graphic::get_filesystem());
		});
	}

	COCKTAIL_REGISTER_SERVICE_PROVIDER(GraphicServiceProvider);
}
