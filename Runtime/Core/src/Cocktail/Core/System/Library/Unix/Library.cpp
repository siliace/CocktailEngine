#include <dlfcn.h>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Library/Unix/Library.hpp>

namespace Ck::Detail::Unix
{
	COCKTAIL_DECLARE_EXCEPTION(LibraryOpenException, "Failed to open library");

	Library::Library(std::string_view name)
	{
		mHandle = dlopen(name.data(), RTLD_LOCAL | RTLD_LAZY);
		if (!mHandle)
			throw LibraryOpenException(dlerror());
	}

	Library::~Library()
	{
		dlclose(mHandle);
	}

	Library::FunctionPtr Library::LoadFunction(std::string_view functionName)
	{
		return reinterpret_cast<FunctionPtr>(dlsym(mHandle, functionName.data()));
	}

	void* Library::GetSystemHandle() const
	{
		return mHandle;
	}
}
