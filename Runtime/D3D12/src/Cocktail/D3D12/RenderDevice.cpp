#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/D3D12/D3D12.hpp>
#include <Cocktail/D3D12/Buffer/Buffer.hpp>
#include <Cocktail/D3D12/Buffer/BufferView.hpp>
#include <Cocktail/D3D12/Context/RenderContext.hpp>
#include <Cocktail/D3D12/RenderDevice.hpp>

#include "Cocktail/D3D12/Command/Fence.hpp"

namespace Ck::D3D12
{
    namespace
    {
        D3D12_COMMAND_LIST_TYPE ToD3D12Type(Renderer::CommandQueueType commandQueueType)
        {
            switch (commandQueueType)
            {
                case Renderer::CommandQueueType::Graphic: return D3D12_COMMAND_LIST_TYPE_DIRECT;
                case Renderer::CommandQueueType::Transfer: return D3D12_COMMAND_LIST_TYPE_COMPUTE;
                case Renderer::CommandQueueType::Compute: return D3D12_COMMAND_LIST_TYPE_COPY;
            }

            COCKTAIL_UNREACHABLE();
        }
    }

    RenderDevice::RenderDevice()
    {
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&mDebugController))))
            mDebugController->EnableDebugLayer();

        D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&mDevice));

        for (Renderer::CommandQueueType commandQueueType : Enum<Renderer::CommandQueueType>::Values)
        {
            D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
            commandQueueDesc.Type = ToD3D12Type(commandQueueType);
            commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
            commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
            commandQueueDesc.NodeMask = 0;

            Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
            auto hr = mDevice->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
            if (!SUCCEEDED(hr))
            {
                CK_LOG(D3D12LogCategory, LogLevel::Error, CK_TEXT("Failed to create ID3D12CommandQueue for CommandQueueType %s"), commandQueueType);
                continue;
            }

            mCommandQueues[commandQueueType] = commandQueue;
        }
    }

    std::shared_ptr<Renderer::Buffer> RenderDevice::CreateBuffer(const Renderer::BufferCreateInfo& createInfo)
    {
        return std::make_shared<Buffer>(this, createInfo);
    }

    std::shared_ptr<Renderer::BufferView> RenderDevice::CreateBufferView(const Renderer::BufferViewCreateInfo& createInfo)
    {
        return std::make_shared<BufferView>(this, createInfo);
    }

    std::shared_ptr<Renderer::Fence> RenderDevice::CreateFence(const Renderer::FenceCreateInfo& createInfo)
    {
        return std::make_shared<Fence>(this, createInfo);
    }

    std::shared_ptr<Renderer::Framebuffer> RenderDevice::CreateFramebuffer(const Renderer::FramebufferCreateInfo& createInfo)
    {
        return nullptr;
    }

    std::shared_ptr<Renderer::RenderContext> RenderDevice::CreateRenderContext(const Renderer::RenderContextCreateInfo& createInfo)
    {
        return std::make_shared<RenderContext>(this, createInfo);
    }

    std::shared_ptr<Renderer::RenderSurface> RenderDevice::CreateRenderSurface(const Renderer::RenderSurfaceCreateInfo& createInfo)
    {
        return nullptr;
    }

    std::shared_ptr<Renderer::Sampler> RenderDevice::CreateSampler(const Renderer::SamplerCreateInfo& createInfo)
    {
        return nullptr;
    }

    std::shared_ptr<Renderer::Shader> RenderDevice::CreateShader(const Renderer::ShaderCreateInfo& createInfo)
    {
        return nullptr;
    }

    std::shared_ptr<Renderer::ShaderProgram> RenderDevice::CreateShaderProgram(const Renderer::ShaderProgramCreateInfo& createInfo)
    {
        return nullptr;
    }

    std::shared_ptr<Renderer::Texture> RenderDevice::CreateTexture(const Renderer::TextureCreateInfo& createInfo)
    {
        return nullptr;
    }

    std::shared_ptr<Renderer::TextureView> RenderDevice::CreateTextureView(const Renderer::TextureViewCreateInfo& createInfo)
    {
        return nullptr;
    }

    bool RenderDevice::IsExtensionSupported(Renderer::RenderDeviceExtension extension) const
    {
        return false;
    }

    Renderer::TextureUsageFlags RenderDevice::GetTextureFormatSupport(const PixelFormat& format, Renderer::MemoryType memoryType) const
    {
        return {};
    }

    bool RenderDevice::IsShadingRateSupported(Extent2D<unsigned int> fragmentSize, Renderer::RasterizationSamples samples)
    {
        return false;
    }

    Signal<LogLevel, Renderer::MessageType, AsciiStringView>& RenderDevice::OnDebugMessage()
    {
        return mOnDebugMessage;
    }

    ID3D12Device* RenderDevice::GetHandle() const
    {
        return mDevice.Get();
    }
}
