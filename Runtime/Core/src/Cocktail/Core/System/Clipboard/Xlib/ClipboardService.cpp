#include <Cocktail/Core/System/Clipboard/Xlib/ClipboardService.hpp>

namespace Ck::Detail::Xlib
{
    std::size_t ClipboardService::Get(void* data, std::size_t length, std::size_t offset)
    {
        return 0;
    }

    std::size_t ClipboardService::GetSize() const
    {
        return 0;
    }

    void ClipboardService::Set(const void* data, std::size_t length)
    {
    }

    void ClipboardService::Clear()
    {
    }
}
