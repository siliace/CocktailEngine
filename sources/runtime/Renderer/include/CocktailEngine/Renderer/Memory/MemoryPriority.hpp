#ifndef COCKTAILENGINE_RENDERER_MEMORY_MEMORYPRIORITY_HPP
#define COCKTAILENGINE_RENDERER_MEMORY_MEMORYPRIORITY_HPP

namespace Ck::Renderer
{
    enum class MemoryPriority
    {
        Critical,
        High,
        Medium,
        Low,
        VeryLow,
        Streaming
    };
}

#endif // COCKTAILENGINE_RENDERER_MEMORY_MEMORYPRIORITY_HPP
