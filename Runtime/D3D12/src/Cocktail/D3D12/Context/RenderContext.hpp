#ifndef COCKTAIL_D3D12_CONTEXT_RENDERCONTEXT_HPP
#define COCKTAIL_D3D12_CONTEXT_RENDERCONTEXT_HPP

#include <memory>

#include <d3d12.h>
#include <wrl/client.h>

#include <Cocktail/Renderer/Context/RenderContext.hpp>
#include <Cocktail/Renderer/Context/RenderContextCreateInfo.hpp>

namespace Ck::D3D12
{
    class RenderDevice;

    class RenderContext : public Renderer::RenderContext, std::enable_shared_from_this<RenderContext>
    {
    public:

        RenderContext(RenderDevice* renderDevice, const Renderer::RenderContextCreateInfo& createInfo);

        void SetObjectName(const char* name) const override;
        Renderer::RenderDevice* GetRenderDevice() const override;
        Renderer::BufferAllocator* GetBufferAllocator(Renderer::BufferUsageFlags usage, Renderer::MemoryType memoryType) override;
        Renderer::Framebuffer* AcquireFramebuffer(Renderer::RenderSurface* renderSurface) override;
        Renderer::CommandList* CreateCommandList(const Renderer::CommandListCreateInfo& createInfo) override;
        void SignalQueue(Renderer::CommandQueueType queue) override;
        void WaitQueue(Renderer::CommandQueueType waitingQueue, Renderer::CommandQueueType waitedQueue) override;
        void SubmitCommandLists(Renderer::CommandQueueType queue, unsigned commandListCount, Renderer::CommandList** commandLists, Renderer::Fence* fence) override;
        void Flush() override;
        void Synchronize() override;

    private:

        RenderDevice* mRenderDevice;
    };
}

#endif // COCKTAIL_D3D12_CONTEXT_RENDERCONTEXT_HPP
