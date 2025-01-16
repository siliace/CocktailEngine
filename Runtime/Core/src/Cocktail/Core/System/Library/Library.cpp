#include <Cocktail/Core/System/Library/Library.hpp>

namespace Ck
{
	std::unique_ptr<Library> Library::LoadLibrary(std::string_view name)
	{
		return ResolveFacadeInstance()->LoadLibrary(name);
	}
	
	void Library::AddLibraryDirectory(const std::filesystem::path& path)
	{
		return ResolveFacadeInstance()->AddLibraryDirectory(path);
	}

	void Library::RemoveLibraryDirectory(const std::filesystem::path& path)
	{
		return ResolveFacadeInstance()->RemoveLibraryDirectory(path);
	}
}
