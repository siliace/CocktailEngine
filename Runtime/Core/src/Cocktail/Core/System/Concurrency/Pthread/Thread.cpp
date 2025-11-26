#include <unistd.h>
#include <sys/syscall.h>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Log/LogLevel.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Concurrency/Runnable.hpp>
#include <Cocktail/Core/System/Concurrency/Pthread/PthreadErrorCategory.hpp>
#include <Cocktail/Core/System/Concurrency/Pthread/Thread.hpp>
#include <Cocktail/Core/System/Concurrency/Pthread/ThreadManager.hpp>
#include <Cocktail/Core/System/Unix/UnixServiceProvider.hpp>

namespace Ck
{
    unsigned int Thread::GetCoreCount()
    {
        return sysconf(_SC_NPROCESSORS_ONLN);
    }

    void Thread::SleepFor(const Duration& duration)
    {
        std::size_t sleepTime = std::max(duration.GetCount(TimeUnit::Microseconds()), static_cast<std::size_t>(1));

        // At this point, it's not useful to call successive sleep
        assert(sleepTime < std::numeric_limits<unsigned int>::max());

        usleep(sleepTime);
    }

    void Thread::Yield()
    {
        if (sched_yield() == -1)
            throw SystemError::GetLastError();
    }

    UniquePtr<Thread> Thread::Create(Runnable* runnable, StringView name, unsigned int stackSize)
    {
        return MakeUnique<Detail::Pthread::Thread>(runnable, name, stackSize);
    }

    namespace Detail::Pthread
    {
        namespace
        {
            unsigned int GetProcessAffinityMask()
            {
                const unsigned int coreCount = Thread::GetCoreCount();

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

            /**
             * \brief Converts a Thread::Priority to a value that can be used in pthread_setschedparam
             */
            int TranslateThreadPriority(Thread::Priority priority)
            {
                switch (priority)
                {
                case Thread::Priority::Highest:
                case Thread::Priority::TimeCritical:
                    return 30;

                case Thread::Priority::AboveNormal:
                    return 25;

                case Thread::Priority::Normal:
                    return 15;

                case Thread::Priority::SlightlyBelowNormal:
                    return 14;

                case Thread::Priority::BelowNormal:
                    return 5;

                case Thread::Priority::Lowest:
                    return 1;
                }

                COCKTAIL_UNREACHABLE();
            }
        }

        void* Thread::Run(void* arg)
        {
            Thread* self = static_cast<Thread*>(arg);
            self->mId = syscall(SYS_gettid);

            ThreadManager* threadManager = App::Resolve<ThreadManager>();
            threadManager->Register(self);

            self->mRunnable->Init();
            unsigned int exitCode = self->mRunnable->Run();
            self->mRunnable->Exit();

            pthread_exit(&exitCode);
        }

        Thread::Thread(pthread_t handle, int id, bool isMain) :
            mHandle(handle),
            mId(id),
            mMain(isMain),
            mRunnable(nullptr),
            mJoined(false)
        {
            if (mMain)
                Thread::SetName(CK_TEXT("Main"));
        }

        Thread::Thread(Runnable* runnable, StringView name, unsigned int stackSize) :
            mHandle(0),
            mId(0),
            mMain(false),
            mRunnable(runnable),
            mJoined(false)
        {
            pthread_attr_t attributes;
            pthread_attr_init(&attributes);
            if (stackSize > PTHREAD_STACK_MIN)
            {
                int error = pthread_attr_setstacksize(&attributes, stackSize);
                if (error != 0)
                    throw std::system_error(error, PthreadErrorCategory::Instance);
            }

            int error = pthread_create(&mHandle, &attributes, &Thread::Run, this);
            pthread_attr_destroy(&attributes);
            if (error != 0)
                throw std::system_error(error, PthreadErrorCategory::Instance);

            Thread::SetName(name);
        }

        Thread::~Thread()
        {
            if (!mMain)
            {
                if (!mJoined)
                    Thread::Kill(true);

                ThreadManager* threadManager = App::Resolve<ThreadManager>();
                threadManager->Unregister(this);
            }
        }

        void Thread::Join()
        {
            if (int error = pthread_join(mHandle, nullptr); error != 0)
                throw std::system_error(error, PthreadErrorCategory::Instance);

            mJoined = true;
        }

        bool Thread::Join(const Duration& timeout)
        {
            Instant waitEnd = Instant::Now().After(timeout);

            timespec ts;
            ts.tv_sec = static_cast<unsigned int>(waitEnd.GetSeconds());
            ts.tv_nsec = static_cast<unsigned int>(waitEnd.GetNanoseconds());

            if (int error = pthread_timedjoin_np(mHandle, nullptr, &ts); error != 0)
            {
                if (error == ETIMEDOUT)
                    return false;

                throw std::system_error(error, PthreadErrorCategory::Instance);
            }

            mJoined = true;
            return true;
        }

        void Thread::Kill(bool waitForTermination)
        {
             if (mRunnable)
                 mRunnable->Stop();

             if (waitForTermination)
                 Join();
        }

        bool Thread::IsMain() const
        {
            return mMain;
        }

        void Thread::SetPriority(Priority priority)
        {
            sched_param schedulerParameters;
            schedulerParameters.sched_priority = 0;

            int policy = SCHED_RR;
            pthread_getschedparam(mHandle, &policy, &schedulerParameters);

            schedulerParameters.sched_priority = TranslateThreadPriority(priority);
            pthread_setschedparam(mHandle, policy, &schedulerParameters);
        }

        void Thread::SetAffinity(unsigned affinityMask)
        {
            if (affinityMask == 0)
                throw RuntimeException(CK_TEXT("Cannot assign a null affinity mask to a thread"));

            unsigned int processAffinityMask = GetProcessAffinityMask();
            if ((affinityMask | processAffinityMask) != processAffinityMask)
            {
                CK_LOG(Detail::Unix::UnixLogCategory, LogLevel::Warning, CK_TEXT("Thread affinity mask %d must be a subset of process affinity mask %d"), affinityMask, processAffinityMask);
                affinityMask = processAffinityMask;
            }

            cpu_set_t threadAffinity;
            CPU_ZERO(&threadAffinity);
            for (unsigned int i = 0; i < Thread::GetCoreCount(); i++)
            {
                if (affinityMask & Bit(i))
                    CPU_SET(i, &threadAffinity);
            }

            int error = pthread_setaffinity_np(mHandle, sizeof(cpu_set_t), &threadAffinity);
            if (error != 0)
                throw std::system_error(error, PthreadErrorCategory::Instance);
        }

        void Thread::SetName(StringView name)
        {
            int error = pthread_setname_np(mHandle, AsciiString::Convert(name).GetData());
            if (error != 0)
                throw std::system_error(error, PthreadErrorCategory::Instance);
        }

        String Thread::GetName() const
        {
            static constexpr unsigned int ThreadNameMaxSize = 16;

            AnsiChar buffer[ThreadNameMaxSize];
            int error = pthread_getname_np(mHandle, buffer, ThreadNameMaxSize);
            if (error != 0)
                throw std::system_error(error, PthreadErrorCategory::Instance);

            return String::ConvertFrom<Encoders::Ascii>(buffer);
        }

        unsigned int Thread::GetId() const
        {
            return mId;
        }

        void* Thread::GetSystemHandle() const
        {
            return reinterpret_cast<void*>(mHandle);
        }
    }
}
