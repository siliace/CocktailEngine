#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemDriver.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemService.hpp>

namespace Ck::Detail::Win32
{
	Ref<Ck::LocalFileSystemDriver> LocalFileSystemService::CreateLocalDriver(std::filesystem::path location, bool followLink, bool lazyRootCreation)
	{
		return LocalFileSystemDriver::New(std::move(location), followLink, lazyRootCreation);
	}
}
