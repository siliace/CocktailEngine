#ifndef COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADLOCALSTORAGE_HPP
#define COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADLOCALSTORAGE_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
    class COCKTAIL_CORE_API ThreadLocalStorage
    {
    public:

        using Handle = unsigned int;

        ThreadLocalStorage();
        ThreadLocalStorage(const ThreadLocalStorage& other) = delete;
        ThreadLocalStorage(ThreadLocalStorage&& other) noexcept;
        ~ThreadLocalStorage();

        ThreadLocalStorage& operator=(const ThreadLocalStorage& other) = delete;
        ThreadLocalStorage& operator=(ThreadLocalStorage&& other) noexcept;

        void* Get() const;
        void Set(void* pointer);

        Handle GetSystemHandle() const;

    private:

        static const Handle InvalidHandle;

        Handle mHandle;
    };
}

#endif //COCKTAIL_CORE_SYSTEM_CONCURRENCY_THREADLOCALSTORAGE_HPP
