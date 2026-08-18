#ifndef COCKTAILENGINE_DEBUGGERCONSOLELOG_HPP
#define COCKTAILENGINE_DEBUGGERCONSOLELOG_HPP

#include <CocktailEngine/Core/Log/LogChannel.hpp>

namespace Ck::Main
{
    class DebuggerLogChannel : public LogChannel
    {
    public:

        void WriteEntry(const LogEntry& entry) override;
    };
}

#endif // COCKTAILENGINE_DEBUGGERCONSOLELOG_HPP
