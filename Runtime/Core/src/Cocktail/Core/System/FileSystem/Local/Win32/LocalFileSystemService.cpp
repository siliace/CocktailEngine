#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemService.hpp>

namespace Ck::Detail::Win32
{
	std::filesystem::path LocalFileSystemService::GetWorkingDirectory() const
	{
		std::filesystem::path::string_type buffer;
		DWORD size = GetCurrentDirectoryW(0, nullptr);
		if (size == 0)
			throw SystemError::GetLastError();

		buffer.resize(size + 1);
		if (GetCurrentDirectoryW(static_cast<DWORD>(buffer.length()), buffer.data()))
			throw SystemError::GetLastError();

		return buffer;
	}

    void LocalFileSystemService::SetWorkingDirectory(const std::filesystem::path& workingDirectory)
    {
		if (SetCurrentDirectoryW(workingDirectory.c_str()) == FALSE)
			throw SystemError::GetLastError();
    }
}
