#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Library/Unix/Library.hpp>
#include <Cocktail/Core/System/Library/Unix/LibraryService.hpp>

namespace Ck::Detail::Unix
{
	std::unique_ptr<Ck::Library> LibraryService::LoadLibrary(StringView name)
	{
		return std::make_unique<Library>(name);
	}

	void LibraryService::AddLibraryDirectory(const Path& path)
	{
	}

	void LibraryService::RemoveLibraryDirectory(const Path& path)
	{
	}
}
