#include <CocktailEngine/Core/System/Concurrency/ConditionVariable.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck
{
    static_assert(Detail::ConditionVariableHandleAlignof == alignof(CONDITION_VARIABLE));
    static_assert(Detail::ConditionVariableHandleSizeof == sizeof(CONDITION_VARIABLE));

    ConditionVariable::ConditionVariable() :
        mHandle{}
    {
        InitializeConditionVariable(static_cast<PCONDITION_VARIABLE>(mHandle.GetAddress()));
    }

    ConditionVariable::~ConditionVariable() = default;

    void ConditionVariable::Wait(Mutex& mutex)
    {
        const BOOL result = SleepConditionVariableCS(static_cast<PCONDITION_VARIABLE>(mHandle.GetAddress()), static_cast<PCRITICAL_SECTION>(mutex.GetSystemHandle()), INFINITE);

        if (result == FALSE)
            throw SystemError::GetLastError();
    }

    bool ConditionVariable::WaitFor(Mutex& mutex, const Duration& timeout)
    {
        DWORD timeoutMilliseconds = INFINITE;
        if (!timeout.IsInfinite())
        {
            constexpr Uint64 MaxFiniteTimeout = static_cast<Uint64>(INFINITE) - 1;
            if (timeout >= Duration::Milliseconds(MaxFiniteTimeout))
            {
                timeoutMilliseconds = INFINITE - 1;
            }
            else
            {
                Uint64 milliseconds = timeout.GetCount(TimeUnit::Milliseconds());
                if (Duration::Milliseconds(milliseconds) < timeout)
                    ++milliseconds;

                timeoutMilliseconds = static_cast<DWORD>(milliseconds);
            }
        }

        const BOOL result = SleepConditionVariableCS(static_cast<PCONDITION_VARIABLE>(mHandle.GetAddress()), static_cast<PCRITICAL_SECTION>(mutex.GetSystemHandle()), timeoutMilliseconds);

        if (result == FALSE)
        {
            const DWORD lastError = ::GetLastError();
            if (lastError == ERROR_TIMEOUT)
                return false;

            throw SystemException(lastError, SystemError::GetSystemErrorCategory());
        }

        return true;
    }

    bool ConditionVariable::WaitUntil(Mutex& mutex, const Instant& instant)
    {
        const Instant now = Instant::Now();
        if (!instant.IsAfter(now))
            return false;

        return WaitFor(mutex, Duration::Between(now, instant));
    }

    void ConditionVariable::NotifyOne()
    {
        WakeConditionVariable(static_cast<PCONDITION_VARIABLE>(mHandle.GetAddress()));
    }

    void ConditionVariable::NotifyAll()
    {
        WakeAllConditionVariable(static_cast<PCONDITION_VARIABLE>(mHandle.GetAddress()));
    }
}
