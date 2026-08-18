#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_RUNNABLE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_RUNNABLE_HPP

#include <concepts>
#include <functional>
#include <tuple>
#include <type_traits>

#include <CocktailEngine/Core/Memory/UniquePtr.hpp>

namespace Ck
{
	/**
	 * \brief Represents a runnable task that can be executed by a thread
	 *
	 * Implement this interface to define work that will run in a separate thread
	 *
	 * The task thread calls Init(), then Run(), then Exit(), in that order and once each
	 * Stop() stands apart: it is called from another thread, by Thread::Kill, to ask the task
	 * to leave Run() early, and it may be called at any point or not at all
	 *
	 * \remark A runnable has to outlive the thread running it, since Thread::Create only takes
	 *         a pointer to it and never owns it
	 */
	class Runnable
	{
	public:

		/**
		 * \brief Default destructor
		 */
		virtual ~Runnable() = default;

		/**
		 * \brief Initializes resources before the thread starts running
		 *
		 * Called once before Run()
		 */
		virtual void Init() = 0;

		/**
		 * \brief Main execution method of the thread
		 *
		 * This method is executed on the thread It should typically contain
		 * the task's main loop or logic
		 *
		 * \return The return code of the thread
		 */
		virtual unsigned int Run() = 0;

		/**
		 * \brief Stops the thread gracefully
		 *
		 * Called to request a clean shutdown of the task
		 * This should set internal flags to exit Run()
		 *
		 * \remark Called from another thread than the one running the task, so whatever Run()
		 *         reads to notice the request has to be synchronized
		 */
		virtual void Stop() = 0;

		/**
		 * \brief Cleanup resources before the thread ends
		 *
		 * Called once after Run()
		 */
		virtual void Exit() = 0;
	};

    namespace Detail
    {

        /**
         * \brief Runnable forwarding to a callable that takes no argument
         *
         * \tparam TCallable Type of the wrapped callable
         *
         * \remark Init(), Stop() and Exit() do nothing: a bare callable has nothing to set up or
         *         tear down, and exposes no way to be interrupted. A task meant to answer
         *         Thread::Kill needs a Runnable of its own, or a stop flag its callable captures
         */
        template <typename TCallable>
        class CallableRunnable : public Runnable
        {
        public:

            /**
             * \brief Takes hold of the callable to invoke
             *
             * \param callable Callable invoked by Run()
             */
            explicit CallableRunnable(TCallable&& callable) :
                mCallable(Forward<TCallable>(callable))
            {
                /// Nothing
            }

            void Init() override
            {
            }

            /**
             * \brief Invokes the callable
             *
             * \return Always 0, a bare callable reports no exit code
             */
            unsigned int Run() override
            {
                mCallable();

                return 0;
            }

            void Stop() override
            {
            }

            void Exit() override
            {
            }

        private:

            TCallable mCallable; /*!< Callable invoked by Run() */
        };

        /**
         * \brief Runnable forwarding to a callable, along with the arguments to call it with
         *
         * \tparam TCallable Type of the wrapped callable
         * \tparam TArgs Types of the stored arguments, already decayed by the factory
         *
         * \remark The arguments are held by value and moved into the call, so Run() consumes
         *         them: an instance carrying arguments runs once. See MakeRunnable for the
         *         reasoning behind copying them
         *
         * \remark Init(), Stop() and Exit() do nothing, as in CallableRunnable
         */
        template <typename TCallable, typename... TArgs>
        class ParameterCallableRunnable : public Runnable
        {
        public:

            using ParameterType = std::tuple<TArgs...>;

            /**
             * \brief Takes hold of the callable and of the arguments to call it with
             *
             * \param callable Callable invoked by Run()
             * \param parameters Arguments to pass to \p callable
             */
            ParameterCallableRunnable(TCallable&& callable, ParameterType&& parameters) :
                mCallable(Forward<TCallable>(callable)),
                mParameters(Move(parameters))
            {
                /// Nothing
            }

            void Init() override
            {
            }

            /**
             * \brief Invokes the callable with the stored arguments
             *
             * \return Always 0, a bare callable reports no exit code
             */
            unsigned int Run() override
            {
                std::apply([this](TArgs&... innerArgs) {
                    std::invoke(mCallable, Move(innerArgs)...);
                }, mParameters);

                return 0;
            }

            void Stop() override
            {
            }

            void Exit() override
            {
            }

        private:

            TCallable mCallable; /*!< Callable invoked by Run() */
            ParameterType mParameters; /*!< Arguments passed to the callable */
        };

        /**
         * \brief Runnable forwarding to a member function of an object
         *
         * \tparam T Type owning the member function
         * \tparam TFunction Type of the member function pointer
         * \tparam TArgs Types of the stored arguments, already decayed by the factory
         *
         * \remark \c TFunction is a parameter of its own rather than rebuilt from \c TArgs: the
         *         signature of the member function and the types stored here are two different
         *         things, precisely because the arguments are decayed on the way in
         *
         * \remark The object is held by reference and never copied, so it has to outlive the
         *         task. Its arguments, on the other hand, are held by value
         *
         * \remark The arguments are moved into the call, so Run() consumes them: an instance
         *         carrying arguments runs once
         *
         * \remark Init(), Stop() and Exit() do nothing, as in CallableRunnable
         */
        template <typename T, typename TFunction, typename... TArgs>
        class ObjectRunnable : public Runnable
        {
        public:

            using ObjectType = T;
            using ReferenceType = T&;
            using FunctionType = TFunction;
            using ParameterType = std::tuple<TArgs...>;

            /**
             * \brief Takes hold of the object, the member function and the arguments
             *
             * \param object Object the member function is called on
             * \param function Member function invoked by Run()
             * \param parameters Arguments to pass to \p function
             */
            ObjectRunnable(ReferenceType object, FunctionType function, ParameterType&& parameters) :
                mObject(object),
                mFunction(function),
                mParameters(Move(parameters))
            {
                /// Nothing
            }

            void Init() override
            {
            }

            /**
             * \brief Invokes the member function on the object with the stored arguments
             *
             * \return Always 0, a member function returning void reports no exit code
             */
            unsigned int Run() override
            {
                std::apply([this](TArgs&... innerArgs) {
                    std::invoke(mFunction, mObject, Move(innerArgs)...);
                }, mParameters);

                return 0;
            }

            void Stop() override
            {
            }

            void Exit() override
            {
            }

        private:

            ReferenceType mObject; /*!< Object the member function is called on */
            FunctionType mFunction; /*!< Member function invoked by Run() */
            ParameterType mParameters; /*!< Arguments passed to the member function */
        };

        /**
         * \brief Runnable forwarding to a const member function of an object
         *
         * \tparam T Type owning the member function
         * \tparam TFunction Type of the member function pointer
         * \tparam TArgs Types of the stored arguments, already decayed by the factory
         *
         * \remark Same notes as ObjectRunnable, the object simply being held by const reference
         */
        template <typename T, typename TFunction, typename... TArgs>
        class ConstObjectRunnable : public Runnable
        {
        public:

            using ObjectType = T;
            using ReferenceType = const T&;
            using FunctionType = TFunction;
            using ParameterType = std::tuple<TArgs...>;

            /**
             * \brief Takes hold of the object, the member function and the arguments
             *
             * \param object Object the member function is called on
             * \param function Member function invoked by Run()
             * \param parameters Arguments to pass to \p function
             */
            ConstObjectRunnable(ReferenceType object, FunctionType function, ParameterType&& parameters) :
                mObject(object),
                mFunction(function),
                mParameters(Move(parameters))
            {
                /// Nothing
            }

            void Init() override
            {
            }

            /**
             * \brief Invokes the member function on the object with the stored arguments
             *
             * \return Always 0, a member function returning void reports no exit code
             */
            unsigned int Run() override
            {
                std::apply([this](TArgs&... innerArgs) {
                    std::invoke(mFunction, mObject, Move(innerArgs)...);
                }, mParameters);

                return 0;
            }

            void Stop() override
            {
            }

            void Exit() override
            {
            }

        private:

            ReferenceType mObject; /*!< Object the member function is called on */
            FunctionType mFunction; /*!< Member function invoked by Run() */
            ParameterType mParameters; /*!< Arguments passed to the member function */
        };
    }

    /**
     * \brief Builds a runnable out of a callable taking no argument
     *
     * \tparam TCallable Type of the callable
     *
     * \param callable Callable to invoke when the task runs
     *
     * \return A runnable invoking \p callable
     *
     * Example:
     * \code
     * UniquePtr<Runnable> task = MakeRunnable([] {
     *     // Code executed inside the thread
     * });
     * UniquePtr<Thread> thread = Thread::Create(task.Get(), CK_TEXT("Worker"));
     * \endcode
     *
     * \remark The returned runnable has to outlive the thread running it: Thread::Create takes
     *         a pointer and never owns it
     *
     * \remark A callable capturing by reference still refers to whatever it captured, which the
     *         caller is left to keep alive. Only arguments passed through the overload below are
     *         copied
     *
     * \remark The constraint is what keeps this overload from swallowing the member function
     *         calls below: on a non-const object handed a const member function it would
     *         otherwise win, its reference binding being the better match
     */
    template <typename TCallable>
        requires std::invocable<TCallable&>
    UniquePtr<Runnable> MakeRunnable(TCallable&& callable)
    {
        using RunnableType = Detail::CallableRunnable<TCallable>;
        return MakeUnique<RunnableType>(Forward<TCallable>(callable));
    }

    /**
     * \brief Builds a runnable out of a callable and the arguments to call it with
     *
     * \tparam TCallable Type of the callable
     * \tparam TArgs Types of the arguments, as written at the call site
     *
     * \param callable Callable to invoke when the task runs
     * \param args Arguments to pass to \p callable
     *
     * \return A runnable invoking \p callable with \p args
     *
     * Example:
     * \code
     * UniquePtr<Runnable> task = MakeRunnable([](unsigned int index) {
     *     // Code executed inside the thread
     * }, workerIndex);
     * \endcode
     *
     * \remark The arguments are decay copied into the runnable, the way std::thread does it, so
     *         that the task never reaches back into a call site that may already be gone. A
     *         reference parameter therefore binds to the copy the runnable holds, not to the
     *         object passed here
     *
     * \remark Being copies, they are moved into the call: a runnable built with arguments runs
     *         once. Build another one to run the task again
     *
     * \remark To have the task work on the caller's object rather than on a copy, pass a pointer
     *         to it, or use the member function overload below whose object is held by reference
     *
     * \remark See the note on the overload above about the constraint
     */
    template <typename TCallable, typename... TArgs>
        requires std::invocable<TCallable&, std::decay_t<TArgs>...>
    UniquePtr<Runnable> MakeRunnable(TCallable&& callable, TArgs&&... args)
    {
        using RunnableType = Detail::ParameterCallableRunnable<TCallable, std::decay_t<TArgs>...>;
        using ParameterType = typename RunnableType::ParameterType;

        return MakeUnique<RunnableType>(Forward<TCallable>(callable), ParameterType(Forward<TArgs>(args)...));
    }

    /**
     * \brief Builds a runnable out of a member function of an object
     *
     * \tparam T Type owning the member function
     * \tparam TArgs Parameter types of \p function
     * \tparam TCallArgs Types of the arguments, as written at the call site
     *
     * \param object Object to call \p function on
     * \param function Member function to invoke when the task runs
     * \param args Arguments to pass to \p function
     *
     * \return A runnable invoking \p function on \p object with \p args
     *
     * Example:
     * \code
     * Simulation simulation;
     * UniquePtr<Runnable> task = MakeRunnable(simulation, &Simulation::Step, stepCount);
     * \endcode
     *
     * \remark \p object is held by reference and never copied, so it has to outlive the task.
     *         \p args are decay copied, as in the overload above, and moved into the call, so a
     *         runnable built with arguments runs once
     *
     * \remark \c TCallArgs is a pack of its own rather than \c TArgs reused: deducing a single
     *         pack from both the signature of \p function and the arguments of the call yields
     *         conflicting results as soon as the two differ, say an \c int parameter passed an
     *         \c int lvalue, which silently drops this overload in favour of the plain callable one
     */
    template <typename T, typename... TArgs, typename... TCallArgs>
    UniquePtr<Runnable> MakeRunnable(T& object, void (T::*function)(TArgs...), TCallArgs&&... args)
    {
        using RunnableType = Detail::ObjectRunnable<T, void (T::*)(TArgs...), std::decay_t<TCallArgs>...>;
        using ParameterType = typename RunnableType::ParameterType;

        return MakeUnique<RunnableType>(object, function, ParameterType(Forward<TCallArgs>(args)...));
    }

    /**
     * \brief Builds a runnable out of a const member function of an object
     *
     * \tparam T Type owning the member function
     * \tparam TArgs Parameter types of \p function
     * \tparam TCallArgs Types of the arguments, as written at the call site
     *
     * \param object Object to call \p function on
     * \param function Member function to invoke when the task runs
     * \param args Arguments to pass to \p function
     *
     * \return A runnable invoking \p function on \p object with \p args
     *
     * \remark Same notes as the non-const overload above
     */
    template <typename T, typename... TArgs, typename... TCallArgs>
    UniquePtr<Runnable> MakeRunnable(const T& object, void (T::*function)(TArgs...) const, TCallArgs&&... args)
    {
        using RunnableType = Detail::ConstObjectRunnable<T, void (T::*)(TArgs...) const, std::decay_t<TCallArgs>...>;
        using ParameterType = typename RunnableType::ParameterType;

        return MakeUnique<RunnableType>(object, function, ParameterType(Forward<TCallArgs>(args)...));
    }
}

#endif // COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_RUNNABLE_HPP
