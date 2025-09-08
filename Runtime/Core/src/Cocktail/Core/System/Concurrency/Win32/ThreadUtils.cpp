#include <Cocktail/Core/Exception.hpp>
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

	void ThreadUtils::SetName(std::thread::native_handle_type threadHandle, StringView name)
	{
		SetThreadDescription(threadHandle, name.GetData());
	}

	void ThreadUtils::SetAffinity(std::thread::native_handle_type threadHandle, unsigned int affinityMask)
	{
		if (affinityMask == 0)
			throw RuntimeException(CK_TEXT("Cannot assign a null affinity mask to a thread"));

		DWORD_PTR processAffinityMask, systemAffinityMask;
		BOOL success = GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask);
		if (success == FALSE)
			throw SystemError::GetLastError();

		if ((affinityMask | processAffinityMask) != processAffinityMask)
		{
			CK_LOG(Detail::Win32::Win32LogCategory, LogLevel::Warning, CK_TEXT("Thread affinity mask {} must be a subset of process affinity mask {}"), affinityMask, processAffinityMask);
			affinityMask = static_cast<unsigned int>(processAffinityMask);
		}

		if (SetThreadAffinityMask(threadHandle, affinityMask) == 0)
			throw SystemError::GetLastError();
	}

	void ThreadUtils::SleepFor(const Duration& duration)
	{
		std::size_t sleepTime = std::max(duration.GetCount(TimeUnit::Milliseconds()), 1llu);

		// At this point, it's not useful to call successive sleep
		assert(sleepTime < std::numeric_limits<unsigned int>::max());

		Sleep(static_cast<DWORD>(sleepTime));
	}

	void ThreadUtils::SleepUntil(const Instant& instant, const Duration& duration)
	{
		// Assert the instant is not before now, otherwise we would wait forever
		assert(Instant::Now().IsBefore(instant));

		WaitUntil([&]() {
			return Instant::Now().IsBefore(instant);
		}, duration);
	}
}
