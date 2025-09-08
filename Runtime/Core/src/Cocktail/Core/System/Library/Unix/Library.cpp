#include <dlfcn.h>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/System/Library/Unix/Library.hpp>

namespace Ck::Detail::Unix
{
	COCKTAIL_DECLARE_EXCEPTION_FROM(LibraryOpenException, RuntimeException);

	Library::Library(StringView name)
	{
		mHandle = dlopen(CK_TEXT_TO_ANSI(name.GetData()), RTLD_LOCAL | RTLD_LAZY);
		if (!mHandle)
			throw LibraryOpenException(CK_ANSI_TO_TEXT(dlerror()));
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
