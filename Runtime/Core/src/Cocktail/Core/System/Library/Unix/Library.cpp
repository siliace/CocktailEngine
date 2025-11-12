#include <dlfcn.h>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/System/Library/Unix/Library.hpp>

namespace Ck::Detail::Unix
{
	COCKTAIL_DECLARE_EXCEPTION_FROM(LibraryOpenException, RuntimeException);

	Library::Library(StringView name)
	{
		mHandle = dlopen(reinterpret_cast<const AnsiChar*>(name.GetData()), RTLD_LOCAL | RTLD_LAZY);
		if (!mHandle)
			throw LibraryOpenException(reinterpret_cast<const Utf8Char*>(dlerror()));
	}

	Library::~Library()
	{
		dlclose(mHandle);
	}

	Library::FunctionPtr Library::LoadFunction(const AnsiChar* functionName)
	{
		return reinterpret_cast<FunctionPtr>(dlsym(mHandle, functionName));
	}

	void* Library::GetSystemHandle() const
	{
		return mHandle;
	}
}
