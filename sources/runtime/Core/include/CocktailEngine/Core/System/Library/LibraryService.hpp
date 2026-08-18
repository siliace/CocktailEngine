#ifndef COCKTAILENGINE_CORE_SYSTEM_LIBRARY_LIBRARYSERVICE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_LIBRARY_LIBRARYSERVICE_HPP

#include <CocktailEngine/Core/Memory/UniquePtr.hpp>
#include <CocktailEngine/Core/System/FileSystem/Path.hpp>

namespace Ck
{
	class Library;
	
	/**
	 * \brief Service interacting with the system's libraries
	 */
	class LibraryService
	{
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~LibraryService() = default;
		
		/**
		 * \brief Load a LibraryModule from its name
		 * \param name The name of the library to load
		 * \return The loaded LibraryModule
		 */
		virtual UniquePtr<Library> LoadLibrary(StringView name) = 0;

		/**
		 * \brief 
		 * \param path 
		 */
		virtual void AddLibraryDirectory(const Path& path) = 0;

		/**
		 * \brief 
		 * \param path 
		 */
		virtual void RemoveLibraryDirectory(const Path& path) = 0;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_LIBRARY_LIBRARYSERVICE_HPP
