#ifndef COCKTAIL_D3D12_BUFFER_BUFFERVIEW_HPP
#define COCKTAIL_D3D12_BUFFER_BUFFERVIEW_HPP

#include <memory>

#include <d3d12.h>
#include <wrl/client.h>

#include <Cocktail/Renderer/Buffer/BufferView.hpp>
#include <Cocktail/Renderer/Buffer/BufferViewCreateInfo.hpp>

namespace Ck::D3D12
{
    class RenderDevice;
    
    class BufferView : public Renderer::BufferView, public std::enable_shared_from_this<BufferView>
    {
    public:

        BufferView(RenderDevice* renderDevice, const Renderer::BufferViewCreateInfo& createInfo);
        void SetObjectName(const char* name) const override;
        Renderer::RenderDevice* GetRenderDevice() const override;
        std::shared_ptr<Renderer::Buffer> GetBuffer() const override;
        PixelFormat GetFormat() const override;
        std::size_t GetOffset() const override;
        std::size_t GetRange() const override;

    private:

        RenderDevice* mRenderDevice;
    };
}

#endif // COCKTAIL_D3D12_BUFFER_BUFFERVIEW_HPP
