#ifndef COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP

#include <Cocktail/Core/System/Library/LibraryService.hpp>

namespace Ck::Detail::Unix
{
	class LibraryService : public Ck::LibraryService
	{
	public:

		std::unique_ptr<Ck::Library> LoadLibrary(std::string_view name) override;
		
		void AddLibraryDirectory(const std::filesystem::path& path) override;

		void RemoveLibraryDirectory(const std::filesystem::path& path) override;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP
