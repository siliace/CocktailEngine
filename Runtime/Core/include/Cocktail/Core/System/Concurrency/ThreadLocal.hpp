#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADLOCAL_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADLOCAL_HPP

#include <functional>

#include <Cocktail/Core/System/Concurrency/ThreadLocalStorage.hpp>

namespace Ck
{
    /**
     * \class ThreadLocal
     *
     * \brief Provides thread-local storage for objects of type T
     *
     * This template class allows each thread to have its own instance of an object.
     * The object is created on demand using a factory function. By default, the object
     * is created using the default constructor of T.
     *
     * \tparam T The type of object to store in thread-local storage
     */
    template <typename T>
    class ThreadLocal
    {
    public:

        /**
         * \typedef Factory
         *
         * \brief Type of the factory function used to create objects of type T
         *
         * The factory function returns a UniquePtr<T> to a newly created object.
         */
        using Factory = std::function<UniquePtr<T>()>;

        /**
         * \brief Default constructor
         *
         * Creates a new thread local instance using a default factory
         *
         * Uses the default factory function which creates a new instance of T
         * using its default constructor.
         */
        ThreadLocal() :
            ThreadLocal(&DefaultFactory)
        {
            /// Nothing
        }

        /**
         * \brief Constructor with a custom factory
         *
         * \param factory A factory function to create objects of type T.
         */
        explicit ThreadLocal(Factory&& factory) :
            mFactory(factory)
        {
            /// Nothing
        }

        /**
         * \brief Deleted copy constructor
         */
        ThreadLocal(const ThreadLocal&) = delete;

        /**
         * \brief Move constructor
         *
         * Transfers ownership of the internal storage and factory from another ThreadLocal.
         *
         * \param other The ThreadLocal instance to move from
         */
        ThreadLocal(ThreadLocal&& other) noexcept
        {
            *this = std::move(other);
        }

        /**
         * \brief Deleted copy assignment operator
         */
        ThreadLocal& operator=(const ThreadLocal&) = delete;

        /**
         * \brief Move assignment operator
         *
         * Transfers ownership of the internal storage, values, and factory from another ThreadLocal.
         *
         * \param other The ThreadLocal instance to move from
         *
         * \return Reference to this ThreadLocal
         */
        ThreadLocal& operator=(ThreadLocal&& other) noexcept
        {
            if (this == &other)
                return *this;

            std::lock_guard lg1(mValueLock);
            std::lock_guard lg2(other.mValueLock);

            mFactory = std::move(other.mFactory);
            mStorage = std::move(other.mStorage);
            mValues = std::move(other.mValues);

            return *this;
        }

        /**
         * \brief Returns the thread-local object for the current thread
         *
         * If the object does not exist for the current thread, it is created using
         * the factory function.
         *
         * \return Pointer to the thread-local object
         */
        T* Get()
        {
            if (T* value = static_cast<T*>(mStorage.Get()))
                return value;

            UniquePtr<T> value = mFactory();
            mStorage.Set(value.Get());

            std::lock_guard lg(mValueLock);
            mValues.Add(std::move(value));

            return mValues.Last().Get();
        }

    private:

        /**
         * \brief Default factory function
         *
         * Creates a new object of type T using its default constructor.
         *
         * \return UniquePtr<T> to the newly created object
         */
        static UniquePtr<T> DefaultFactory()
        {
            return MakeUnique<T>();
        }

        Factory mFactory; /*!< Factory function to create new objects */
        ThreadLocalStorage mStorage; /*!< Platform-specific thread-local storage */
        Array<UniquePtr<T>> mValues; /*!< Container to keep ownership of created objects */
        mutable std::mutex mValueLock; /*!< Mutex to protect access to mValues */
    };
}

#endif // COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADLOCAL_HPP
