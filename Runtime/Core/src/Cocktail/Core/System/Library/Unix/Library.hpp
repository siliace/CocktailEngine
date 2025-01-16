#ifndef COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARY_HPP
#define COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARY_HPP

#include <Cocktail/Core/System/Library/Library.hpp>

namespace Ck::Detail::Unix
{
	class Library : public Ck::Library
	{
	public:

		explicit Library(std::string_view name);

		~Library() override;

		FunctionPtr LoadFunction(std::string_view functionName) override;

		void* GetSystemHandle() const override;

	private:

        void* mHandle;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_LIBRARY_UNIX_LIBRARY_HPP
