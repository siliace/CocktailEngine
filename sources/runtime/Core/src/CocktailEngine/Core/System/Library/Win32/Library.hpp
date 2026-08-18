#ifndef COCKTAILENGINE_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYMODULE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYMODULE_HPP

#include <CocktailEngine/Core/System/Library/Library.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	class Library : public Ck::Library
	{
	public:

		explicit Library(StringView name);

		~Library() override;

		FunctionPtr LoadFunction(const AnsiChar* functionName) override;

		void* GetSystemHandle() const override;

	private:

		HMODULE mHandle;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_LIBRARY_WIN32_LIBRARYMODULE_HPP
