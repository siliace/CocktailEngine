#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/SystemServiceProvider.hpp>
#include <Cocktail/Core/System/SystemServiceProviderImpl.hpp>
#include <Cocktail/Core/System/FileSystem/StorageService.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemService.hpp>

namespace Ck
{
	SystemServiceProvider::SystemServiceProvider(const Ref<Application>& application) :
		Extends<SystemServiceProvider, ServiceProvider>(application)
	{
		application->RegisterServiceProvider<Detail::SystemServiceProviderImpl>();
	}

	void SystemServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<StorageService>();
		application->Singleton<EmbeddedFileSystemDriver>();
	}

	void SystemServiceProvider::DoBoot(const Ref<Application>& application)
	{
		application->Invoke([](Ref<StorageService> storage, Ref<LocalFileSystemService> localFileSystemService) {
			storage->Mount(storage->GetDefaultProtocol(), localFileSystemService->CreateLocalDriver());
		});

		application->Invoke([](Ref<StorageService> storage, Ref<EmbeddedFileSystemDriver> embeddedFileSystemDriver) {
			storage->Mount("builtin", embeddedFileSystemDriver);
		});
	}
}
