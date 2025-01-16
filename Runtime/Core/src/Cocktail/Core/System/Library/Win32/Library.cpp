#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Library/Win32/Library.hpp>

namespace Ck::Detail::Win32
{
	Library::Library(std::string_view name)
	{
		mHandle = LoadLibraryA(name.data());
		if (!mHandle)
			throw SystemError::GetLastError();
	}

	Library::~Library()
	{
		FreeLibrary(mHandle);
	}

	Library::FunctionPtr Library::LoadFunction(std::string_view functionName)
	{
		return reinterpret_cast<FunctionPtr>(GetProcAddress(mHandle, functionName.data()));
	}

	void* Library::GetSystemHandle() const
	{
		return mHandle;
	}
}
