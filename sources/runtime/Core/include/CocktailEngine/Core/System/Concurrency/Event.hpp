#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_EVENT_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_EVENT_HPP

#include <CocktailEngine/Core/Export.hpp>
#include <CocktailEngine/Core/Utility/StorageFor.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

namespace Ck
{
    namespace Detail
    {
#ifdef COCKTAIL_OS_WINDOWS
        static constexpr std::size_t EventHandleAlignof = 8;
        static constexpr std::size_t EventHandleSizeof = 8;
#else
        static constexpr std::size_t EventHandleAlignof = 8;
        static constexpr std::size_t EventHandleSizeof = 96;
#endif
    }

    /**
     * \brief Signalling flag threads can wait on
     *
     * Unlike a ConditionVariable, an event carries its own state and needs no Mutex:
     * it is either signaled or not, and a signal raised while nobody waits is kept
     * until someone does. A thread calling a waiting method therefore never misses a
     * signal that preceded it, which is what makes an event the primitive to reach for
     * when the waiter may arrive after the thing it waits for already happened.
     *
     * What becomes of that signal once a waiter has consumed it is the reset mode,
     * fixed at construction and never changed afterwards.
     *
     * \note An event says that something happened, not what: it holds one bit and no
     * payload. Protecting state the waiter then reads remains a Mutex's job, and
     * waiting for state rather than for an occurrence is a ConditionVariable's.
     *
     * \see ConditionVariable
     */
    class COCKTAILENGINE_CORE_API Event
    {
    public:

        /**
         * \brief Tells what a waiter leaves behind once it has been released
         */
        enum class ResetMode
        {
            /**
             * \brief One waiter is released per signal, which it consumes
             *
             * The event goes back to un-signaled as the waiter returns, so a signal is
             * a permission handed to exactly one thread. This is the mode for a
             * hand-off between a producer and a single consumer.
             */
            Auto,

            /**
             * \brief Every waiter is released, and the event stays signaled
             *
             * The signal remains until Reset is called, so threads arriving later
             * return immediately as well. This is the mode for announcing something
             * that stays true, such as a resource having become ready.
             */
            Manual
        };

        /**
         * \brief Creates an event in the requested reset mode
         *
         * \param resetMode What a released waiter leaves the event in
         * \param initiallySignaled Whether the event starts out signaled, which
         *        releases the first waiter without anything having to signal it
         */
        explicit Event(ResetMode resetMode, bool initiallySignaled = false);

        /**
         * \brief Events are not copyable: they are a rendezvous point, and a copy of
         *        one would be a different rendezvous that no waiter is watching
         */
        Event(const Event& other) = delete;

        /**
         * \brief Events are not movable: waiters block on the address of the one they
         *        were given, which moving would leave behind
         */
        Event(Event&& other) noexcept = delete;

        /**
         * \brief Destroys the event
         *
         * \warning No thread may be waiting on the event when it is destroyed.
         */
        ~Event();

        /**
         * \brief Events are not copy assignable, for the reason they are not copyable
         *
         * \return Nothing, the operator being deleted
         */
        Event& operator=(const Event& other) = delete;

        /**
         * \brief Events are not move assignable, for the reason they are not movable
         *
         * \return Nothing, the operator being deleted
         */
        Event& operator=(Event&& other) noexcept = delete;

        /**
         * \brief Signals the event, releasing waiters according to the reset mode
         *
         * Auto releases a single waiter, Manual releases all of them and leaves the
         * event signaled. Signaling an event that already is has no further effect:
         * the state is one bit, so two signals nobody consumed in between release one
         * waiter, not two.
         */
        void Set();

        /**
         * \brief Returns the event to its un-signaled state
         *
         * This is how a Manual event is re-armed. It does not reclaim a signal a
         * waiter has already been released by, and calling it on an event that is
         * not signaled does nothing.
         */
        void Reset();

        /**
         * \brief Blocks until the event is signaled
         *
         * Returns immediately when the event already is. On an Auto event, returning
         * consumes the signal.
         */
        void Wait();

        /**
         * \brief Blocks until the event is signaled or an absolute deadline is reached
         *
         * The state is read before the deadline is, so a deadline already in the past
         * turns this into a non-blocking poll rather than an automatic failure.
         *
         * \param instant Deadline past which the wait is given up
         *
         * \return True if the event was signaled, which consumes the signal on an Auto
         *         event, false if the deadline was reached without one.
         *
         * \warning An Instant is read from the wall clock, which can be moved while a
         * thread waits on it. Prefer WaitFor when an interval is what matters.
         */
        bool WaitUntil(const Instant& instant);

        /**
         * \brief Blocks until the event is signaled or a timeout expires
         *
         * \param timeout How long to wait at most. Duration::Infinite() waits
         *        indefinitely, as Wait does
         *
         * \return True if the event was signaled, false if the timeout expired first
         */
        bool WaitFor(const Duration& timeout);

        /**
         * \brief Returns the reset mode the event was created with
         *
         * \return What a released waiter leaves this event in
         */
        ResetMode GetResetMode() const;

        /**
         * \brief Returns the native object backing this event
         *
         * \return A pointer to the platform handle, a HANDLE on Windows and the
         *         mutex-and-condition pair the POSIX back-end builds the event from
         *         elsewhere.
         *
         * \warning This exists for the platform back-ends of this module and for code
         * that has to hand the event to a native API. Nothing about what it points to
         * is part of this class's contract, and driving the event through it bypasses
         * the reset mode this class maintains.
         */
        void* GetSystemHandle() const
        {
            return const_cast<Event*>(this)->mHandle.GetAddress();
        }

    private:

        ResetMode mResetMode;
        StorageForSize<Detail::EventHandleAlignof, Detail::EventHandleSizeof> mHandle;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_EVENT_HPP
