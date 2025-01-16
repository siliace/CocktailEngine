#include <Cocktail/Core/System/Library/Library.hpp>

namespace Ck
{
	Ref<LibraryModule> Library::LoadLibrary(std::string_view name)
	{
		return ResolveFacadeInstance()->LoadLibrary(name);
	}
}
