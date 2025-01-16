#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystem.hpp>

namespace Ck
{
	std::filesystem::path LocalFileSystem::GetWorkingDirectory()
	{
		return ResolveFacadeInstance()->GetWorkingDirectory();
	}

    void LocalFileSystem::SetWorkingDirectory(const std::filesystem::path& workingDirectory)
    {
		return ResolveFacadeInstance()->SetWorkingDirectory(workingDirectory);
    }

    Ref<LocalFileSystemDriver> LocalFileSystem::CreateLocalDriver(std::filesystem::path location, bool followLink, bool lazyRootCreation)
    {
		return ResolveFacadeInstance()->CreateLocalDriver(std::move(location), followLink, lazyRootCreation);
	}
}
