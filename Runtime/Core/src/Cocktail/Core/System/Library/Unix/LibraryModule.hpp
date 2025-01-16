#ifndef COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYMODULE_HPP
#define COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYMODULE_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/Library/LibraryModule.hpp>

namespace Ck::Detail::Unix
{
	class LibraryModule : public Inherit<LibraryModule, Object, Ck::LibraryModule>
	{
	public:

		explicit LibraryModule(std::string_view name);

		~LibraryModule() override;

		FunctionPtr LoadFunction(std::string_view functionName) override;

		void* GetSystemHandle() const override;

	private:

        void* mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARYMODULE_HPP
