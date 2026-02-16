#ifndef COCKTAIL_D3D12_CONTEXT__RENDERSURFACE_HPP
#define COCKTAIL_D3D12_CONTEXT__RENDERSURFACE_HPP

#include <Cocktail/Renderer/Context/RenderSurface.hpp>
#include <Cocktail/Renderer/Context/RenderSurfaceCreateInfo.hpp>

namespace Ck::D3D12
{
    class RenderDevice;

    class RenderSurface : public Renderer::RenderSurface
    {
    public:

        RenderSurface(RenderDevice* renderDevice, const Renderer::RenderSurfaceCreateInfo& createInfo);
        void SetObjectName(const char* name) const override;
        Renderer::RenderDevice* GetRenderDevice() const override;
        Extent2D<unsigned int> GetSize() const override;
        PixelFormat GetColorFormat() const override;
        PixelFormat DepthStencilFormat() const override;
        Renderer::RasterizationSamples GetSamples() const override;
        bool IsVsyncEnabled() const override;
        void EnableVSync(bool enable) override;
        unsigned GetBufferCount() const override;

    private:

        RenderDevice* mRenderDevice;
    };
}

#endif // COCKTAIL_D3D12_CONTEXT__RENDERSURFACE_HPP
