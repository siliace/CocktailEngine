#ifndef COCKTAILENGINE_CORE_CONCURRENCY_THREADPOOL_HPP
#define COCKTAILENGINE_CORE_CONCURRENCY_THREADPOOL_HPP

#include <atomic>
#include <concepts>
#include <exception>
#include <tuple>
#include <type_traits>

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/System/Concurrency/BlockingQueue.hpp>
#include <CocktailEngine/Core/System/Concurrency/ConditionVariable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Future.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/Runnable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>

namespace Ck
{
    namespace Detail
    {
        /**
         * \brief Type-erased unit of work a ThreadPool worker can run
         */
        class PackedTaskBase
        {
        public:

            virtual ~PackedTaskBase() = default;

            /**
             * \brief Runs the work this task holds, exactly once
             *
             * \warning Implementations must not let an exception escape: a task that
             * throws leaves the pool unable to account for it, and the worker running
             * it has nowhere to report the failure.
             */
            virtual void Execute() = 0;
        };

        /**
         * \brief Task that carries no work, only the wake-up that comes with being queued
         *
         * \note Currently unreferenced: a worker is woken out of the queue by
         * BlockingQueue::Close rather than by being handed a task to discard.
         */
        class NoOpPackedTask final : public PackedTaskBase
        {
        public:

            void Execute() override
            {
                /// Nothing
            }
        };

        /**
         * \brief Fire-and-forget task, owning the callable and the arguments it was given
         *
         * \tparam TCallable Type of the callable to run
         * \tparam TArgs Types of the arguments it is applied to
         */
        template <typename TCallable, typename... TArgs>
        class ExecutePackedTask final : public PackedTaskBase
        {
        public:

            ExecutePackedTask(TCallable&& callable, std::tuple<TArgs...>&& arguments) :
                mCallable(Forward<TCallable>(callable)),
                mArguments(Move(arguments))
            {
                /// Nothing
            }

            void Execute() override
            {
                try
                {
                    std::apply(Move(mCallable), Move(mArguments));
                }
                catch (...)
                {
                    /// Fire-and-forget tasks have no observer for their failure.
                }
            }

        private:

            TCallable mCallable;
            std::tuple<TArgs...> mArguments;
        };

        /**
         * \brief Task whose outcome can be observed before it has run
         *
         * \tparam TResult Type the task produces
         */
        template <typename TResult>
        class TypedPackedTask : public PackedTaskBase
        {
        public:

            using ResultType = TResult;

            /**
             * \brief Returns a handle on the result this task will produce
             *
             * \return A future satisfied when the task runs, or holding the exception
             *         it raised.
             */
            virtual Future<ResultType> GetFuture() const = 0;
        };

        /**
         * \brief Submitted task, owning the callable, its arguments and its promise
         *
         * \tparam TCallable Type of the callable to run
         * \tparam TArgs Types of the arguments it is applied to
         */
        template <typename TCallable, typename... TArgs>
        class SubmittedPackedTask final : public TypedPackedTask<std::invoke_result_t<TCallable, TArgs...>>
        {
        public:

            using ResultType = std::invoke_result_t<TCallable, TArgs...>;

            SubmittedPackedTask(TCallable&& callable, std::tuple<TArgs...>&& arguments) :
                mCallable(Forward<TCallable>(callable)),
                mArguments(Move(arguments))
            {
                /// Nothing
            }

            void Execute() override
            {
                try
                {
                    mPromise.SetValue(std::apply(Move(mCallable), Move(mArguments)));
                }
                catch (...)
                {
                    mPromise.SetException(std::current_exception());
                }
            }

            Future<ResultType> GetFuture() const override
            {
                return mPromise.ToFuture();
            }

        private:

            TCallable mCallable;
            std::tuple<TArgs...> mArguments;
            Promise<ResultType> mPromise;
        };

        /**
         * \brief Body shared by every worker of a ThreadPool
         *
         * One runner is shared by all the workers of a pool: they take from the same
         * queue, so which worker runs a given task is whichever reaches it first.
         *
         * Alongside the queue, the runner counts the tasks that are pending, meaning
         * enqueued but not finished running. The count is raised before a task is made
         * visible to a worker and lowered only once that task has returned, so a task
         * is never momentarily invisible to Flush while it travels from the queue to a
         * worker. Reaching zero is the state Flush waits for.
         */
        class PackedTaskRunner final : public StoppableRunnable
        {
        public:

            /**
             * \brief Hands a task to the workers
             *
             * \param task Task to run, counted as pending until it has finished
             */
            void Enqueue(UniquePtr<PackedTaskBase> task)
            {
                /// Counted before the task is pushed: once it is in the queue a worker
                /// may already be running it, and a count raised afterwards would leave
                /// a window where a concurrent Flush sees nothing pending.
                ++mPendingTasks;
                mTaskQueue.Emplace(Move(task));
            }

            /**
             * \brief Takes and runs tasks until the runner is stopped
             *
             * \return Zero, the runner having no outcome of its own to report
             */
            unsigned int Run() override
            {
                while (!ShouldStop())
                {
                    UniquePtr<PackedTaskBase> task = mTaskQueue.TakeUntil([this]() -> bool {
                        return ShouldStop();
                    }).GetOr(nullptr);

                    if (!task)
                        continue;

                    /// The count has to come back down even if the task breaks its
                    /// contract and throws, or a Flush waiting on it never returns.
                    try
                    {
                        task->Execute();
                    }
                    catch (...)
                    {
                        ReleasePendingTask();
                        throw;
                    }

                    ReleasePendingTask();
                }

                return 0;
            }

            /**
             * \brief Blocks until no task is pending
             *
             * Safe to call from several threads at once: every caller waits on the same
             * state and all of them are released together.
             */
            void Flush()
            {
                LockGuard<Mutex> lock(mPendingLock);
                mPendingNotifier.Wait(mPendingLock, [this]() -> bool {
                    return mPendingTasks == 0;
                });
            }

            /**
             * \brief Asks every worker to return from Run once its current task is done
             */
            void Stop() override
            {
                StoppableRunnable::Stop();
                mTaskQueue.Close();
            }

        private:

            /**
             * \brief Accounts for a finished task and wakes the waiters it was holding
             */
            void ReleasePendingTask()
            {
                /// Exactly one thread observes each transition to zero, which is the only
                /// one a waiter cares about.
                if (--mPendingTasks != 0)
                    return;

                /// The lock is what makes the notification impossible to miss: a thread
                /// that found the count non-zero holds it until it is atomically released
                /// by the wait, so this cannot slip between its test and its wait.
                LockGuard<Mutex> lock(mPendingLock);
                mPendingNotifier.NotifyAll();
            }

            std::atomic<unsigned int> mPendingTasks = 0;
            Mutex mPendingLock;
            ConditionVariable mPendingNotifier;
            BlockingQueue<UniquePtr<PackedTaskBase>> mTaskQueue;
        };
    }

    /**
     * \class ThreadPool
     *
     * \brief Executes independent tasks on a fixed set of worker threads.
     *
     * Workers start during construction and share one FIFO task queue. A task is
     * removed from that queue once by one worker; with several workers, neither
     * the worker selected nor completion order is guaranteed. Calls to Execute(),
     * Submit() and Flush() may run concurrently with each other, but none may race
     * with destruction of the pool.
     *
     * The pool owns decayed copies of every callable and argument. References are
     * therefore not retained: use std::ref explicitly when a task must operate on
     * an object owned by its caller. A task and its arguments are consumed when it
     * runs, so move-only callables and arguments are supported.
     *
     * Destruction requests that workers stop, waits for tasks already executing to
     * finish, then joins every worker. Work still waiting in the queue is not
     * guaranteed to run; a Future for discarded submitted work reports
     * BrokenPromiseException. A pool must not be destroyed by one of its own tasks.
     */
    class COCKTAILENGINE_CORE_API ThreadPool
    {
    public:

        /**
         * \brief Creates and starts a fixed number of worker threads
         *
         * When \p poolName is non-empty, worker names use the
         * `poolName-thread-index` form. Normal priority leaves the native default
         * unchanged; other priorities are applied after each worker starts and may
         * fail when the platform or current process forbids that priority.
         *
         * \param threadCount Number of workers to create; it must be greater than zero
         * \param poolName Optional prefix used to name workers for diagnostics
         * \param priority Requested priority for every worker
         * \param stackSize Stack size in bytes, or zero for the platform default
         */
        ThreadPool(unsigned int threadCount, StringView poolName = StringView::Empty, Thread::Priority priority = Thread::Priority::Normal, unsigned int stackSize = 0);

        /**
         * \brief Stops the pool, joins its workers, and discards work not yet started
         *
         * This operation blocks until currently executing tasks return. It must not
         * be called from a task executing in this pool.
         */
        ~ThreadPool();

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;

        /**
         * \brief Schedules fire-and-forget work
         *
         * The callable and arguments are stored by value and invoked exactly once
         * unless destruction discards the task before it starts. An exception from
         * the callable is caught so it cannot terminate a worker, but is otherwise
         * discarded because this overload returns no error channel.
         *
         * \tparam TCallable Type of the callable to execute
         * \tparam TArgs Types of its arguments
         *
         * \param callable Callable to schedule
         * \param args Arguments supplied to \p callable when the task runs
         */
        template <typename TCallable, typename... TArgs>
            requires(std::move_constructible<std::decay_t<TCallable>> && (std::move_constructible<std::decay_t<TArgs>> && ...) &&
                     std::invocable<std::decay_t<TCallable>, std::decay_t<TArgs>...>)
        void Execute(TCallable&& callable, TArgs&&... args)
        {
            using Task = Detail::ExecutePackedTask<std::decay_t<TCallable>, std::decay_t<TArgs>...>;
            UniquePtr<Task> task = MakeUnique<Task>(Forward<TCallable>(callable), std::tuple<std::decay_t<TArgs>...>(Forward<TArgs>(args)...));

            mRunner->Enqueue(Move(task));
        }

        /**
         * \brief Schedules work and returns a handle for its result
         *
         * The callable and arguments are stored by value and invoked exactly once
         * unless destruction discards the task before it starts. A callable exception
         * is captured and rethrown when Future::GetValue() is called. Result values
         * must be non-void objects that Future can copy for each observer.
         *
         * \tparam TCallable Type of the callable to execute
         * \tparam TArgs Types of its arguments
         *
         * \param callable Callable to schedule
         * \param args Arguments supplied to \p callable when the task runs
         *
         * \return A future for the callable's result.
         */
        template <typename TCallable, typename... TArgs>
            requires(std::move_constructible<std::decay_t<TCallable>> && (std::move_constructible<std::decay_t<TArgs>> && ...) &&
                     std::invocable<std::decay_t<TCallable>, std::decay_t<TArgs>...> && std::is_object_v<std::invoke_result_t<std::decay_t<TCallable>, std::decay_t<TArgs>...>> &&
                     std::copy_constructible<std::invoke_result_t<std::decay_t<TCallable>, std::decay_t<TArgs>...>>)
        Future<std::invoke_result_t<std::decay_t<TCallable>, std::decay_t<TArgs>...>> Submit(TCallable&& callable, TArgs&&... args)
        {
            using Task = Detail::SubmittedPackedTask<std::decay_t<TCallable>, std::decay_t<TArgs>...>;
            UniquePtr<Task> task = MakeUnique<Task>(Forward<TCallable>(callable), std::tuple<std::decay_t<TArgs>...>(Forward<TArgs>(args)...));

            Future<typename Task::ResultType> future = task->GetFuture();
            mRunner->Enqueue(Move(task));

            return future;
        }

        /**
         * \brief Blocks until every task handed to the pool has finished running
         *
         * Returning means that no task remains queued and that no worker is still
         * inside one, so every effect of the work scheduled before this call is
         * visible to the calling thread. The pool is not stopped and not drained of
         * its workers: it accepts work again, and the call may be repeated.
         *
         * Work scheduled by another thread while this call is in flight may or may not
         * be waited for, since whether it was counted yet decides it. What is never
         * affected is the work scheduled before the call: that is always complete on
         * return.
         *
         * Several threads may flush at once; they are all released together once the
         * pool runs dry.
         *
         * \throw LogicException When called from one of this pool's own workers, which
         *        would have the caller wait for the very task it is running. The check
         *        needs the calling thread to be one the engine knows about, so a thread
         *        started outside of Thread::Create deadlocks instead of being told.
         *
         * \warning It must not run concurrently with destruction of the pool, which
         * discards queued work without running it and leaves the waiter blocked.
         */
        void Flush();

    private:

        UniquePtr<Detail::PackedTaskRunner> mRunner;
        Array<UniquePtr<Thread>> mThreads;
    };
}

#endif // COCKTAILENGINE_CORE_CONCURRENCY_THREADPOOL_HPP
