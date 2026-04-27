#include <Windows.h>
#include <debugapi.h>

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/LogEntry.hpp>

#include <Cocktail/Main/Win32/DebuggerLogChannel.hpp>

namespace Ck::Main
{
    void DebuggerLogChannel::WriteEntry(const LogEntry& entry)
    {
        OutputDebugString(entry.Message.GetData());
    }
}
