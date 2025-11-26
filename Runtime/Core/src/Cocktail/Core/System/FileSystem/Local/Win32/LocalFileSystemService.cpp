#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemDriver.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemService.hpp>

namespace Ck::Detail::Win32
{
    LocalFileSystemService::LocalFileSystemService()
    {
        mRootDriver = LocalFileSystemService::CreateDriver(Path::Empty);
    }

    Path LocalFileSystemService::GetWorkingDirectory() const
	{
		DWORD size = GetCurrentDirectory(0, nullptr);
		if (size == 0)
			throw SystemError::GetLastError();

		TextChar* buffer = COCKTAIL_STACK_ALLOC(TextChar, size);
		if (GetCurrentDirectory(size, buffer))
			throw SystemError::GetLastError();

		return Path(buffer, size);
	}

    void LocalFileSystemService::SetWorkingDirectory(const Path& workingDirectory)
    {
		if (SetCurrentDirectory(workingDirectory.ToString().GetData()) == FALSE)
			throw SystemError::GetLastError();
    }

    Path LocalFileSystemService::GetTempDirectoryPath() const
	{
		DWORD size = ::GetTempPath(0, nullptr);
		if (size == 0)
			throw SystemError::GetLastError();

		TextChar* buffer = COCKTAIL_STACK_ALLOC(TextChar, size);
		if (::GetTempPath(size, buffer) == 0)
			throw SystemError::GetLastError();

		return Path(buffer, size - 1);
	}

    Ck::LocalFileSystemDriver* LocalFileSystemService::GetRootDriver() const
    {
	    return mRootDriver.Get();
    }

    UniquePtr<Ck::LocalFileSystemDriver> LocalFileSystemService::CreateDriver(const Path& base)
    {
		return MakeUnique<LocalFileSystemDriver>(base);
    }
}
