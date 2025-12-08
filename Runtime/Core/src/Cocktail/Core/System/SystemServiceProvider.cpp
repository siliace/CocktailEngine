#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Memory/Allocator/MallocAllocator.hpp>
#include <Cocktail/Core/System/SystemServiceProvider.hpp>
#include <Cocktail/Core/System/SystemServiceProviderImpl.hpp>
#include <Cocktail/Core/System/FileSystem/StorageService.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>
#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystemService.hpp>

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
		application->Invoke([&](StorageService* storage, LocalFileSystemService* fileSystemService) {
			storage->MountExternal(storage->GetDefaultScheme(), fileSystemService->GetRootDriver());
		});

		application->Invoke([](StorageService* storage, EmbeddedFileSystemDriver* embeddedFileSystemDriver) {
			storage->MountExternal(CK_TEXT("builtin"), embeddedFileSystemDriver);
		});
	}
}
