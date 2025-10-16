#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_WIN32_THREADMANAGER_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_WIN32_THREADMANAGER_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/System/Concurrency/ThreadManager.hpp>
#include <Cocktail/Core/System/Concurrency/Win32/Thread.hpp>

namespace Ck::Detail::Win32
{
	class ThreadManager : public Ck::ThreadManager
	{
	public:

		ThreadManager();
		~ThreadManager();
		void Register(Thread* thread);
		void Unregister(Thread* thread);
		Thread* GetCurrentThread() override;

	private:

		std::unique_ptr<Thread> mMainThread;
		Array<Thread*> mThreads;
		std::mutex mThreadMutex;
		DWORD mTlsIndex;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_WIN32_THREADMANAGER_HPP
