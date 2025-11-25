#include <Cocktail/Core/System/Library/Library.hpp>

namespace Ck
{
	UniquePtr<Library> Library::LoadLibrary(StringView name)
	{
		return ResolveFacadeInstance()->LoadLibrary(name);
	}
	
	void Library::AddLibraryDirectory(const Path& path)
	{
		return ResolveFacadeInstance()->AddLibraryDirectory(path);
	}

	void Library::RemoveLibraryDirectory(const Path& path)
	{
		return ResolveFacadeInstance()->RemoveLibraryDirectory(path);
	}
}
