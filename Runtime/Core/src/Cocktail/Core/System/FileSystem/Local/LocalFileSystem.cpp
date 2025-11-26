#include <Cocktail/Core/System/FileSystem/Local/LocalFileSystem.hpp>

namespace Ck
{
	Path LocalFileSystem::GetWorkingDirectory()
	{
		return ResolveFacadeInstance()->GetWorkingDirectory();
	}

    void LocalFileSystem::SetWorkingDirectory(const Path& workingDirectory)
    {
		return ResolveFacadeInstance()->SetWorkingDirectory(workingDirectory);
    }

    Path LocalFileSystem::GetTempDirectoryPath()
    {
		return ResolveFacadeInstance()->GetTempDirectoryPath();
    }
    LocalFileSystemDriver* LocalFileSystem::GetRootDriver()
    {
	    return ResolveFacadeInstance()->GetRootDriver();
    }

    UniquePtr<LocalFileSystemDriver> LocalFileSystem::CreateDriver(const Path& base)
    {
        return ResolveFacadeInstance()->CreateDriver(std::move(base));
    }
}
