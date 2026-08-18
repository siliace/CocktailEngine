#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_WIN32_THREADMANAGER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_WIN32_THREADMANAGER_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/System/Concurrency/ThreadManager.hpp>
#include <CocktailEngine/Core/System/Concurrency/Win32/Thread.hpp>

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

		UniquePtr<Thread> mMainThread;
		Array<Thread*> mThreads;
		std::mutex mThreadMutex;
		DWORD mTlsIndex;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_WIN32_THREADMANAGER_HPP
