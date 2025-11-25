#ifndef COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP

#include <Cocktail/Core/System/Library/LibraryService.hpp>

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

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP
