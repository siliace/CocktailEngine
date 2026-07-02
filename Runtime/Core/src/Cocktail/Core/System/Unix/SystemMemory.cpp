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
        static const std::size_t pageSize = static_cast<std::size_t>(sysconf(_SC_PAGESIZE));
        return pageSize;
    }
}
