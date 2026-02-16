#include <Cocktail/D3D12/RenderDevice.hpp>
#include <Cocktail/D3D12/Buffer/BufferView.hpp>

namespace Ck
{
    D3D12::BufferView::BufferView(RenderDevice* renderDevice, const Renderer::BufferViewCreateInfo& createInfo) :
        mRenderDevice(renderDevice)
    {
        /// Nothing
    }

    void D3D12::BufferView::SetObjectName(const char* name) const
    {
    }

    Renderer::RenderDevice* D3D12::BufferView::GetRenderDevice() const
    {
        return mRenderDevice;
    }

    std::shared_ptr<Renderer::Buffer> D3D12::BufferView::GetBuffer() const
    {
        return nullptr;
    }

    PixelFormat D3D12::BufferView::GetFormat() const
    {
        return PixelFormat::Undefined();
    }

    std::size_t D3D12::BufferView::GetOffset() const
    {
        return 0;
    }

    std::size_t D3D12::BufferView::GetRange() const
    {
        return 0;
    }
}
