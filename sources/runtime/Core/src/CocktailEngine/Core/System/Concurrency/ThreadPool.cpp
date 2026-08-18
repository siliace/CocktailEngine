#include <CocktailEngine/Core/Memory/Ptr.hpp>
#include <CocktailEngine/Core/System/Concurrency/ThreadPool.hpp>

namespace Ck
{
    ThreadPool::ThreadPool(unsigned int threadCount, StringView poolName, Thread::Priority priority, unsigned int stackSize)
    {
        assert(threadCount > 0);

        mRunner = MakeUnique<Detail::PackedTaskRunner>();
        mThreads.Reserve(threadCount);

        for (unsigned int i = 0; i < threadCount; ++i)
        {
            String name;
            if (!poolName.IsEmpty())
                name = String::Format(CK_TEXT("%s-thread-%u"), poolName, i);

            UniquePtr<Thread> thread = Thread::Create(mRunner.Get(), name, stackSize);
            if (priority != Thread::Priority::Normal)
                thread->SetPriority(priority);

            mThreads.Emplace(Move(thread));
        }
    }

    ThreadPool::~ThreadPool()
    {
        mRunner->Stop();
        mThreads.ForEach([](Ptr<Thread> thread) {
            thread->Join();
        });
    }

    void ThreadPool::Flush()
    {
        if (Thread* currentThread = Thread::GetCurrentThread())
        {
            for (Ptr<Thread> worker : mThreads)
            {
                if (worker.Get() == currentThread)
                    throw LogicException(CK_TEXT("Cannot wait for a ThreadPool to drain from one of its own worker threads"));
            }
        }

        mRunner->Flush();
    }
}
