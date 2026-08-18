#include <CocktailEngine/Core/System/Concurrency/Event.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>
#include <CocktailEngine/Core/Utility/Time/Instant.hpp>

namespace Ck
{
    /// The handle is reached through opaque storage of a size the header has to state
    /// without seeing the type, so the two have to be checked against each other.
    static_assert(Detail::EventHandleAlignof == alignof(HANDLE));
    static_assert(Detail::EventHandleSizeof == sizeof(HANDLE));

    namespace
    {
        HANDLE GetEventHandle(const Event& event)
        {
            return *static_cast<HANDLE*>(event.GetSystemHandle());
        }
    }

    Event::Event(ResetMode resetMode, bool initiallySignaled) :
        mResetMode(resetMode),
        mHandle{}
    {
        /// Win32 event objects already carry the retained signal and the two reset
        /// modes this class exposes, so the handle is the whole implementation here.
        const HANDLE handle = CreateEvent(nullptr, static_cast<BOOL>(mResetMode == ResetMode::Manual), static_cast<BOOL>(initiallySignaled), nullptr);

        /// CreateEvent reports failure with a null handle, INVALID_HANDLE_VALUE being
        /// what the file APIs return and not this one.
        if (handle == nullptr)
            throw SystemError::GetLastError();

        *static_cast<HANDLE*>(mHandle.GetAddress()) = handle;
    }

    Event::~Event()
    {
        CloseHandle(GetEventHandle(*this));
    }

    void Event::Set()
    {
        if (SetEvent(GetEventHandle(*this)) == FALSE)
            throw SystemError::GetLastError();
    }

    void Event::Reset()
    {
        if (ResetEvent(GetEventHandle(*this)) == FALSE)
            throw SystemError::GetLastError();
    }

    void Event::Wait()
    {
        WaitFor(Duration::Infinite());
    }

    bool Event::WaitUntil(const Instant& instant)
    {
        const Instant now = Instant::Now();

        /// A deadline in the past polls the retained signal rather than failing
        /// outright, which is what the predicate overloads of ConditionVariable do and
        /// what the POSIX back-end of this class does.
        if (!instant.IsAfter(now))
            return WaitFor(Duration());

        return WaitFor(Duration::Between(now, instant));
    }

    bool Event::WaitFor(const Duration& timeout)
    {
        DWORD timeoutMilliseconds = INFINITE;
        if (!timeout.IsInfinite())
        {
            /// INFINITE is a timeout value and not a duration, so a finite wait long
            /// enough to collide with it is capped just below instead.
            constexpr Uint64 MaxFiniteTimeout = static_cast<Uint64>(INFINITE) - 1;
            if (timeout >= Duration::Milliseconds(MaxFiniteTimeout))
            {
                timeoutMilliseconds = INFINITE - 1;
            }
            else
            {
                /// Rounded up, so that a sub-millisecond timeout waits rather than
                /// returning before it has waited at all.
                Uint64 milliseconds = timeout.GetCount(TimeUnit::Milliseconds());
                if (Duration::Milliseconds(milliseconds) < timeout)
                    ++milliseconds;

                timeoutMilliseconds = static_cast<DWORD>(milliseconds);
            }
        }

        const DWORD result = WaitForSingleObject(GetEventHandle(*this), timeoutMilliseconds);
        if (result == WAIT_OBJECT_0)
            return true;

        /// WAIT_FAILED is not a timeout: reporting it as one would turn an invalid
        /// handle into a wait that silently never succeeds.
        if (result == WAIT_TIMEOUT)
            return false;

        throw SystemError::GetLastError();
    }

    Event::ResetMode Event::GetResetMode() const
    {
        return mResetMode;
    }
}
