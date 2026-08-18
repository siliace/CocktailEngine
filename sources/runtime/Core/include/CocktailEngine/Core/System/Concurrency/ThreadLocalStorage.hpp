#ifndef COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_THREADLOCALSTORAGE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_THREADLOCALSTORAGE_HPP

#include <CocktailEngine/Core/Export.hpp>

namespace Ck
{
    class COCKTAILENGINE_CORE_API ThreadLocalStorage
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

#endif //COCKTAILENGINE_CORE_SYSTEM_CONCURRENCY_THREADLOCALSTORAGE_HPP
