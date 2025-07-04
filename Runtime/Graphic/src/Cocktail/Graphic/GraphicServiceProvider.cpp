#include <cmrc/cmrc.hpp>

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>

#include <Cocktail/Graphic/GraphicServiceProvider.hpp>

CMRC_DECLARE(Graphic);

namespace Ck
{
	CK_REGISTER_SERVICE_PROVIDER(GraphicServiceProvider);

	GraphicServiceProvider::GraphicServiceProvider(Application* application) :
		ServiceProvider(std::move(application))
	{
		/// Nothing
	}

	void GraphicServiceProvider::DoBoot(Application* application)
	{
		application->Invoke([](EmbeddedFileSystemDriver* embeddedDriver) {
			embeddedDriver->Register(cmrc::Graphic::get_filesystem());
		});
	}
}
