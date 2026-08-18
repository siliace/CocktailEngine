#ifndef COCKTAILENGINE_CORE_CONCURRENCY_THREADPOOL_HPP
#define COCKTAILENGINE_CORE_CONCURRENCY_THREADPOOL_HPP

#include <atomic>
#include <concepts>
#include <exception>
#include <tuple>
#include <type_traits>
#include <utility>

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/Exception.hpp>
#include <CocktailEngine/Core/System/Concurrency/BlockingQueue.hpp>
#include <CocktailEngine/Core/System/Concurrency/Future.hpp>
#include <CocktailEngine/Core/System/Concurrency/Runnable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>

namespace Ck
{
    namespace Detail
    {
        class PackedTaskBase
        {
        public:

            virtual ~PackedTaskBase() = default;
            virtual void Execute() = 0;
        };

        class NoOpPackedTask final : public PackedTaskBase
        {
        public:

            void Execute() override
            {
                /// Wakes an idle worker during shutdown.
            }
        };

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

        template <typename TResult>
        class TypedPackedTask : public PackedTaskBase
        {
        public:

            using ResultType = TResult;

            virtual Future<ResultType> GetFuture() const = 0;
        };

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

        class PackedTaskRunner final : public Runnable
        {
        public:

            void Enqueue(UniquePtr<PackedTaskBase> task)
            {
                mTaskQueue.Emplace(Move(task));
            }

            unsigned int Run() override
            {
                while (!mStopThread)
                {
                    UniquePtr<PackedTaskBase> task = mTaskQueue.TryTake([this]() -> bool {
                        return mStopThread || mRequestSynchronization;
                    }).GetOr(nullptr);

                    if (task)
                    {
                        ++mTasksInFlight;
                        task->Execute();
                        --mTasksInFlight;
                    }

                    if (mRequestSynchronization && mTasksInFlight == 0 && mTaskQueue.IsEmpty())
                    {
                        // mSynchronizationEvent->Trigger();
                        mRequestSynchronization = false;
                    }
                }

                return 0;
            }

            void Flush()
            {
                if (mTaskQueue.IsEmpty() && mTasksInFlight == 0)
                    return;

                mRequestSynchronization = true;
                mTaskQueue.NotifyAll();
                // mSynchronizationEvent->Wait();
            }

            void Stop() override
            {
                mStopThread = true;
                mTaskQueue.NotifyAll();
            }

        private:

            std::atomic<bool> mStopThread = false;
            std::atomic<unsigned int> mTasksInFlight = 0;
            std::atomic<bool> mRequestSynchronization = false;
            BlockingQueue<UniquePtr<PackedTaskBase>> mTaskQueue;
            // UniquePtr<Event> mSynchronizationEvent = nullptr;
        };
    }

    /**
     * \class ThreadPool
     *
     * \brief Executes independent tasks on a fixed set of worker threads.
     *
     * Workers start during construction and share one FIFO task queue. A task is
     * removed from that queue once by one worker; with several workers, neither
     * the worker selected nor completion order is guaranteed. Calls to Execute()
     * and Submit() may run concurrently with each other, but neither may race with
     * destruction of the pool.
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
         * \brief Block the calling thread until the completion of every submitted task.
         */
        void Flush();

    private:

        Array<UniquePtr<Thread>> mThreads;
        UniquePtr<Detail::PackedTaskRunner> mRunner;
    };
}

#endif // COCKTAILENGINE_CORE_CONCURRENCY_THREADPOOL_HPP
