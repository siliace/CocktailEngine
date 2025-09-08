#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/SystemServiceProvider.hpp>
#include <Cocktail/Core/System/SystemServiceProviderImpl.hpp>
#include <Cocktail/Core/System/FileSystem/StorageService.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemDriver.hpp>

namespace Ck
{
	SystemServiceProvider::SystemServiceProvider(Application* application) :
		ServiceProvider(application)
	{
		application->RegisterServiceProvider<Detail::SystemServiceProviderImpl>();
	}

	void SystemServiceProvider::DoRegister(Application* application)
	{
		application->Singleton<StorageService>();
		application->Singleton<EmbeddedFileSystemDriver>();
	}

	void SystemServiceProvider::DoBoot(Application* application)
	{
		application->Invoke([](StorageService* storage, EmbeddedFileSystemDriver* embeddedFileSystemDriver) {
			storage->Mount(CK_TEXT("builtin"), embeddedFileSystemDriver);
		});

		application->Invoke([](StorageService* storage, LocalFileSystemDriver* localFileSystemDriver) {
			storage->Mount(storage->GetDefaultScheme(), localFileSystemDriver);
		});
	}
}
