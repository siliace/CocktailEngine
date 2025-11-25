#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Concurrency/Runnable.hpp>
#include <Cocktail/Core/System/Concurrency/Thread.hpp>
#include <Cocktail/Core/System/Concurrency/Win32/Thread.hpp>
#include <Cocktail/Core/System/Concurrency/Win32/ThreadManager.hpp>
#include <Cocktail/Core/System/Win32/WindowsServiceProvider.hpp>

#include "Cocktail/Core/Exception.hpp"

namespace Ck
{
	unsigned int Thread::GetCoreCount()
	{
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);

		return systemInfo.dwNumberOfProcessors;
	}

	void Thread::SleepFor(const Duration& duration)
	{
		std::size_t sleepTime = std::max(duration.GetCount(TimeUnit::Milliseconds()), 1llu);

		// At this point, it's not useful to call successive sleep
		assert(sleepTime < std::numeric_limits<unsigned int>::max());

		Sleep(static_cast<DWORD>(sleepTime));
	}

	void Thread::Yield()
	{
		if (::SwitchToThread() == FALSE)
			::Sleep(0);
	}

	UniquePtr<Thread> Thread::Create(Runnable* runnable, StringView name, unsigned int stackSize)
	{
		return MakeUnique<Detail::Win32::Thread>(runnable, name, stackSize);
	}

	namespace Detail::Win32
	{
		namespace
		{
			int ToWin32Type(Thread::Priority priority)
			{
				switch (priority)
				{
				case Thread::Priority::Normal:
				case Thread::Priority::SlightlyBelowNormal:
					return THREAD_PRIORITY_NORMAL;

				case Thread::Priority::AboveNormal:
					return THREAD_PRIORITY_ABOVE_NORMAL;

				case Thread::Priority::BelowNormal:
					return THREAD_PRIORITY_BELOW_NORMAL;

				case Thread::Priority::Highest:
					return THREAD_PRIORITY_HIGHEST;

				case Thread::Priority::Lowest:
					return THREAD_PRIORITY_LOWEST;

				case Thread::Priority::TimeCritical:
					return THREAD_PRIORITY_TIME_CRITICAL;
				}

				COCKTAIL_UNREACHABLE();
			}
		}

		DWORD Thread::Run(LPVOID arg)
		{
			Thread* self = static_cast<Thread*>(arg);

			ThreadManager* threadManager = App::Resolve<ThreadManager>();
			threadManager->Register(self);

			self->mRunnable->Init();
			DWORD exitCode = self->mRunnable->Run();
			self->mRunnable->Exit();

			return exitCode;
		}

		Thread::Thread(HANDLE handle, unsigned int id, bool isMain) :
			mId(id),
			mMain(isMain),
			mHandle(handle),
			mRunnable(nullptr)
		{
			assert(mHandle != INVALID_HANDLE_VALUE);
			if (mMain)
				Thread::SetName(CK_TEXT("Main"));
		}

		Thread::Thread(Runnable* runnable, StringView name, unsigned int stackSize) :
			mId(0),
			mMain(false),
			mHandle(INVALID_HANDLE_VALUE),
			mRunnable(runnable)
		{
			mHandle = CreateThread(nullptr, stackSize, &Thread::Run, this, 0, &mId);
			if (mHandle == INVALID_HANDLE_VALUE)
				throw SystemError::GetLastError();

			SetName(name);
		}

		Thread::~Thread()
		{
			if (!mMain)
			{
				if (mHandle != INVALID_HANDLE_VALUE)
					Thread::Kill(true);

				ThreadManager* threadManager = App::Resolve<ThreadManager>();
				threadManager->Unregister(this);
			}
		}

		void Thread::Join()
		{
			assert(mHandle != INVALID_HANDLE_VALUE);

			if (WaitForSingleObject(mHandle, INFINITE) != WAIT_OBJECT_0)
				throw SystemError::GetLastError();
		}

		bool Thread::Join(const Duration& timeout)
		{
			assert(mHandle != INVALID_HANDLE_VALUE);

			DWORD result = WaitForSingleObject(mHandle, timeout.GetCount(TimeUnit::Milliseconds()));
			if (result == WAIT_FAILED)
				throw SystemError::GetLastError();

			return result == WAIT_OBJECT_0;
		}

		void Thread::Kill(bool waitForTermination)
		{
			assert(mHandle != INVALID_HANDLE_VALUE);

			if (mRunnable)
				mRunnable->Stop();

			if (waitForTermination)
			{
				if (WaitForSingleObject(mHandle, INFINITE) != WAIT_OBJECT_0)
					throw SystemError::GetLastError();
			}

			CloseHandle(mHandle);
			mHandle = INVALID_HANDLE_VALUE;
		}

		bool Thread::IsMain() const
		{
			return mMain;
		}

		void Thread::SetPriority(Priority priority)
		{
			if (SetThreadPriority(mHandle, ToWin32Type(priority)) == FALSE)
				throw SystemError::GetLastError();
		}

		void Thread::SetAffinity(unsigned int affinityMask)
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

			if (SetThreadAffinityMask(mHandle, affinityMask) == 0)
				throw SystemError::GetLastError();
		}

		void Thread::SetName(StringView name)
		{
			SetThreadDescription(mHandle, name.GetData());
		}

		String Thread::GetName() const
		{
			return String::Empty;
		}

		unsigned int Thread::GetId() const
		{
			return mId;
		}

		void* Thread::GetSystemHandle() const
		{
			return mHandle;
		}
	}
}
