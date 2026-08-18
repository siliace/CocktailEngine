#include <cerrno>
#include <sched.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/syscall.h>

#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/Application/App.hpp>
#include <CocktailEngine/Core/Log/Log.hpp>
#include <CocktailEngine/Core/Log/LogLevel.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/System/Concurrency/Runnable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Pthread/PthreadErrorCategory.hpp>
#include <CocktailEngine/Core/System/Concurrency/Pthread/Thread.hpp>
#include <CocktailEngine/Core/System/Concurrency/Pthread/ThreadManager.hpp>
#include <CocktailEngine/Core/System/Unix/UnixServiceProvider.hpp>

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
             * \brief Tell whether a scheduling policy is a real-time one
             *
             * Real-time policies are driven by \c sched_priority, time-sharing ones are not
             */
            bool IsRealtimePolicy(int policy)
            {
                return policy == SCHED_FIFO || policy == SCHED_RR;
            }

            /**
             * \brief Converts a Thread::Priority to a nice value, for time-sharing policies
             *
             * Under a time-sharing policy such as \c SCHED_OTHER, POSIX mandates a
             * \c sched_priority of 0: the relative weight of a thread is carried by its nice
             * value instead, which ranges from -20 (most favourable) to 19 (least favourable)
             */
            int TranslateThreadPriorityToNiceValue(Thread::Priority priority)
            {
                switch (priority)
                {
                case Thread::Priority::TimeCritical:
                    return -20;

                case Thread::Priority::Highest:
                    return -10;

                case Thread::Priority::AboveNormal:
                    return -5;

                case Thread::Priority::Normal:
                    return 0;

                case Thread::Priority::SlightlyBelowNormal:
                    return 1;

                case Thread::Priority::BelowNormal:
                    return 5;

                case Thread::Priority::Lowest:
                    return 19;
                }

                COCKTAIL_UNREACHABLE();
            }

            /**
             * \brief Places a priority level inside the [\p minimum, \p maximum] range
             *
             * \param minimum Lowest value the range allows
             * \param maximum Highest value the range allows
             * \param percent Position of the level inside the range, in percent
             */
            int InterpolatePriority(int minimum, int maximum, int percent)
            {
                return minimum + ((maximum - minimum) * percent) / 100;
            }

            /**
             * \brief Converts a Thread::Priority to a value usable in pthread_setschedparam
             *
             * The value is interpolated inside the range the kernel reports for \p policy
             * instead of being hard-coded, because that range is policy and platform dependent
             *
             * \param priority Priority level to translate
             * \param policy Real-time scheduling policy the value is destined to
             */
            int TranslateThreadPriority(Thread::Priority priority, int policy)
            {
                const int minimum = sched_get_priority_min(policy);
                const int maximum = sched_get_priority_max(policy);
                if (minimum == -1 || maximum == -1)
                    throw SystemError::GetLastError();

                switch (priority)
                {
                case Thread::Priority::TimeCritical:
                    return InterpolatePriority(minimum, maximum, 100);

                case Thread::Priority::Highest:
                    return InterpolatePriority(minimum, maximum, 90);

                case Thread::Priority::AboveNormal:
                    return InterpolatePriority(minimum, maximum, 75);

                case Thread::Priority::Normal:
                    return InterpolatePriority(minimum, maximum, 50);

                case Thread::Priority::SlightlyBelowNormal:
                    return InterpolatePriority(minimum, maximum, 45);

                case Thread::Priority::BelowNormal:
                    return InterpolatePriority(minimum, maximum, 25);

                case Thread::Priority::Lowest:
                    return InterpolatePriority(minimum, maximum, 0);
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
            sched_param schedulerParameters {};
            int policy = SCHED_OTHER;

            if (int error = pthread_getschedparam(mHandle, &policy, &schedulerParameters); error != 0)
                throw std::system_error(error, PthreadErrorCategory::Instance);

            if (IsRealtimePolicy(policy))
            {
                schedulerParameters.sched_priority = TranslateThreadPriority(priority, policy);

                if (int error = pthread_setschedparam(mHandle, policy, &schedulerParameters); error != 0)
                {
                    // Raising the priority of a real-time thread requires CAP_SYS_NICE, which an
                    // unprivileged process does not hold: report it without failing the caller
                    if (error == EPERM)
                    {
                        CK_LOG(Detail::Unix::UnixLogCategory, LogLevel::Warning, CK_TEXT("Not enough privileges to change the priority of thread %u, priority left untouched"), mId);
                        return;
                    }

                    throw std::system_error(error, PthreadErrorCategory::Instance);
                }
            }
            else
            {
                // Under a time-sharing policy the priority is carried by the nice value. On Linux,
                // setpriority() applies to a single thread when handed a kernel thread id, which the
                // thread publishes on entry, hence the guard below
                if (mId == 0)
                    throw RuntimeException(CK_TEXT("Cannot change the priority of a thread that did not start yet"));

                if (setpriority(PRIO_PROCESS, static_cast<id_t>(mId), TranslateThreadPriorityToNiceValue(priority)) != 0)
                {
                    // Lowering the nice value is reserved to privileged processes, and the RLIMIT_NICE
                    // soft limit caps how far an unprivileged one may go
                    if (errno == EPERM || errno == EACCES)
                    {
                        CK_LOG(Detail::Unix::UnixLogCategory, LogLevel::Warning, CK_TEXT("Not enough privileges to change the priority of thread %u, priority left untouched"), mId);
                        return;
                    }

                    throw SystemError::GetLastError();
                }
            }
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
            if (!name.IsEmpty())
            {
                StringView truncatedName = name.SubStringView(0, 15); /// Pthread does not support thread names with more than 15 chars
                int error = pthread_setname_np(mHandle, AsciiString::Convert(truncatedName).GetData());
                if (error != 0 && error != ESRCH)
                    throw std::system_error(error, PthreadErrorCategory::Instance);
            }
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
