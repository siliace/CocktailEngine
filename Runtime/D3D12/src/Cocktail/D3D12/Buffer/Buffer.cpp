#include <Cocktail/D3D12/Buffer/Buffer.hpp>
#include <Cocktail/D3D12/RenderDevice.hpp>

namespace Ck::D3D12
{
    Buffer::Buffer(RenderDevice* renderDevice, const Renderer::BufferCreateInfo& createInfo) :
        mRenderDevice(renderDevice),
        mUsage(createInfo.Usage),
        mSize(createInfo.Size),
        mMemoryType(createInfo.MemoryType)
    {
        D3D12_HEAP_PROPERTIES heapProps = {};
        heapProps.Type = mMemoryType == Renderer::MemoryType::Static ? D3D12_HEAP_TYPE_DEFAULT : D3D12_HEAP_TYPE_UPLOAD;

        D3D12_RESOURCE_DESC desc = {};
        desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        desc.Alignment = 0;
        desc.Width = mSize;
        desc.Height = 1;
        desc.DepthOrArraySize = 1;
        desc.MipLevels = 1;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        desc.Flags = D3D12_RESOURCE_FLAG_NONE;

        mRenderDevice->GetHandle()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mHandle));
    }

    void Buffer::SetObjectName(const char* name) const
    {
    }

    Renderer::RenderDevice* Buffer::GetRenderDevice() const
    {
        return mRenderDevice;
    }

    bool Buffer::IsExclusive() const
    {
        return true;
    }

    Renderer::MemoryType Buffer::GetMemoryType() const
    {
        return mMemoryType;
    }

    Renderer::ResourceType Buffer::GetResourceType() const
    {
        return Renderer::ResourceType::Buffer;
    }

    Renderer::BufferUsageFlags Buffer::GetUsage() const
    {
        return mUsage;
    }

    std::size_t Buffer::GetSize() const
    {
        return mSize;
    }
}
