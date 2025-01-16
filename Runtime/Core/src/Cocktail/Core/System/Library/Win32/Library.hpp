#ifndef COCKTAIL_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYMODULE_HPP
#define COCKTAIL_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYMODULE_HPP

#include <Cocktail/Core/System/Library/Library.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	class Library : public Ck::Library
	{
	public:

		explicit Library(std::string_view name);

		~Library() override;

		FunctionPtr LoadFunction(std::string_view functionName) override;

		void* GetSystemHandle() const override;

	private:

		HMODULE mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYMODULE_HPP
