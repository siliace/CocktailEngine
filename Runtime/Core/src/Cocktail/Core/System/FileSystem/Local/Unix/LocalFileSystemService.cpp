#include <unistd.h>

#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemDriver.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemService.hpp>

namespace Ck::Detail::Unix
{
	Path LocalFileSystemService::GetWorkingDirectory() const
	{
		char* buffer = getcwd(nullptr, 0);
		if (!buffer)
			throw SystemError::GetLastError();

		Path workingDirectory(buffer);
		free(buffer);

		return workingDirectory;
	}

    void LocalFileSystemService::SetWorkingDirectory(const Path& workingDirectory)
    {
		if (chdir(workingDirectory.ToString().GetData()) != 0)
			throw SystemError::GetLastError();
    }

    Path LocalFileSystemService::GetTempDirectoryPath() const
    {
		return CK_TEXT("/var/tmp");
    }
}
