#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Library/Win32/Library.hpp>
#include <Cocktail/Core/System/Library/Win32/LibraryService.hpp>

namespace Ck::Detail::Win32
{
	std::unique_ptr<Ck::Library> LibraryService::LoadLibrary(std::string_view name)
	{
		return std::make_unique<Library>(name);
	}

	void LibraryService::AddLibraryDirectory(const std::filesystem::path& path)
	{
		DLL_DIRECTORY_COOKIE cookie = AddDllDirectory(path.c_str());
		if (cookie == nullptr)
			throw SystemError::GetLastError();

		mDirectoryCookies[path.string()] = cookie;
	}

	void LibraryService::RemoveLibraryDirectory(const std::filesystem::path& path)
	{
		DLL_DIRECTORY_COOKIE cookie = mDirectoryCookies[path.string()];
		if (!cookie)
			return;

		if (!RemoveDllDirectory(cookie))
			throw SystemError::GetLastError();
	}
}
