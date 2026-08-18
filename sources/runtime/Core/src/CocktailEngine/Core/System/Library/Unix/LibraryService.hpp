#ifndef COCKTAILENGINE_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP

#include <CocktailEngine/Core/System/Library/LibraryService.hpp>

namespace Ck::Detail::Unix
{
	class LibraryService : public Ck::LibraryService
	{
	public:

		UniquePtr<Ck::Library> LoadLibrary(StringView name) override;
		
		void AddLibraryDirectory(const Path& path) override;

		void RemoveLibraryDirectory(const Path& path) override;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP
