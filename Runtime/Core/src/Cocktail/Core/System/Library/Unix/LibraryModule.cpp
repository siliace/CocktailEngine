#include <dlfcn.h>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Library/Unix/LibraryModule.hpp>

namespace Ck::Detail::Unix
{
	COCKTAIL_DECLARE_EXCEPTION_BASE(LibraryOpenException, "Failed to open library", std::runtime_error);

	LibraryModule::LibraryModule(std::string_view name)
	{
		mHandle = dlopen(name.data(), RTLD_LOCAL | RTLD_LAZY);
		if (!mHandle)
			throw LibraryOpenException(dlerror());
	}

	LibraryModule::~LibraryModule()
	{
		dlclose(mHandle);
	}

	LibraryModule::FunctionPtr LibraryModule::LoadFunction(std::string_view functionName)
	{
		return reinterpret_cast<FunctionPtr>(dlsym(mHandle, functionName.data()));
	}

	void* LibraryModule::GetSystemHandle() const
	{
		return mHandle;
	}
}
