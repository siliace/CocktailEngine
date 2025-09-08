#ifndef COCKTAIL_CORE_SYSTEM_LIBRARY_LIBRARY_HPP
#define COCKTAIL_CORE_SYSTEM_LIBRARY_LIBRARY_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/System/Library/LibraryService.hpp>

namespace Ck
{
	/**
	 * \brief Facade to LibraryService
	 */
	class COCKTAIL_CORE_API Library : public ServiceFacade<LibraryService>
	{
	public:

		/**
		 * \brief 
		 */
		using FunctionPtr = int(*)();

		/**
		 * \brief Load a LibraryModule from its name
		 * \param name The name of the library to load
		 * \return The loaded LibraryModule
		 */
		static std::unique_ptr<Library> LoadLibrary(StringView name);

		/**
		 * \brief 
		 * \param path 
		 */
		static void AddLibraryDirectory(const Path& path);

		/**
		 * \brief 
		 * \param path 
		 */
		static void RemoveLibraryDirectory(const Path& path);

		/**
		 * \brief Destructor
		 */
		virtual ~Library() = default;

		/**
		 * \brief 
		 * \param functionName 
		 * \return 
		 */
		virtual FunctionPtr LoadFunction(const AnsiChar* functionName) = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual void* GetSystemHandle() const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_LIBRARY_HPP
