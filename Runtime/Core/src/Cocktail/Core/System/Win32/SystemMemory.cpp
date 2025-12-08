#include <Cocktail/Core/System/SystemMemory.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck
{
    void* SystemMemory::Allocate(std::size_t size)
    {
        return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    }

    void SystemMemory::Free(void* pointer, std::size_t)
    {
        VirtualFree(pointer, 0, MEM_RELEASE);
    }

    std::size_t SystemMemory::GetPageSize()
    {
        SYSTEM_INFO systemInfo;
        GetSystemInfo(&systemInfo);

        return systemInfo.dwPageSize;
    }
}
