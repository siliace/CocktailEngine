#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>
#include <CocktailEngine/Core/System/Concurrency/ThreadManager.hpp>

namespace Ck
{
    void Thread::SleepUntil(const Instant& instant, const Duration& duration)
    {
        // Assert the instant is not before now, otherwise we would wait forever
        assert(Instant::Now().IsBefore(instant));

        WaitUntil(
            [&]() {
                return Instant::Now().IsBefore(instant);
            },
            duration);
    }

    UniquePtr<Thread> Thread::Create(Runnable* runnable, StringView name, unsigned int stackSize)
    {
        return ResolveFacadeInstance()->Create(runnable, name, stackSize);
    }

    Thread* Thread::GetCurrentThread()
    {
        return ResolveFacadeInstance()->GetCurrentThread();
    }
}
