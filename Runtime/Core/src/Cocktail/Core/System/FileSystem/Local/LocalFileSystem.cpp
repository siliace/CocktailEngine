#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystem.hpp>

namespace Ck
{
	Ref<LocalFileSystemDriver> LocalFileSystem::CreateLocalDriver(std::filesystem::path location, bool followLink, bool lazyRootCreation)
	{
		return ResolveFacadeInstance()->CreateLocalDriver(std::move(location), followLink, lazyRootCreation);
	}
}
