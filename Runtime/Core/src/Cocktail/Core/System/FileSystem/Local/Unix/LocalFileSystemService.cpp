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

		Path workingDirectory(CK_ANSI_TO_TEXT(buffer));
		free(buffer);

		return workingDirectory;
	}

    void LocalFileSystemService::SetWorkingDirectory(const Path& workingDirectory)
    {
		if (chdir(CK_TEXT_TO_ANSI(workingDirectory.ToString().GetData())) != 0)
			throw SystemError::GetLastError();
    }

    Path LocalFileSystemService::GetTempDirectoryPath() const
    {
		return CK_TEXT("/var/tmp");
    }

    std::unique_ptr<Ck::LocalFileSystemDriver> LocalFileSystemService::CreateDriver(const Path& base)
    {
		return std::make_unique<LocalFileSystemDriver>();
    }
}
