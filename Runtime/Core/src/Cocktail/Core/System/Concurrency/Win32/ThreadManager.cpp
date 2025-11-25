#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Concurrency/Win32/ThreadManager.hpp>

namespace Ck::Detail::Win32
{
	ThreadManager::ThreadManager()
	{
		mTlsIndex = TlsAlloc();
		if (mTlsIndex == 0)
		{
			DWORD error = ::GetLastError();
			if (error != NO_ERROR)
				throw std::system_error(static_cast<int>(error), SystemError::GetSystemErrorCategory());
		}

		mMainThread = MakeUnique<Thread>(::GetCurrentThread(), ::GetCurrentThreadId(), true);
		TlsSetValue(mTlsIndex, mMainThread.Get());

		std::lock_guard<std::mutex> lg(mThreadMutex);
		mThreads.Add(mMainThread.Get());
	}

	ThreadManager::~ThreadManager()
	{
		TlsFree(mTlsIndex);
	}

	void ThreadManager::Register(Thread* thread)
	{
		assert(thread->GetId() == ::GetCurrentThreadId());

		TlsSetValue(mTlsIndex, thread);

		std::lock_guard<std::mutex> lg(mThreadMutex);
		mThreads.Add(thread);
	}

	void ThreadManager::Unregister(Thread* thread)
	{
		mThreads.Remove(thread);
		TlsSetValue(mTlsIndex, nullptr);
	}

	Thread* ThreadManager::GetCurrentThread()
	{
		return static_cast<Thread*>(TlsGetValue(mTlsIndex));
	}
}
