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
		 * \brief Load a LibraryModule from its name
		 * \param name The name of the library to load
		 * \return The loaded LibraryModule
		 */
		static Ref<LibraryModule> LoadLibrary(std::string_view name);
	};
}

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_LIBRARY_HPP
