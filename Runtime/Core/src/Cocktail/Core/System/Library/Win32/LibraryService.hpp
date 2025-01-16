#ifndef COCKTAIL_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYSERVICE_HPP

#include <unordered_map>

#include <Cocktail/Core/System/Library/LibraryService.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	class LibraryService : public Ck::LibraryService
	{
	public:

		std::unique_ptr<Ck::Library> LoadLibrary(std::string_view name) override;
		
		void AddLibraryDirectory(const std::filesystem::path& path) override;

		void RemoveLibraryDirectory(const std::filesystem::path& path) override;

	private:

		std::unordered_map<std::string, DLL_DIRECTORY_COOKIE> mDirectoryCookies;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYSERVICE_HPP
