#ifndef COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP

#include <unordered_map>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/Library/LibraryService.hpp>

namespace Ck::Detail::Unix
{
	class LibraryService : public Inherit<LibraryService, Object, Ck::LibraryService>
	{
	public:

		Ref<Ck::LibraryModule> LoadLibrary(std::string_view name) override;
		
		void AddLibraryDirectory(const std::filesystem::path& path) override;

		void RemoveLibraryDirectory(const std::filesystem::path& path) override;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYSERVICE_HPP
