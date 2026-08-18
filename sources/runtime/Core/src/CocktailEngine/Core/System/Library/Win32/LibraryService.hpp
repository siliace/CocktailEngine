#ifndef COCKTAILENGINE_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYSERVICE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYSERVICE_HPP

#include <CocktailEngine/Core/HashMap.hpp>
#include <CocktailEngine/Core/System/Library/LibraryService.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	class LibraryService : public Ck::LibraryService
	{
	public:

		UniquePtr<Ck::Library> LoadLibrary(StringView name) override;
		
		void AddLibraryDirectory(const Path& path) override;

		void RemoveLibraryDirectory(const Path& path) override;

	private:

		HashMap<Path, DLL_DIRECTORY_COOKIE> mDirectoryCookies;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYSERVICE_HPP
