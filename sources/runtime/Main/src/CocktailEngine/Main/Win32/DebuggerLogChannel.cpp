#include <Windows.h>
#include <debugapi.h>

#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/Log/LogEntry.hpp>

#include <CocktailEngine/Main/Win32/DebuggerLogChannel.hpp>

namespace Ck::Main
{
    void DebuggerLogChannel::WriteEntry(const LogEntry& entry)
    {
        OutputDebugString(entry.Message.GetData());
    }
}
