#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Library/Win32/Library.hpp>
#include <Cocktail/Core/System/Library/Win32/LibraryService.hpp>

namespace Ck::Detail::Win32
{
	UniquePtr<Ck::Library> LibraryService::LoadLibrary(StringView name)
	{
		return MakeUnique<Library>(name);
	}

	void LibraryService::AddLibraryDirectory(const Path& path)
	{
		DLL_DIRECTORY_COOKIE cookie = AddDllDirectory(path.ToString().GetData());
		if (cookie == nullptr)
			throw SystemError::GetLastError();

		mDirectoryCookies[path] = cookie;
	}

	void LibraryService::RemoveLibraryDirectory(const Path& path)
	{
		DLL_DIRECTORY_COOKIE cookie = mDirectoryCookies[path];
		if (!cookie)
			return;

		if (!RemoveDllDirectory(cookie))
			throw SystemError::GetLastError();
	}
}
