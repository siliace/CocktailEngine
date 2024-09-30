#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectory.hpp>

namespace Ck::Detail::Win32
{
	LocalDirectory::LocalDirectory(const std::filesystem::path& path) :
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

		mHandle = CreateFileW(path.c_str(),
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

	std::vector<std::filesystem::path> LocalDirectory::GetContent() const
	{
		std::filesystem::path base = mPath / "*";

		WIN32_FIND_DATAW result;
		HANDLE findHandle = FindFirstFileW(base.c_str(), &result);
		if (findHandle == INVALID_HANDLE_VALUE)
			throw SystemError::GetLastError();

		std::vector<std::filesystem::path> content;

		do
		{
			content.emplace_back(result.cFileName);
		}
		while (FindNextFileW(findHandle, &result));

		FindClose(findHandle);

		return content;
	}

	const std::filesystem::path& LocalDirectory::GetPath() const
	{
		return mPath;
	}

	void* LocalDirectory::GetSystemHandle() const
	{
		return mHandle;
	}
}
