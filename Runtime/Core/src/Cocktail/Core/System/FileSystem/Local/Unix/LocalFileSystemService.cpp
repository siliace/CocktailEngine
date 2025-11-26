#include <unistd.h>

#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemDriver.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemService.hpp>

namespace Ck::Detail::Unix
{
    LocalFileSystemService::LocalFileSystemService()
    {
        mRootDriver = LocalFileSystemService::CreateDriver(Path::Empty);
    }

	Path LocalFileSystemService::GetWorkingDirectory() const
	{
		Utf8Char* buffer = reinterpret_cast<Utf8Char*>(getcwd(nullptr, 0));
		if (!buffer)
			throw SystemError::GetLastError();

		Path workingDirectory(buffer);
		free(buffer);

		return workingDirectory;
	}

    void LocalFileSystemService::SetWorkingDirectory(const Path& workingDirectory)
    {
		if (chdir(reinterpret_cast<const AnsiChar*>(workingDirectory.ToString().GetData())) != 0)
			throw SystemError::GetLastError();
    }

    Path LocalFileSystemService::GetTempDirectoryPath() const
    {
		return CK_TEXT("/var/tmp");
    }

    Ck::LocalFileSystemDriver* LocalFileSystemService::GetRootDriver() const
    {
	    return mRootDriver.Get();
    }

    UniquePtr<Ck::LocalFileSystemDriver> LocalFileSystemService::CreateDriver(const Path& base)
    {
		return MakeUnique<LocalFileSystemDriver>();
    }
}
