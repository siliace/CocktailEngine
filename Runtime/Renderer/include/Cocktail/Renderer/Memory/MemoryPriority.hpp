#ifndef COCKTAIL_RENDERER_MEMORY_MEMORYPRIORITY_HPP
#define COCKTAIL_RENDERER_MEMORY_MEMORYPRIORITY_HPP

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

#endif // COCKTAIL_RENDERER_MEMORY_MEMORYPRIORITY_HPP
