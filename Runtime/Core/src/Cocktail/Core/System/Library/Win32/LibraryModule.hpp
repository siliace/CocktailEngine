#ifndef COCKTAIL_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYMODULE_HPP
#define COCKTAIL_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYMODULE_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/Library/LibraryModule.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	class LibraryModule : public Inherit<LibraryModule, Object, Ck::LibraryModule>
	{
	public:

		explicit LibraryModule(std::string_view name);

		~LibraryModule() override;

		FunctionPtr LoadFunction(std::string_view functionName) override;

		void* GetSystemHandle() const override;

	private:

		HMODULE mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYMODULE_HPP
