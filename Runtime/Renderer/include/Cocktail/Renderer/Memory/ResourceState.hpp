#ifndef COCKTAIL_RENDERER_MEMORY_RESOURCESTATE_HPP
#define COCKTAIL_RENDERER_MEMORY_RESOURCESTATE_HPP

namespace Ck::Renderer
{
    enum class ResourceState
    {
        Undefined,
        General,
        GraphicShaderResource,
        ComputeShaderResource,
        CopySource,
        CopyDestination,
        VertexBuffer,
        IndexBuffer,
        UniformBuffer,
    };
}

#endif // COCKTAIL_RENDERER_MEMORY_RESOURCESTATE_HPP
