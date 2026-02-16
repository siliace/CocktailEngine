#include <Cocktail/D3D12/Context/RenderSurface.hpp>
#include <Cocktail/D3D12/RenderDevice.hpp>

namespace Ck::D3D12
{
    RenderSurface::RenderSurface(RenderDevice* renderDevice, const Renderer::RenderSurfaceCreateInfo& createInfo) :
        mRenderDevice(renderDevice)
    {
    }

    void RenderSurface::SetObjectName(const char* name) const
    {
    }

    Renderer::RenderDevice* RenderSurface::GetRenderDevice() const
    {
        return mRenderDevice;
    }

    Extent2D<unsigned int> RenderSurface::GetSize() const
    {
        return { 0u, 0u };
    }

    PixelFormat RenderSurface::GetColorFormat() const
    {
        return PixelFormat::Undefined();
    }

    PixelFormat RenderSurface::DepthStencilFormat() const
    {
        return PixelFormat::Undefined();
    }

    Renderer::RasterizationSamples RenderSurface::GetSamples() const
    {
        return Renderer::RasterizationSamples::e1;
    }

    bool RenderSurface::IsVsyncEnabled() const
    {
        return false;
    }

    void RenderSurface::EnableVSync(bool enable)
    {
    }

    unsigned int RenderSurface::GetBufferCount() const
    {
        return 0;
    }
}
