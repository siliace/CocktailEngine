#include <Cocktail/D3D12/Context/RenderContext.hpp>
#include <Cocktail/D3D12/RenderDevice.hpp>

namespace Ck::D3D12
{
    RenderContext::RenderContext(RenderDevice* renderDevice, const Renderer::RenderContextCreateInfo& createInfo) :
        mRenderDevice(renderDevice)
    {
        /// Nothing
    }

    void RenderContext::SetObjectName(const char* name) const
    {
    }

    Renderer::RenderDevice* RenderContext::GetRenderDevice() const
    {
        return mRenderDevice;
    }

    Renderer::BufferAllocator* RenderContext::GetBufferAllocator(Renderer::BufferUsageFlags usage, Renderer::MemoryType memoryType)
    {
        return nullptr;
    }

    Renderer::Framebuffer* RenderContext::AcquireFramebuffer(Renderer::RenderSurface* renderSurface)
    {
        return nullptr;
    }

    Renderer::CommandList* RenderContext::CreateCommandList(const Renderer::CommandListCreateInfo& createInfo)
    {
        return nullptr;
    }

    void RenderContext::SignalQueue(Renderer::CommandQueueType queue)
    {
    }

    void RenderContext::WaitQueue(Renderer::CommandQueueType waitingQueue, Renderer::CommandQueueType waitedQueue)
    {
    }

    void RenderContext::SubmitCommandLists(Renderer::CommandQueueType queue, unsigned commandListCount, Renderer::CommandList** commandLists, Renderer::Fence* fence)
    {
    }

    void RenderContext::Flush()
    {
    }

    void RenderContext::Synchronize()
    {
    }
}
