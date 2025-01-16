#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Library/Unix/LibraryService.hpp>
#include <Cocktail/Core/System/Library/Unix/LibraryModule.hpp>

namespace Ck::Detail::Unix
{
	Ref<Ck::LibraryModule> LibraryService::LoadLibrary(std::string_view name)
	{
		return LibraryModule::New(name);
	}

	void LibraryService::AddLibraryDirectory(const std::filesystem::path& path)
	{
	}

	void LibraryService::RemoveLibraryDirectory(const std::filesystem::path& path)
	{
	}
}
