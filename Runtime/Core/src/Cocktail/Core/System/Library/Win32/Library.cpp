#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Library/Win32/Library.hpp>

namespace Ck::Detail::Win32
{
	Library::Library(StringView name)
	{
		mHandle = ::LoadLibraryW(name.GetData());
		if (!mHandle)
			throw SystemError::GetLastError();
	}

	Library::~Library()
	{
		FreeLibrary(mHandle);
	}

	Library::FunctionPtr Library::LoadFunction(const AnsiChar* functionName)
	{
		return reinterpret_cast<FunctionPtr>(GetProcAddress(mHandle, functionName));
	}

	void* Library::GetSystemHandle() const
	{
		return mHandle;
	}
}
