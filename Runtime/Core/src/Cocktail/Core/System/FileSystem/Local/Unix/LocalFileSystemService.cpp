#include <unistd.h>

#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemDriver.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemService.hpp>

namespace Ck::Detail::Unix
{
	std::filesystem::path LocalFileSystemService::GetWorkingDirectory() const
	{
		char* buffer = getcwd(nullptr, 0);
		if (!buffer)
			throw SystemError::GetLastError();

		std::filesystem::path workingDirectory(buffer);
		free(buffer);

		return workingDirectory;
	}

    void LocalFileSystemService::SetWorkingDirectory(const std::filesystem::path& workingDirectory)
    {
		if (chdir(workingDirectory.c_str()) != 0)
			throw SystemError::GetLastError();
    }
}
