#ifndef COCKTAIL_CORE_SYSTEM_LIBRARY_LIBRARYSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_LIBRARY_LIBRARYSERVICE_HPP

#include <filesystem>

#include <Cocktail/Core/System/Library/LibraryModule.hpp>

namespace Ck
{
	/**
	 * \brief Service interacting with the system's libraries
	 */
	class LibraryService : public Implements<LibraryModule, Interface>
	{
	public:
		
		/**
		 * \brief Load a LibraryModule from its name
		 * \param name The name of the library to load
		 * \return The loaded LibraryModule
		 */
		virtual Ref<LibraryModule> LoadLibrary(std::string_view name) = 0;

		/**
		 * \brief 
		 * \param path 
		 */
		virtual void AddLibraryDirectory(const std::filesystem::path& path) = 0;

		/**
		 * \brief 
		 * \param path 
		 */
		virtual void RemoveLibraryDirectory(const std::filesystem::path& path) = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_LIBRARYSERVICE_HPP
