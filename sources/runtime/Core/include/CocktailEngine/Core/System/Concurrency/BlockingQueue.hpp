#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/System/Concurrency/ConditionVariable.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/Utility/Optional.hpp>

namespace Ck
{
    /**
     * \class BlockingQueue
     *
     * \brief Hands elements from producers to consumers, making consumers wait when
     *        there is nothing to hand them
     *
     * Elements are delivered in the order they were pushed, and each one goes to
     * exactly one consumer. Any number of threads may push and take at the same
     * time; with several consumers, which one receives a given element is whichever
     * reaches it first, and nothing orders their progress against each other.
     *
     * What blocks is taking, not pushing. The queue has no capacity bound, so a
     * producer only ever waits for the lock and never for room: it grows for as long
     * as the allocator can serve it, and a consumer that cannot keep up is paid for
     * in memory rather than in back-pressure. Take() waits for an element and nothing
     * else, Take(const Duration&) gives up after a delay, or does not wait at all when
     * handed a zero one, and TakeUntil() waits until a condition of the caller's own
     * says to stop.
     *
     * A queue is open until Close() is called, and closing is final. What closing does
     * is let go of the consumers waiting inside the queue: a closed queue is not one
     * that has been drained, it is one nobody is left waiting in. It is how a consumer
     * loop is brought to an end from another thread, nothing else here having any say
     * over a consumer already waiting.
     *
     * Closing is a shutdown step rather than a state the queue keeps working in. Every
     * method that pushes or takes requires an open queue and asserts as much, so a
     * consumer loop has to be written to stay out of the queue once the close has
     * happened, reading the same stop its TakeUntil() predicate reads. IsEmpty() is
     * the one exception, answering for a closed queue that nothing is left to take.
     *
     * What a consumer gets out of a close is an empty optional, from whichever of
     * Take(), Take(const Duration&) and TakeUntil() it was waiting in. No take raises
     * on a close, and none of them tells a close apart from the other reason it has to
     * come back empty, so a consumer that needs to know reads its own state. The close
     * also wins over an element already queued, which is then left where it is: a
     * closed queue hands nothing out, and what remains in it is destroyed with it.
     *
     * A single Mutex protects the elements and the closed flag, and a single
     * ConditionVariable is what consumers wait on. Spurious wake-ups are absorbed
     * internally by every waiting method, so a method that returns an element really
     * was handed one.
     *
     * \tparam T Type of the queued elements. It must be move constructible, elements
     *         being moved out on the way to the caller; Push(const T&) additionally
     *         requires it to be copy constructible
     *
     * \note Copies and moves of the queue itself are not available: it holds a Mutex
     * and a ConditionVariable, and neither can be copied or moved.
     *
     * \note Elements are held in an Array and taken from its front, so every take
     * moves the elements behind it forward, and its cost grows with the length of the
     * queue. This suits a queue that is normally close to empty, which is what a
     * consumer kept awake by its producers gives, rather than a deep backlog.
     *
     * \warning No thread may be inside the queue when it is destroyed. A consumer
     * waits on the ConditionVariable the queue owns, and destroying it under that
     * thread is undefined. Asking the consumer to stop is not enough: TakeUntil() and
     * Close() get it out of the queue, but it still has to reacquire the lock and
     * return, so its thread must be joined before the queue goes away. For a queue
     * held as a member next to the thread draining it, that means the join cannot be
     * left to the members destroying themselves, declaration order deciding which of
     * the two dies first.
     *
     * \see ConditionVariable
     * \see ThreadPool
     */
    template <typename T>
    class BlockingQueue
    {
    public:

        /**
         * \brief Constructs an empty, open queue
         */
        BlockingQueue() :
            mClosed(false)
        {
            /// Nothing
        }

        /**
         * \brief Closes the queue and destroys whatever was left in it
         *
         * \warning The close performed here does not wait for consumers, and could
         * not: the Mutex and the ConditionVariable about to be destroyed are precisely
         * what a woken consumer needs in order to return. It is a last courtesy for a
         * queue nobody is inside, not a replacement for joining the consumer threads
         * first, which the class documentation requires.
         */
        ~BlockingQueue()
        {
            Close();
        }

        /**
         * \brief Waits for the element at the front of the queue and removes it
         *
         * This method blocks for as long as the queue is empty, and returns as soon
         * as an element is pushed, assuming no other consumer takes it first.
         *
         * \pre The queue is open
         *
         * \return The element that was at the front of the queue, or an empty optional
         *         if the queue was closed while this call was waiting. This overload
         *         has no other way out, so an empty result means a close and nothing
         *         else
         *
         * \warning An element arriving and a close are the only two things that end
         * this wait. A consumer that has to be stoppable by some state of its own must
         * use TakeUntil() instead.
         */
        Optional<T> Take()
        {
            LockGuard<Mutex> lock(mLock);
            assert(!mClosed);
            mNotifier.Wait(mLock, [&] {
                return !mQueue.IsEmpty() || mClosed;
            });

            if (mClosed)
                return Optional<T>::Empty();

            T value = mQueue.PopFirst();
            return Optional<T>::Of(Move(value));
        }

        /**
         * \brief Waits for the element at the front of the queue, giving up after a delay
         *
         * The timeout bounds the wait, not the call: the lock still has to be acquired
         * before the wait begins and again before this method returns.
         *
         * \pre The queue is open
         *
         * \param timeout How long to wait for an element. Duration::Infinite() makes
         *        this overload behave as Take(), and a zero or already elapsed duration
         *        makes it take without waiting, reporting the queue as it is found.
         *        That is the only way here to ask the queue a question rather than wait
         *        on it
         *
         * \return The element that was at the front of the queue, or an empty optional
         *         if the timeout expired with the queue still empty or if the queue was
         *         closed while this call was waiting. The two are not told apart, so a
         *         caller that has to treat them differently reads its own state, or
         *         reaches for TakeUntil()
         *
         * \warning As with Take(), an element and a close are what end this wait early.
         * Nothing else cuts it short, so the caller is otherwise released no sooner
         * than \p timeout.
         */
        Optional<T> Take(const Duration& timeout)
        {
            LockGuard<Mutex> lock(mLock);
            assert(!mClosed);
            bool completed = mNotifier.WaitFor(mLock, timeout, [&] {
                return !mQueue.IsEmpty() || mClosed;
            });

            if (!completed || mClosed)
                return Optional<T>::Empty();

            T value = mQueue.PopFirst();
            return Optional<T>::Of(Move(value));
        }

        /**
         * \brief Waits for the element at the front of the queue, giving up when a
         *        predicate says to
         *
         * This method blocks like Take() does, but with two more ways out: it returns
         * once an element is available, once \p predicate returns true, or once the
         * queue is closed, whichever comes first. The predicate loses to an element, so
         * a call can still come back with a value when the stop it reads is already
         * set. A close does not lose to an element: it gives up one that is sitting in
         * the queue, left there for the queue to destroy.
         *
         * The predicate is evaluated with the queue's lock held, before the first wait
         * and after every wake-up. It is how a consumer loop learns it should stop, so
         * the state it reads normally lives outside the queue, and Close() is what
         * brings the consumer back to look at it.
         *
         * This is the only waiting method that takes a stop condition of the caller's
         * own, which is what makes it the one a consumer loop is built on.
         *
         * \tparam TPredicate Callable returning whether the caller should stop waiting
         *
         * \pre The queue is open. The close ending this wait is one that lands while it
         *      waits, not one that preceded the call
         *
         * \param predicate Condition ending the wait, evaluated under the queue's lock
         *
         * \return The element that was at the front of the queue, or an empty optional
         *         if \p predicate or a close ended the wait. Which of the two it was is
         *         not reported, the caller being expected to recognise its own stop
         *
         * \note State the predicate reads from outside the queue is not protected by
         * the queue's lock and has to be published by other means, an atomic being the
         * simplest. Writing it and then calling Close() is enough for a consumer to see
         * it, in time: the close is decided under the very lock this method holds while
         * it evaluates the predicate, so a notification cannot fall between the
         * predicate being evaluated false and the caller going to sleep.
         *
         * \warning The predicate runs under the queue's lock. It must not block, and it
         * must not take a lock held by a thread that is pushing or closing, which would
         * be the two halves of an ABBA deadlock.
         */
        template <typename TPredicate>
        Optional<T> TakeUntil(TPredicate predicate)
        {
            LockGuard<Mutex> lock(mLock);
            assert(!mClosed);
            mNotifier.Wait(mLock, [&] {
                return !mQueue.IsEmpty() || mClosed || predicate();
            });

            if (mQueue.IsEmpty() || mClosed)
                return Optional<T>::Empty();

            T value = mQueue.PopFirst();
            return Optional<T>::Of(Move(value));
        }

        /**
         * \brief Tells whether anything could still be taken from the queue
         *
         * \return True if the queue was empty or closed, false otherwise
         *
         * \note A closed queue reports itself as empty whatever it still holds, there
         * being no way left to take any of it. This is the one method that may be
         * called on a closed queue, and the answer it gives there says nothing about
         * how many elements went undelivered.
         *
         * \warning The answer describes an instant that has already passed by the time
         * it is read, any other thread being free to push or take in between. It cannot
         * be used to decide that a following Take() will not block: ask for an element
         * with a zero timeout instead, and act on what comes back rather than on what
         * was true a moment ago.
         */
        bool IsEmpty() const
        {
            LockGuard<Mutex> lock(mLock);
            if (mClosed)
                return true;

            return mQueue.IsEmpty();
        }

        /**
         * \brief Copies a value onto the back of the queue and wakes one consumer
         *
         * The queue being unbounded, this waits for the lock and for nothing else. The
         * notification is issued after the lock is released, so the woken consumer does
         * not immediately block on the producer still holding it.
         *
         * \pre The queue is open. A queue is closed to let go of its consumers, so
         *      pushing past that point queues an element for nobody
         *
         * \param value Value to copy into the queue
         */
        void Push(const T& value)
        {
            {
                LockGuard<Mutex> lock(mLock);
                assert(!mClosed);
                mQueue.Add(value);
            }
            mNotifier.NotifyOne();
        }

        /**
         * \brief Moves a value onto the back of the queue and wakes one consumer
         *
         * \pre The queue is open
         *
         * \param value Value to move into the queue, left in its moved-from state
         */
        void Push(T&& value)
        {
            {
                LockGuard<Mutex> lock(mLock);
                assert(!mClosed);
                mQueue.Add(Move(value));
            }
            mNotifier.NotifyOne();
        }

        /**
         * \brief Builds an element at the back of the queue and wakes one consumer
         *
         * The element is constructed in the queue's own storage, sparing the copy or
         * the move that the Push overloads make, which is what move-only elements such
         * as a UniquePtr want.
         *
         * \tparam Args Types of the arguments the element is constructed from
         *
         * \pre The queue is open
         *
         * \param args Arguments forwarded to the constructor of T
         */
        template <typename... Args>
        void Emplace(Args&&... args)
        {
            {
                LockGuard<Mutex> lock(mLock);
                assert(!mClosed);
                mQueue.Emplace(Forward<Args>(args)...);
            }
            mNotifier.NotifyOne();
        }

        /**
         * \brief Closes the queue, releasing every consumer waiting in it
         *
         * This is what a consumer waiting in TakeUntil() needs once the state its
         * predicate reads has changed: the queue cannot see that change, so the thread
         * making it has to ask for the predicate to be evaluated again. Shutting a
         * consumer loop down is the usual reason to call this, and the close is what
         * makes it work for consumers that have not reached their wait yet as well as
         * for those already in it.
         *
         * Every waiting consumer is woken and leaves the queue with an empty optional,
         * whichever take it was waiting in, and none of them raises. Elements still
         * queued are neither handed out nor discarded: the close wins over an element
         * already there, and what is left is destroyed with the queue.
         *
         * Closing is final and repeating it changes nothing. The flag is raised under
         * the queue's lock, which is what makes the close impossible to miss: a
         * consumer holds that same lock from the moment it evaluates its condition
         * until the wait atomically gives it up, so this call cannot slip into that gap
         * and find nobody listening yet. The notification follows outside the lock, so
         * a woken consumer does not immediately block on the thread that closed.
         *
         * \note This is one of the two methods that may be called on a queue other
         * threads are inside of, IsEmpty() being the other. Every other method requires
         * a queue that is open.
         *
         * \warning Waking a consumer is not being rid of it. A woken consumer still has
         * to reacquire this queue's lock and return from the method it was in, so it is
         * normally still inside the queue when this call comes back. Joining the
         * consumer threads is what makes them gone, and remains necessary before the
         * queue is destroyed.
         */
        void Close()
        {
            {
                LockGuard<Mutex> lock(mLock);
                mClosed = true;
            }
            mNotifier.NotifyAll();
        }

    private:

        mutable Mutex mLock;
        ConditionVariable mNotifier;
        Array<T> mQueue;
        bool mClosed;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_BLOCKINGQUEUE_HPP
