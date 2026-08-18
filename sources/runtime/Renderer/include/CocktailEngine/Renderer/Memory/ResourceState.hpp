#ifndef COCKTAILENGINE_RENDERER_MEMORY_RESOURCESTATE_HPP
#define COCKTAILENGINE_RENDERER_MEMORY_RESOURCESTATE_HPP

namespace Ck::Renderer
{
    enum class ResourceState
    {
        Undefined,
        General,
        FramebufferAttachment,
        GraphicShaderResource,
        ComputeShaderResource,
        CopySource,
        CopyDestination,
        VertexBuffer,
        IndexBuffer,
        UniformBuffer,
    };
}

#endif // COCKTAILENGINE_RENDERER_MEMORY_RESOURCESTATE_HPP
