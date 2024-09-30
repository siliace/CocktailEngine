#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Library/Win32/LibraryModule.hpp>

namespace Ck::Detail::Win32
{
	LibraryModule::LibraryModule(std::string_view name)
	{
		mHandle = LoadLibraryA(name.data());
		if (!mHandle)
			throw SystemError::GetLastError();
	}

	LibraryModule::~LibraryModule()
	{
		FreeLibrary(mHandle);
	}

	LibraryModule::FunctionPtr LibraryModule::LoadFunction(std::string_view functionName)
	{
		return reinterpret_cast<FunctionPtr>(GetProcAddress(mHandle, functionName.data()));
	}

	void* LibraryModule::GetSystemHandle() const
	{
		return mHandle;
	}
}
