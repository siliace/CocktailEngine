#ifndef COCKTAIL_CORE_SYSTEM_LIBRARY_LIBRARYMODULE_HPP
#define COCKTAIL_CORE_SYSTEM_LIBRARY_LIBRARYMODULE_HPP

#include <string>

#include <Cocktail/Core/Meta/Implements.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class LibraryModule : public Implements<LibraryModule, Interface>
	{
	public:

		/**
		 * \brief 
		 */
		using FunctionPtr = int(*)();

		/**
		 * \brief 
		 * \param functionName 
		 * \return 
		 */
		virtual FunctionPtr LoadFunction(std::string_view functionName) = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual void* GetSystemHandle() const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_LIBRARYMODULE_HPP
