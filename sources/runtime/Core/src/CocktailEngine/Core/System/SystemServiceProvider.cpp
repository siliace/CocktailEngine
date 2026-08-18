#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Memory/Allocator/MallocAllocator.hpp>
#include <CocktailEngine/Core/System/FileSystem/Local/LocalFileSystemService.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemDriver.hpp>
#include <CocktailEngine/Core/System/FileSystem/StorageService.hpp>
#include <CocktailEngine/Core/System/SystemServiceProvider.hpp>
#include <CocktailEngine/Core/System/SystemServiceProviderImpl.hpp>

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
	    application->Singleton<MallocAllocator>();
	}

	void SystemServiceProvider::DoBoot(Application* application)
	{
		application->Invoke([&](StorageService* storage, LocalFileSystemService* fileSystemService) {
			storage->MountExternal(storage->GetDefaultScheme(), fileSystemService->GetRootDriver());
			storage->Mount(CK_TEXT("memory"), MakeUnique<MemoryFileSystemDriver>());
		});
	}
}
