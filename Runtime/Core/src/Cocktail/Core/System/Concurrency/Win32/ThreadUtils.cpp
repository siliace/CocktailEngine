#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Concurrency/ThreadUtils.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>
#include <Cocktail/Core/System/Win32/WindowsServiceProvider.hpp>

namespace Ck
{
	std::thread::native_handle_type ThreadUtils::GetCurrentThreadHandle()
	{
		return GetCurrentThread();
	}

	void ThreadUtils::SetName(std::thread::native_handle_type threadHandle, std::string_view name)
	{
		std::wstring wName(name.begin(), name.end());
		SetThreadDescription(threadHandle, wName.c_str());
	}

	void ThreadUtils::SetAffinity(std::thread::native_handle_type threadHandle, unsigned int affinityMask)
	{
		if (affinityMask == 0)
			throw std::runtime_error("Cannot assign a null affinity mask to a thread");

		DWORD_PTR processAffinityMask, systemAffinityMask;
		BOOL success = GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask);
		if (success == FALSE)
			throw SystemError::GetLastError();

		if ((affinityMask | processAffinityMask) != processAffinityMask)
		{
			CK_LOG(Detail::Win32::Win32LogCategory, LogLevel::Warning, "Thread affinity mask {} must be a subset of process affinity mask {}", affinityMask, processAffinityMask);
			affinityMask = static_cast<unsigned int>(processAffinityMask);
		}

		if (SetThreadAffinityMask(threadHandle, affinityMask) == 0)
			throw SystemError::GetLastError();
	}
}
