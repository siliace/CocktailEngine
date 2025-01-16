#include <pthread.h>
#include <unistd.h>

#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Concurrency/ThreadUtils.hpp>

namespace Ck
{
	namespace
	{
		unsigned int GetProcessAffinityMask()
		{
			const unsigned int coreCount = sysconf(_SC_NPROCESSORS_ONLN);

			cpu_set_t processAffinity;
			CPU_ZERO(&processAffinity);
			int error = sched_getaffinity(getpid(), sizeof(cpu_set_t), &processAffinity);
			if (error)
				throw SystemError::GetLastError();

			unsigned int processAffinityMask = 0;
			for (unsigned int i = 0; i < coreCount; i++)
			{
				if (CPU_ISSET(i, &processAffinity))
					processAffinityMask |= Bit(i);
			}

			return processAffinityMask;
		}
	}

	std::thread::native_handle_type ThreadUtils::GetCurrentThreadHandle()
	{
		return pthread_self();
	}

	void ThreadUtils::SetName(std::thread::native_handle_type threadHandle, std::string_view name)
	{
		int error = pthread_setname_np((pthread_t)threadHandle, name.data());
		if (error)
			throw SystemError::GetLastError();
	}

	void ThreadUtils::SetAffinity(std::thread::native_handle_type threadHandle, unsigned int affinityMask)
	{
		if (affinityMask == 0)
			throw std::runtime_error("Cannot assign a null affinity mask to a thread");

		unsigned int processAffinityMask = GetProcessAffinityMask();
		if ((affinityMask | processAffinityMask) != processAffinityMask)
		{
			Log::Warning("Thread affinity mask {} must be a subset of process affinity mask {}", affinityMask, processAffinityMask);
			affinityMask = static_cast<unsigned int>(processAffinityMask);
		}

		cpu_set_t threadAffinity;
		CPU_ZERO(&threadAffinity);
		for (unsigned int i = 0; i < sysconf(_SC_NPROCESSORS_ONLN); i++)
		{
			if (affinityMask & Bit(i))
				CPU_SET(i, &threadAffinity);
		}

		int error = pthread_setaffinity_np((pthread_t)threadHandle, sizeof(cpu_set_t), &threadAffinity);
		if (error)
			throw SystemError::GetLastError();
	}
}
