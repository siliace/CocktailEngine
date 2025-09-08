#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectory.hpp>

namespace Ck::Detail::Win32
{
	LocalDirectory::LocalDirectory(const Path& path) :
		mPath(path)
	{
		LUID privilege;
		HANDLE accessToken;

		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &accessToken) == FALSE)
			throw SystemError::GetLastError();

		if (LookupPrivilegeValue(nullptr, SE_BACKUP_NAME, &privilege) == FALSE)
			throw SystemError::GetLastError();

		TOKEN_PRIVILEGES tpPrivileges;
		tpPrivileges.PrivilegeCount = 1;
		tpPrivileges.Privileges[0].Luid = privilege;
		tpPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (AdjustTokenPrivileges(accessToken, FALSE, &tpPrivileges, 0, nullptr, nullptr) == FALSE)
			throw SystemError::GetLastError();

		mHandle = CreateFileW(path.ToString().GetData(),
		                      GENERIC_READ,
		                      FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
		                      nullptr,
		                      OPEN_EXISTING,
		                      FILE_FLAG_BACKUP_SEMANTICS,
		                      nullptr
		);

		if (mHandle == INVALID_HANDLE_VALUE)
			throw SystemError::GetLastError();
	}

	LocalDirectory::~LocalDirectory()
	{
		CloseHandle(mHandle);
	}

	Array<Path> LocalDirectory::GetContent() const
	{
		String contentPath = mPath.ToString().Append(CK_TEXT('*'));

		WIN32_FIND_DATAW result;
		HANDLE findHandle = FindFirstFile(contentPath.GetData(), &result);
		if (findHandle == INVALID_HANDLE_VALUE)
			throw SystemError::GetLastError();

		Array<Path> content;

		do
		{
			content.Emplace(result.cFileName);
		} while (FindNextFileW(findHandle, &result));

		FindClose(findHandle);

		return content;
	}

	const Path& LocalDirectory::GetPath() const
	{
		return mPath;
	}

	void* LocalDirectory::GetSystemHandle() const
	{
		return mHandle;
	}
}
