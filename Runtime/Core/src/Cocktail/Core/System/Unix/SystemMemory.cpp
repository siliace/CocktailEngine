#include <sys/mman.h>
#include <unistd.h>

#include <Cocktail/Core/System/SystemMemory.hpp>

namespace Ck
{
    void* SystemMemory::Allocate(std::size_t size)
    {
        return mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }

    void SystemMemory::Free(void* pointer, std::size_t size)
    {
        munmap(pointer, size);
    }

    std::size_t SystemMemory::GetPageSize()
    {
        return sysconf(_SC_PAGESIZE);
    }
}
