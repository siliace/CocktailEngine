#ifndef COCKTAIL_D3D12_BUFFER_BUFFER_HPP
#define COCKTAIL_D3D12_BUFFER_BUFFER_HPP

#include <memory>

#include <d3d12.h>
#include <wrl/client.h>

#include <Cocktail/Renderer/Buffer/Buffer.hpp>
#include <Cocktail/Renderer/Buffer/BufferCreateInfo.hpp>

namespace Ck::D3D12
{
    class RenderDevice;

    class Buffer : public Renderer::Buffer, public std::enable_shared_from_this<Buffer>
    {
    public:

        Buffer(RenderDevice* renderDevice, const Renderer::BufferCreateInfo& createInfo);

        void SetObjectName(const char* name) const override;

        Renderer::RenderDevice* GetRenderDevice() const override;

        bool IsExclusive() const override;

        Renderer::MemoryType GetMemoryType() const override;

        Renderer::ResourceType GetResourceType() const override;

        Renderer::BufferUsageFlags GetUsage() const override;

        std::size_t GetSize() const override;

    private:

        RenderDevice* mRenderDevice;
        Microsoft::WRL::ComPtr<ID3D12Resource> mHandle;
        Renderer::BufferUsageFlags mUsage;
        std::size_t mSize;
        Renderer::MemoryType mMemoryType;
    };
}

#endif // COCKTAIL_D3D12_BUFFER_BUFFER_HPP
