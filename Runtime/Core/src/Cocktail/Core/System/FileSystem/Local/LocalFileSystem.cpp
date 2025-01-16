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
}
